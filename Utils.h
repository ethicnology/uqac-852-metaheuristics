#include "Tools.h"
void Permute(SMSSDTSolution *s, int cpt);
void Swap(SMSSDTSolution *s, int pos1, int pos2);
void Inversion(SMSSDTSolution *s, int pos1, int pos2);
void StopAndLog(double Start, double Stop, SMSSDTSolution BestSolution, char *ReportFile);
void showLeS(SMSSDTSolution *s);