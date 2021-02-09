#include "VNS.h"
using namespace std;

void VNS(int iteration, SMSSDTProblem *problem, int shutoff, int fitness){
	int k = 1, kMax = problem->getN();
	clock_t	start, current;
	SMSSDTSolution *solution = NULL;

	solution = new SMSSDTSolution(problem->getN(), true);

	for (int j = 0; j < iteration; j++){
		start = clock();	
		SMSSDTSolution bestSolution(problem->getN());
		do {
			Permute(solution, k);
			Tools::Evaluer(problem, *solution);
			if (solution->getObj() < fitness) {
				bestSolution = *solution;
				fitness = (int)bestSolution.getObj();
				k = 0;
			}
			if (k < kMax) {
				k++;
			}
			else k = 1;
			current = clock();
		} while (((double(current) - double(start)) / CLOCKS_PER_SEC) < shutoff );
		StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
		showLeS(&bestSolution);
		fitness = INT_MAX;
	}
}
