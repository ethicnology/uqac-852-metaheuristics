#include "GeneticsUtils.h"

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

deque<SMSSDTSolution> InitializeSemiRandomPlebe(SMSSDTProblem *problem, int size) {
	deque<SMSSDTSolution> population;
	population = InitializeRandomPlebe(problem, size);
	/*SortPopulation(&population);*/
	for (int i = 0; i < population.size()/2; i++) {
		LocalDescent(problem, &population[i], 5);
	}
	return population;
}

void ShowPlebe(deque<SMSSDTSolution>* population) {
	for (auto it = population->begin(); it != population->end(); ++it) {
		showLeS(&*it);
	}
}

int CompareIndividuals(SMSSDTSolution s0, SMSSDTSolution s1) {
	return s0.getObj() < s1.getObj();
}

void SortPopulation(deque<SMSSDTSolution> *population) {
	std::sort(population->begin(), population->end(), CompareIndividuals);
}

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


void TestUtils(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) {
	deque<SMSSDTSolution> population;
	population = InitializeSemiRandomPlebe(problem, 10);
	ShowPlebe(&population);
}