#include "TabuSearch.h"
using namespace std;

int checkTabus(deque<vector<int>> tabuList, vector<int> neighbour) {
	for (int i = (int)tabuList.size() - 1; i >= 0; i--) {
		if (tabuList[i] == neighbour) return 1;
	}
	return 0;
}

void TabuSearch(int iteration, SMSSDTProblem *problem, int shutoff, int fitness) {
	int tabuSizeMax = problem->getN();
	clock_t	start, current;
	SMSSDTSolution* solution = NULL, shaked = NULL;
	deque<vector<int>> tabuList;

	tabuList.resize(tabuSizeMax);
	solution = new SMSSDTSolution(problem->getN(), true);

	/* On-The-Fly variables */
	std::string algoName = "TabuSearch";
	double avgSol = 0, wstTime = DBL_MIN, avgTime = 0, bstTime = DBL_MAX;	
	int bstSol = INT_MAX, wstSol = INT_MIN;

	for (int j = 0; j < iteration; j++) {
		start = clock();
		SMSSDTSolution bestSolution(problem->getN());
		do {
			shaked = Shaking(problem, *solution, 0);
			if (checkTabus(tabuList, shaked.Solution) == 0) {
				*solution = shaked;
				Tools::Evaluer(problem, *solution);
				if (tabuList.size() == tabuSizeMax) {
					tabuList.pop_front();
				}
				tabuList.push_back(vector<int>(solution->Solution));
				if (solution->getObj() < fitness) {
					bestSolution = *solution;
					fitness = (int)bestSolution.getObj();
				}
			}
			current = clock();
		} while (((double(current) - double(start)) / CLOCKS_PER_SEC) < shutoff);
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