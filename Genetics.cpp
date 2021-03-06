#include "Genetics.h"

void Genetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness, string typePopulation) 
{

	/* Variables pour l'algorithme */
	clock_t	start, current;				//Temps d'�x�cutions d'une instance
	

	deque<SMSSDTSolution> population;	//Notre population
	SMSSDTSolution firstParent = NULL;	//Inidividu parent
	SMSSDTSolution secondParent = NULL; //Inidividu parent
	SMSSDTSolution bestSolution = NULL; //Meilleur inidividu d'une population

	int populationSize = 50;				//Taille de la population sur laquelle nous travaillons	
	int nombreDeGeneration = 10;			//Nombre de g�n�ration pour lequel notre algorithme va s'ex�cuter (CRITERE D'ARR�T)
	int generation = 0;						//G�n�ration initiale

	int numberChildren = 2*populationSize; //Nombre d'enfant � chaque g�n�ration
	double alpha = 0.1;					   //Probabilit� qu'un enfant subisse une mutation
	double alphaMem = 1.0;			       //Probabilit� de faire une descente locale sur un enfant dans algo m�m�tique
	/* ================================ */

	//L'algorithme n�cessite une population d'au moins 2 individus
	if (populationSize < 2) {
		cout << "Erreur population trop petite";
		return;
	}

	//Selon les param�tres d'entr�es la population de base est compl�tement al�atoire ou semi al�atoire
	if (typePopulation.compare("semiRandom") == 0 || typePopulation.compare("memetiqueSR") == 0) {
		cout << "On passe SemiRandom" << endl;
		population = InitializeSemiRandomPlebe(problem, populationSize);
		cout << "On a fini SemiRandom" << endl;
	} else {
		cout << "On passe Random" << endl;
		population = InitializeRandomPlebe(problem, populationSize);
	}
	
	//Affichage de notre meilleur individu initiallement
	cout << "==== Meilleur individu avant algo genetique ====" << endl;
	bestSolution = GetBestSolution(population);
	showLeS(&bestSolution);

	//Une it�ration est une ex�cution compl�te de l'algorithme
	for (int i = 0; i < iteration; i++) {
		start = clock(); //D�marage du chronom�tre

		//Tant que le crit�re d'arr�t n'est pas satisfait
		while (generation < nombreDeGeneration) {
			//R�cup�ration de l'inverse de l'ensemble des fonctions objectives de la population
			double totalInverseTardiness = GetInverseTotalTardinessPopulation(population); //Renvois -1 si un individus == 0;

			//Si -1 alors on a un individus optimal dans la population initiale, pas besoin d'ex�cuter l'algorithme
			if (totalInverseTardiness == -1) {
				cout << "BREAK DEPUIS POP INITIALE" << endl;
				bestSolution = GetBestSolution(population);
				showLeS(&bestSolution);
				break;
			}
			
			deque<SMSSDTSolution> newPopulation = population; //Stockage temporaire de la nouvelle population avant la s�lection finale

			//Croisement des parents pour former des enfants, chaque couple donne 2 enfants (j +=2)
			for (int j = 0; j < numberChildren; j += 2) {				
				do { //S�lection proportionnelle des parents
					firstParent = GetIndividuProportionnelle(population, totalInverseTardiness);
					secondParent = GetIndividuProportionnelle(population, totalInverseTardiness);
				} while (firstParent.Solution == secondParent.Solution); //Si les parents sont le m�me individus on refait le choix

				//Croisement pour cr�er les enfants, stockage des enfants dans newPopulation
				Crossover(firstParent, secondParent, population, &newPopulation, problem);
			}		

			//mutation potentielle des enfants selon la probabilit� alpha
			newPopulation = Mutation(newPopulation, alpha, problem, populationSize);

			if (typePopulation.compare("memetique") == 0) {
				newPopulation = Memetique(newPopulation, alphaMem, problem, populationSize, 1);
			}

			
			totalInverseTardiness = GetInverseTotalTardinessPopulation(newPopulation);
			//Si un enfant trouve un optimum (fctObj == 0) alors on arr�te l'algorithme, c'est une solution optimale
			if (totalInverseTardiness == -1) { 
				cout << "BREAK DEPUIS ENFANTS" << endl;
				population = newPopulation;
				bestSolution = GetBestSolution(population);
				showLeS(&bestSolution);
				break;
			}

			//selection proportionnelle de la nouvelle population
			population = NextGeneration(newPopulation, totalInverseTardiness, populationSize);
			generation++;
			cout << "generation  : " << generation << endl;
		}

		cout << "================================================" << endl;
		cout << "==== Meilleur individu APRES algo genetique ====" << endl;
		bestSolution = GetBestSolution(population); //R�cup�ration du meilleur individus dans la population restante
		showLeS(&bestSolution);
		cout << "Timer = " << (double(clock()) - double(start)) / CLOCKS_PER_SEC << " secondes" << endl;
		StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
	}
}

