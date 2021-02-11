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
	}
}