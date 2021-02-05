#include "DescenteLocale.h"

void Descentelocale(int iteration, SMSSDTProblem* leProb, int critereArret, int fctObjectif)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true);

	for (int j = 0; j < iteration; j++){
		Start = clock();	
		SMSSDTSolution	BestSolution(leProb->getN());

		do {
			Permute(Solution, 1);
			Tools::Evaluer(leProb, *Solution);
			if (Solution->getObj() < fctObjectif) {
				BestSolution = *Solution;
				fctObjectif = BestSolution.getObj();
			}
			Current = clock();
		} while (((double(Current) - double(Start)) / CLOCKS_PER_SEC) < critereArret);

		StopAndLog(Start, clock(), BestSolution, leProb->getNomFichier());
		showLeS(&BestSolution);
		fctObjectif = INT_MAX;
	}
}
