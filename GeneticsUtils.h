#include "Tools.h"
#include "Utils.h"
deque<SMSSDTSolution> InitializeRandomPlebe(SMSSDTProblem* problem, int size);
deque<SMSSDTSolution> InitializeSemiRandomPlebe(SMSSDTProblem* problem, int size);
void ShowPlebe(deque<SMSSDTSolution>* population);
int CompareIndividuals(SMSSDTSolution s0, SMSSDTSolution s1);
void SortPopulation(deque<SMSSDTSolution>* population);
void LocalDescent(SMSSDTProblem* problem, SMSSDTSolution* solution, int shutoff);
void TestUtils(int iteration, SMSSDTProblem* problem, int shutoff, int fitness);