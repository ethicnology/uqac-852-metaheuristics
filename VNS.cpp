#include "VNS.h"
using namespace std;

void VNS(int iteration, SMSSDTProblem *problem, int shutoff, int fitness){
	int k = 1, kMax = 4;
	clock_t	start, current;
	SMSSDTSolution *solution = NULL;

	solution = new SMSSDTSolution(problem->getN(), true);

	/* On-The-Fly variables */
	std::string algoName = "VNS";
	double avgSol = 0, wstTime = DBL_MIN, avgTime = 0, bstTime = DBL_MAX;
	int bstSol = INT_MAX, wstSol = INT_MIN;

	for (int j = 0; j < iteration; j++){
		start = clock();	
		SMSSDTSolution bestSolution(problem->getN());
		do {
			*solution = Shaking(problem, *solution, k);
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

		/* Compute results On-The-Fly */
		double currentTimer = (double)((double)clock() - (double)start) / CLOCKS_PER_SEC;
		avgSol += (double)bestSolution.getObj();
		avgTime += currentTimer;
		if (bestSolution.getObj() < bstSol) {
			bstSol = (int)bestSolution.getObj();
		}
		if (bestSolution.getObj() > wstSol) {
			wstSol = (int)bestSolution.getObj();
		}
		if (currentTimer < bstTime) {
			bstTime = currentTimer;
		}
		if (currentTimer > wstTime) {
			wstTime = currentTimer;
		}
	}
	avgSol = avgSol / iteration;
	avgTime = avgTime / iteration;
	Tools::ResultsToCSV(algoName, problem->getNomFichier(), wstSol, avgSol, bstSol, wstTime, avgTime, bstTime);
}
