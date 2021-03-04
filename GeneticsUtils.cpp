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
	SortPopulation(&population);
	for (int i = population.size() / 2; i < population.size(); i++) {
		LocalDescent(problem, &population[i], 1);
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
	clock_t	start, current;
	SMSSDTSolution	s0 = *solution;
	start = clock();
	do {
		s0 = Shaking(problem, *solution, 0);
		Tools::Evaluer(problem, s0);
		if (s0.getObj() < solution->getObj()) {
			*solution = s0;
			fitness = solution->getObj();
		}
		current = clock();
	} while (((double(current) - double(start)) / CLOCKS_PER_SEC) < shutoff);
}


void TestUtils(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) {
	deque<SMSSDTSolution> population;
	population = InitializeSemiRandomPlebe(problem, 10);
	ShowPlebe(&population);
}