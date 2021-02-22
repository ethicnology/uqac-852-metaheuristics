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

void RandomGenetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) {
	deque<SMSSDTSolution> population;
	population = InitializeRandomPopulation(problem, 100);
	for (auto it = population.begin(); it != population.end(); it++){
		showLeS(&*it);
	}
}
