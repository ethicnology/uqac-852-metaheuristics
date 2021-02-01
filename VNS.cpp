#include "VNS.h"
using namespace std;

void Permute(SMSSDTSolution *s, int cpt) {
	int i;
	for (i = 0; i < cpt; i++){
		std::next_permutation(s->Solution.begin(), s->Solution.end());
	}
}

void StopAndLog(double Start, double Stop, SMSSDTSolution BestSolution, char* ReportFile) {
	double Elapsed;
	Elapsed = (double(Stop) - double(Start)) / CLOCKS_PER_SEC;
	Tools::WriteReportLog(Elapsed, BestSolution, ReportFile);
	BestSolution.printIt();
}

void VNS(int Iteration, char *Dataset){
	clock_t	Start, Current;
	double StopCriterion = 60, Fitness = 100000;
	SMSSDTProblem *LeProb;	
	SMSSDTSolution* Solution = NULL;

	LeProb = new SMSSDTProblem(Dataset);
	Solution = new SMSSDTSolution(LeProb->getN(), true);

	for (int j = 0; j < Iteration; j++){
		Start = clock();	
		SMSSDTSolution	BestSolution(LeProb->getN());
		do {
			Permute(Solution, 1);
			Tools::Evaluer(LeProb, *Solution);
			if (Solution->getObj() < Fitness) {
				BestSolution = *Solution;
				Fitness = BestSolution.getObj();
			}
			Current = clock();
		} while (StopCriterion != ((double(Current) - double(Start)) / CLOCKS_PER_SEC));
		StopAndLog(Start, clock(), BestSolution, LeProb->getNomFichier());
		Fitness = 100000;
	}
}