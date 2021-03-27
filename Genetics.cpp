#include "Genetics.h"

void Genetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness, string typePopulation) 
{
	/* Variables pour l'algorithme */
	clock_t	start;				//Temps d'éxécutions d'une instance
	

	deque<SMSSDTSolution> population;	//Notre population
	SMSSDTSolution firstParent = NULL;	//Inidividu parent
	SMSSDTSolution secondParent = NULL; //Inidividu parent
	SMSSDTSolution bestSolution = NULL; //Meilleur inidividu d'une population

	int populationSize = 50;				//Taille de la population sur laquelle nous travaillons	
	int nombreDeGeneration = 30;			//Nombre de génération pour lequel notre algorithme va s'exécuter (CRITERE D'ARRÊT)
	int generation = 0;						//Génération initiale

	int numberChildren = 2*populationSize; //Nombre d'enfant à chaque génération
	double alpha = 0.1;					   //Probabilité qu'un enfant subisse une mutation
	double alphaMem = 1.0;			       //Probabilité de faire une descente locale sur un enfant dans algo mémétique
		
	
	double moyenneSolution = 0;					   //Calcul la moyenne des fonctions objectives de toutes les itérations	
	int meilleureSolution = INT_MAX;
	int pireSolution = INT_MIN;

	double moyenneTemps = 0;			   //Calcul le temps moyen d'une exécutions de toutes les itérations3
	double meilleurTemps = INT_MAX;
	double pireTemps = INT_MIN;
	/* ================================ */

	
	

	//L'algorithme nécessite une population d'au moins 2 individus
	if (populationSize < 2) {
		std::cout << "Erreur population trop petite"<< endl;
		return;
	}

	//Une itération est une exécution complète de l'algorithme
	for (int i = 0; i < iteration; i++) {
		start = clock(); //Démarage du chronomètre

		//Selon les paramètres d'entrées la population de base est complètement aléatoire ou semi aléatoire
		if (typePopulation.compare("SemiRandom") == 0) {
			population = InitializeSemiRandomPlebe(problem, populationSize, 3);
		}
		else if (typePopulation.compare("Random") == 0) {
			population = InitializeRandomPlebe(problem, populationSize);
		}
		else if (typePopulation.compare("Memetics") == 0) {
			population = InitializeRandomPlebe(problem, populationSize);
		}
		else {
			std::cout << "Wrong values" << endl;
			return;
		}
		

		//Tant que le critère d'arrêt n'est pas satisfait
		do {
			//Récupération de l'inverse de l'ensemble des fonctions objectives de la population
			double totalInverseTardiness = GetInverseTotalTardinessPopulation(population); //Renvois -1 si un individus == 0;

			//Si -1 alors on a un individus optimal dans la population initiale, pas besoin d'exécuter l'algorithme
			if (totalInverseTardiness == -1) {
				bestSolution = GetBestSolution(population);
				break;
			}
			
			deque<SMSSDTSolution> newPopulation = population; //Stockage temporaire de la nouvelle population avant la sélection finale

			//Croisement des parents pour former des enfants, chaque couple donne 2 enfants (j +=2)
			for (int j = 0; j < numberChildren; j += 2) {				
				do { //Sélection proportionnelle des parents
					firstParent = GetIndividuProportionnelle(population, totalInverseTardiness);
					secondParent = GetIndividuProportionnelle(population, totalInverseTardiness);
				} while (firstParent.Solution == secondParent.Solution); //Si les parents sont le même individus on refait le choix

				//Croisement pour créer les enfants, stockage des enfants dans newPopulation
				Crossover(firstParent, secondParent, population, &newPopulation, problem);
			}		

			//mutation potentielle des enfants selon la probabilité alpha
			newPopulation = Mutation(newPopulation, alpha, problem, populationSize);

			//Si on exécute l'algo memetic alors les enfants subissent une descente locale
			if (typePopulation.compare("Memetics") == 0) {
				newPopulation = MemeticsPlebe(newPopulation, alphaMem, problem, populationSize, 3);
			}

			
			totalInverseTardiness = GetInverseTotalTardinessPopulation(newPopulation);
			//Si un enfant trouve un optimum (fctObj == 0) alors on arrête l'algorithme, c'est une solution optimale
			if (totalInverseTardiness == -1) { 
				population = newPopulation;
				bestSolution = GetBestSolution(population);
				break;
			}

			//selection proportionnelle de la nouvelle population
			population = NextGeneration(newPopulation, totalInverseTardiness, populationSize);
			generation++;
		} while (((double(clock()) - double(start)) / CLOCKS_PER_SEC) < shutoff);

		//Récupération du meilleur individu dans la population restante
		bestSolution = GetBestSolution(population); 

		/* Calcul des meilleurs, moyennes et pires solutions*/
		double currentTimer = (double)((double)clock() - (double)start) / CLOCKS_PER_SEC;
		moyenneSolution += (double) bestSolution.getObj();		
		moyenneTemps += currentTimer;

		if (bestSolution.getObj() < meilleureSolution) {
			meilleureSolution = bestSolution.getObj();
		}
		
		if (bestSolution.getObj() > pireSolution) {
			pireSolution = bestSolution.getObj();
		}

		if (currentTimer < meilleurTemps) {
			meilleurTemps = currentTimer;
		}

		if (currentTimer > pireTemps) {
			pireTemps = currentTimer;
		}


		showLeS(&bestSolution);
		StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
		generation = 0;
	}

	moyenneSolution = moyenneSolution / iteration;
	moyenneTemps = moyenneTemps / iteration;
	Tools::ResultsToCSV(typePopulation, problem->getNomFichier(),pireSolution,moyenneSolution,meilleureSolution,pireTemps,moyenneTemps,meilleurTemps);
}