#include "DescenteLocale.h"

void Descentelocale(int iteration, SMSSDTProblem* leProb, int critereArret, int fitness)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true);

	/* On-The-Fly variables */
	std::string algoName = "LocalDescent";
	double avgSol = 0, wstTime = INT_MIN, avgTime = 0, bstTime = INT_MAX;
	int bstSol = INT_MAX, wstSol = INT_MIN;

	for (int j = 0; j < iteration; j++){
		Start = clock();	
		SMSSDTSolution	BestSolution(leProb->getN());

		do {
			*Solution = Shaking(leProb, *Solution, 0);
			Tools::Evaluer(leProb, *Solution);
			if (Solution->getObj() < fitness) {
				BestSolution = *Solution;
				fitness = BestSolution.getObj();
			}
			Current = clock();
		} while (((double(Current) - double(Start)) / CLOCKS_PER_SEC) < critereArret);

		StopAndLog(Start, clock(), BestSolution, leProb->getNomFichier());
		showLeS(&BestSolution);
		fitness = INT_MAX;

		/* Compute results On-The-Fly */
		double currentTimer = (double)((double)clock() - (double)Start) / CLOCKS_PER_SEC;
		avgSol += (double)BestSolution.getObj();
		avgTime += currentTimer;
		if (BestSolution.getObj() < bstSol) {
			bstSol = BestSolution.getObj();
		}
		if (BestSolution.getObj() > wstSol) {
			wstSol = BestSolution.getObj();
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
	Tools::ResultsToCSV(algoName, leProb->getNomFichier(), wstSol, avgSol, bstSol, wstTime, avgTime, bstTime);
}
