#include "GeneticsUtils.h"

/* Renvoie une population générée aléatoirement */
deque<SMSSDTSolution> InitializeRandomPlebe(SMSSDTProblem* problem, int size) {
	SMSSDTSolution* lambda = NULL;
	deque<SMSSDTSolution> population(size);
	for (int i = 0; i < size; i++) {
		lambda = new SMSSDTSolution(problem->getN(), true);
		Tools::Evaluer(problem, *lambda);
		population[i] = *lambda;
	}
	return population;
}

/* Renvoie une population dont une moitié est générée aléatoirement, l'autre à subit une descente locale */
deque<SMSSDTSolution> InitializeSemiRandomPlebe(SMSSDTProblem *problem, int size, int shutoff) {
	deque<SMSSDTSolution> population;
	population = InitializeRandomPlebe(problem, size);
	/*SortPopulation(&population);*/
	for (int i = 0; i < population.size()/2; i++) {
		LocalDescent(problem, &population[i], shutoff);
	}
	return population;
}
 
/* Les enfants subissent une descente locale et on renvoie le résultat
   Les enfants sont définis grâce à la taille de la population sans enfant*/
deque<SMSSDTSolution> MemeticsPlebe(deque<SMSSDTSolution> population, double alphaMem, SMSSDTProblem* problem, int populationNoChildren, int shutoff) {
	deque<SMSSDTSolution> temp = population;
	for (int i = populationNoChildren-1; i < population.size(); i++) {
		double rand = RandomValue(0, 1);
		if (rand < alphaMem) {
			LocalDescent(problem, &temp[i], shutoff);
		}
	}

	return temp;
}

/* La solution subit une descente locale 
   shutoff est le critère d'arrêt de la descente locale (répétition sans amélioration de la solution) */
void LocalDescent(SMSSDTProblem* problem, SMSSDTSolution* solution, int shutoff) {
	int fitness = INT_MAX;
	SMSSDTSolution	s0 = *solution;
	int stuck = 0;
	do {
		s0 = Shaking(problem, *solution, 0);
		Tools::Evaluer(problem, s0);
		if (s0.getObj() < solution->getObj()) {
			*solution = s0;
			fitness = solution->getObj();
			stuck = 0;
		}
		stuck++;
	} while (stuck <= shutoff);
}


/* Renvoie le meilleur inidividu d'une solution */
SMSSDTSolution GetBestSolution(deque<SMSSDTSolution>  population) {
	SMSSDTSolution bestSolution = population[0];
	for (int i = 1; i < population.size(); i++) {
		if (bestSolution.getObj() > population[i].getObj()) {
			bestSolution = population[i];
		}
	}

	return bestSolution;
}

/* Renvoi un individu choisi sa fonction objective
   Le choix est aléatoire mais un individu proposant une bonne solution à plus de chance d'être choisi */
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

/* Renvoie la selection de la nouvelle génération */
deque<SMSSDTSolution> NextGeneration(deque<SMSSDTSolution> population, double totalInverseTardiness, int sizePopulation) {
	deque<SMSSDTSolution> newPopulation;
	for (int i = 0; i < sizePopulation; i++) {
		newPopulation.push_back(GetIndividuProportionnelle(population, totalInverseTardiness));
	}

	return newPopulation;
}

/* Applique une mutation selon le paramètre alpha sur un enfant
   Dans notre cas la mutation est un swap de deux taches */
deque<SMSSDTSolution> Mutation(deque<SMSSDTSolution> population, double alpha, SMSSDTProblem* problem, int populationNoChildren) {
	deque<SMSSDTSolution> temp = population;
	for (int i = populationNoChildren - 1; i < population.size(); i++) {
		double rand = RandomValue(0, 1);
		if (rand < alpha) {
			temp[i] = Swap(temp[i], (int)RandomValue(0, temp[i].Solution.size() - 1), RandomValue(0, temp[i].Solution.size() - 1));
			Tools::Evaluer(problem, temp[i]);
		}
	}

	return temp;
}

/* renvoie l'inverse de lu retard total de la population (somme du de l'inverse du retard de chacun des individus)
   On utilise l'inverse car on cherche à minimiser notre fonction objective */
double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population) {
	double totalTardiness = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].getObj() == 0) {
			totalTardiness = -1;
			break;
		}
		totalTardiness += 1 / population[i].getObj();
	}

	return totalTardiness;
}

/* Permet le croisement entre deux parent et permet la création de 2 enfants */
void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> oldPopulation, deque<SMSSDTSolution>* newPopulation, SMSSDTProblem* problem){
	int minCrossover = 3;
	int maxCrossover = firstParent.Solution.size() - 3;
	int randCrossoverPoint = (int)RandomValue(minCrossover, maxCrossover);

	SMSSDTSolution firstChild = firstParent;
	SMSSDTSolution secondChild = secondParent;

	int i = 0;
	int j = 0;
	int k = 0;
	do {
		if (j == firstParent.Solution.size()) {
			return;
		}
		else {
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

	newPopulation->push_back(firstChild);
	newPopulation->push_back(secondChild);
}

/* Permet la comparaison de deux individus */
int CompareIndividuals(SMSSDTSolution s0, SMSSDTSolution s1) {
	return s0.getObj() < s1.getObj();
}

/* permet de trier la population */
void SortPopulation(deque<SMSSDTSolution>* population) {
	std::sort(population->begin(), population->end(), CompareIndividuals);
}

/* Permet l'affichage de la population */
void ShowPlebe(deque<SMSSDTSolution>* population) {
	for (auto it = population->begin(); it != population->end(); ++it) {
		showLeS(&*it);
	}
}