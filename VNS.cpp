#include "VNS.h"
using namespace std;

void VNS(int Iteration, char *Dataset){
	clock_t	Start, Current;
	int StopCriterion = 30, Fitness = 100000, k, kMax = 15;
	SMSSDTProblem *LeProb;	
	SMSSDTSolution *Solution = NULL;

	LeProb = new SMSSDTProblem(Dataset);
	Solution = new SMSSDTSolution(LeProb->getN(), true);
	k = 1;

	for (int j = 0; j < Iteration; j++){
		Start = clock();	
		SMSSDTSolution	BestSolution(LeProb->getN());
		do {
			Permute(Solution, k);
			Tools::Evaluer(LeProb, *Solution);
			if (Solution->getObj() < Fitness) {
				BestSolution = *Solution;
				Fitness = BestSolution.getObj();
				k = 0;
			}
			if (k < kMax) {
				k++;
			}
			else k = 1;
			Current = clock();
		} while (((double(Current) - double(Start)) / CLOCKS_PER_SEC) < StopCriterion );
		StopAndLog(Start, clock(), BestSolution, LeProb->getNomFichier());
		showLeS(&BestSolution);
		Fitness = 100000;
	}
}
