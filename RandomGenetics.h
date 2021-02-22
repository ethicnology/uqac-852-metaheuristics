#include "Tools.h"
#include "Utils.h"
void RandomGenetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness);
deque<SMSSDTSolution> InitializeRandomPopulation(SMSSDTProblem* problem, int size);