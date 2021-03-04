#include "RandomGenetics.h"

deque<SMSSDTSolution> InitializeRandomPopulation(SMSSDTProblem* problem, int size) {
	SMSSDTSolution* lambda = NULL;
	deque<SMSSDTSolution> population;
	//population.resize(size);
	for (int i = 0; i < size; i++) {
		lambda = new SMSSDTSolution(problem->getN(), true);
		Tools::Evaluer(problem, *lambda);
		population.push_back(*lambda);
	}
	return population;
}

void RandomGenetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) 
{
	int populationMax = 20;

	deque<SMSSDTSolution> population;
	SMSSDTSolution firstParent = NULL;
	SMSSDTSolution secondParent = NULL;
	SMSSDTSolution bestSolution = NULL;
	double alpha = 0.1;
	int nombreDeGeneration = 30;
	int generation = 0;


	if (populationMax < 2) {
		cout << "Erreur population trop petite";
		exit;
	}

	population = InitializeRandomPopulation(problem, populationMax);

	cout << "==== Meilleur individu avant algo genetique ====" << endl;
	bestSolution = GetBestSolution(population);
	showLeS(&bestSolution);

	for (int i = 0; i < iteration; i++) {

		while (generation < nombreDeGeneration) {
			double totalInverseTardiness = GetInverseTotalTardinessPopulation(population);

			//Sélection des parents
			do {
				firstParent = GetIndividuProportionnelle(population, totalInverseTardiness);
				secondParent = GetIndividuProportionnelle(population, totalInverseTardiness);
			} while (firstParent.Solution == secondParent.Solution);

			//Croisement pour créer les enfants
			Crossover(firstParent, secondParent, &population, problem);

			//mutation potentielle des enfants
			Mutation(&population[population.size() - 1], alpha, problem);
			Mutation(&population[population.size() - 2], alpha, problem);

			//selection nouvelle population
			totalInverseTardiness = GetInverseTotalTardinessPopulation(population);
			population = NextGeneration(population, totalInverseTardiness, populationMax);
			generation++;
			cout << "generation  : " << generation << endl;
		}
		
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

void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> *population, SMSSDTProblem* problem) 
{
	int minCrossover = 3;
	int maxCrossover = firstParent.Solution.size() - 3;
	int randCrossoverPoint = (int) RandomValue(minCrossover, maxCrossover);

	/*cout << "firstParent.Solution.size() " << firstParent.Solution.size() << endl;
	cout << "CrossOverPoint " << randCrossoverPoint << endl;*/

	SMSSDTSolution firstChild = firstParent;
	SMSSDTSolution secondChild = secondParent;

	int i = 0;
	int j = 0;
	int k = 0;
	do {
		if (firstChild.notIn(secondParent.Solution[j], randCrossoverPoint)) {
			firstChild.Solution[i] = secondParent.Solution[j];
			i++;
		}

		if (secondChild.notIn(firstParent.Solution[j], randCrossoverPoint)) {
			secondChild.Solution[k] = firstParent.Solution[j];
			k++;
		}

		j++;
	} while (j < secondChild.Solution.size() || (i < randCrossoverPoint && k < randCrossoverPoint));
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

	population->push_back(firstChild);
	population->push_back(secondChild);
}



double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population) {
	double totalTardiness = 0;
	for (int i = 0; i < population.size(); i++) {
		totalTardiness += 1/population[i].getObj();
	}

	return totalTardiness;
}

void Mutation(SMSSDTSolution* individu, double alpha, SMSSDTProblem* problem) {
	double rand = RandomValue(0, 1);
	//showLeS(individu);
	if (rand < alpha) {
		*individu = Swap(*individu, (int)RandomValue(0, individu->Solution.size() - 1), RandomValue(0, individu->Solution.size() - 1));
		Tools::Evaluer(problem, *individu);
	}
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




