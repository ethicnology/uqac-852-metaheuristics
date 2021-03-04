#include "RandomGenetics.h"

void RandomGenetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) 
{
	clock_t	start, current;
	int populationMax = 50;

	deque<SMSSDTSolution> population;
	SMSSDTSolution firstParent = NULL;
	SMSSDTSolution secondParent = NULL;
	SMSSDTSolution bestSolution = NULL;
	double alpha = 0.1;
	int nombreDeGeneration = 50;
	int generation = 0;

	int numberChildren = 2*populationMax;


	if (populationMax < 2) {
		cout << "Erreur population trop petite";
		return;
	}

	population = InitializeRandomPlebe(problem, populationMax);

	cout << "==== Meilleur individu avant algo genetique ====" << endl;
	bestSolution = GetBestSolution(population);
	showLeS(&bestSolution);

	for (int i = 0; i < iteration; i++) {
		start = clock();
		while (generation < nombreDeGeneration) {
			double totalInverseTardiness = GetInverseTotalTardinessPopulation(population);
			deque<SMSSDTSolution> newPopulation = population;

			for (int j = 0; j < numberChildren; j += 2) {
				//Sélection des parents
				do {
					firstParent = GetIndividuProportionnelle(population, totalInverseTardiness);
					secondParent = GetIndividuProportionnelle(population, totalInverseTardiness);
				} while (firstParent.Solution == secondParent.Solution);

				//Croisement pour créer les enfants
				Crossover(firstParent, secondParent, population, &newPopulation, problem);
			}		

			//mutation potentielle des enfants
			newPopulation = Mutation(newPopulation, alpha, problem, populationMax);

			//selection nouvelle population
			totalInverseTardiness = GetInverseTotalTardinessPopulation(newPopulation);
			population = NextGeneration(newPopulation, totalInverseTardiness, populationMax);
			generation++;
			cout << "generation  : " << generation << endl;

			/*cout << "================================================" << endl;
			cout << "==== Meilleur individu GENERATION " << generation << " ====" << endl;
			bestSolution = GetBestSolution(population);
			showLeS(&bestSolution);*/
		}
		StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
	}
	cout << "================================================" << endl;
	cout << "==== Meilleur individu APRES algo genetique ====" << endl;
	bestSolution = GetBestSolution(population);
	showLeS(&bestSolution);
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




