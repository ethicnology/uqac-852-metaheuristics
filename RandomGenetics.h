#include "Tools.h"
#include "Utils.h"
#include <vector>
void RandomGenetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness);
deque<SMSSDTSolution> InitializeRandomPopulation(SMSSDTProblem* problem, int size);
double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population);
SMSSDTSolution GetIndividuProportionnelle(deque<SMSSDTSolution> population, double totalInverseTardiness);
void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> oldPopulation, deque<SMSSDTSolution>* newPopulation, SMSSDTProblem* problem);

deque<SMSSDTSolution> Mutation(deque<SMSSDTSolution> population, double alpha, SMSSDTProblem* problem, int maxPopulation);
deque<SMSSDTSolution> NextGeneration(deque<SMSSDTSolution> population, double totalInverseTardiness, int sizePopulation);

SMSSDTSolution GetBestSolution(deque<SMSSDTSolution>  population);

