#include "Utils.h"
using namespace std;

void Permute(SMSSDTSolution* s, int cpt) {
	int i;
	for (i = 0; i < cpt; i++) {
		std::next_permutation(s->Solution.begin(), s->Solution.end());
	}
}

void StopAndLog(double Start, double Stop, SMSSDTSolution BestSolution, char* ReportFile) {
	double Elapsed;
	Elapsed = (double(Stop) - double(Start)) / CLOCKS_PER_SEC;
	Tools::WriteReportLog(Elapsed, BestSolution, ReportFile);
	BestSolution.printIt();
}