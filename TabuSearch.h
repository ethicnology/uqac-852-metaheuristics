#include "Tools.h"
#include "Utils.h"
void TabuSearch(int iteration, SMSSDTProblem* problem, int shutoff, int fitness);
int checkTabus(deque<vector<int>> tabuList, vector<int> neighbour);