#include "DescenteLocale.h"

void Descentelocale(int iteration, SMSSDTProblem* leProb, int critereArret, int fitness)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true);

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
	}
}