SMSSDTSolution GetBestSolution(deque<SMSSDTSolution>  population) {
	SMSSDTSolution bestSolution = population[0];
	for (int i = 1; i < population.size(); i++) {
		if (bestSolution.getObj() > population[i].getObj()) {
			bestSolution = population[i];
		}
	}

	return bestSolution;
}

deque<SMSSDTSolution> NextGeneration(deque<SMSSDTSolution> population, double totalInverseTardiness, int sizePopulation) {
	deque<SMSSDTSolution> newPopulation;
	for (int i = 0; i < sizePopulation; i++) {
		newPopulation.push_back(GetIndividuProportionnelle(population, totalInverseTardiness));
	}

	return newPopulation;
}

deque<SMSSDTSolution> Memetique(deque<SMSSDTSolution> population, double alphaMem, SMSSDTProblem* problem, int maxPopulation, int shutoff) {
	deque<SMSSDTSolution> temp = population;
	for (int i = maxPopulation - 1; i < population.size(); i++) {
		double rand = RandomValue(0, 1);
		if (rand < alphaMem) {
			LocalDescent(problem, &temp[i], shutoff);
		}
	}

	return temp;
}

void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> oldPopulation, deque<SMSSDTSolution> *newPopulation, SMSSDTProblem* problem)
{
	int minCrossover = 3;
	int maxCrossover = firstParent.Solution.size() - 3;
	int randCrossoverPoint = (int) RandomValue(minCrossover, maxCrossover);

	/*cout << endl << "firstParent.Solution.size() " << firstParent.Solution.size() << endl;
	cout << "CrossOverPoint " << randCrossoverPoint << endl;*/

	SMSSDTSolution firstChild = firstParent;
	SMSSDTSolution secondChild = secondParent;

	int i = 0;
	int j = 0;
	int k = 0;
	do {
		if (j == firstParent.Solution.size()) {
			cout << "======= -------- ====="<< endl;
			cout << "ERREUR CROISEMENT" << endl;
			cout << "Valeur de j " << j << endl;
			cout << "Valeur de i " << i << endl;
			cout << "Valeur de k " << k << endl;
			cout << "Crossover point " << randCrossoverPoint << endl;
			/*showLeS(&firstParent);
			showLeS(&secondParent);
			cout << "=========================" << endl;

			showLeS(&firstChild);
			cout << "=========================" << endl;

			showLeS(&secondChild);
			cout << "=========================" << endl;*/
			return;
		} else { 
			if (firstChild.notIn(secondParent.Solution[j], randCrossoverPoint, i) && i < randCrossoverPoint) {
				firstChild.Solution[i] = secondParent.Solution[j];
				i++;
			}

			if (secondChild.notIn(firstParent.Solution[j], randCrossoverPoint, k) && k < randCrossoverPoint) {
				secondChild.Solution[k] = firstParent.Solution[j];
				k++;
			}
		}	

		j++;		
	} while (i < randCrossoverPoint || k < randCrossoverPoint);


	Tools::Evaluer(problem, firstChild);
	Tools::Evaluer(problem, secondChild);
	/*cout << " ================================== " << endl;
	cout << "AFFICHAGE DU CROISEMENT WATCH OUT ! " << endl;
	cout << " ================================== " << endl;
	cout << " ======== Parent 1 ======== " << endl;
	showLeS(&firstParent);
	cout << " ======== Parent 2 ======== " << endl;
	showLeS(&secondParent);
	cout << " ======== Enfant 1 ======== " << endl;
	showLeS(&firstChild);
	cout << " ======== Enfant 2 ======== " << endl;
	showLeS(&secondChild);*/

	newPopulation->push_back(firstChild);
	newPopulation->push_back(secondChild);
}



double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population) {
	double totalTardiness = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].getObj() == 0) {
			cout << "mon objectif == 0" << endl;
			showLeS(&population[i]);
			totalTardiness = -1;
			break;
		}
		totalTardiness += 1/population[i].getObj();
	}

	return totalTardiness;
}

deque<SMSSDTSolution> Mutation(deque<SMSSDTSolution> population, double alpha, SMSSDTProblem* problem, int maxPopulation) {
	deque<SMSSDTSolution> temp = population;
	for (int i = maxPopulation-1; i < population.size(); i++) {
		double rand = RandomValue(0, 1);
		if (rand < alpha) {
			temp[i] = Swap(temp[i], (int)RandomValue(0, temp[i].Solution.size() - 1), RandomValue(0, temp[i].Solution.size() - 1));
			Tools::Evaluer(problem, temp[i]);
		}
	}

	return temp;
}

SMSSDTSolution GetIndividuProportionnelle(deque<SMSSDTSolution> population, double totalInverseTardiness) {
	if (population.empty()) {
		return NULL;
	}
	double random = RandomValue(0, 100);
	SMSSDTSolution individuChoisit = population.back();
	double tempTardiness = 0;
	for (int i = 0; i < population.size(); i++) {
		double proba = 1 / population[i].getObj();
		tempTardiness += proba * 100 / totalInverseTardiness;
		if (random < tempTardiness) {
			individuChoisit = population[i];
			random = DBL_MAX;
			break;
		}		
	}

	return individuChoisit;
}




