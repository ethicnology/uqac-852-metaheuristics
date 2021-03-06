#include "Tools.h"
#include "Utils.h"
#include "GeneticsUtils.h"
void Genetics(int iteration, SMSSDTProblem* problem, int shutoff, int fitness, string typePopulation);
double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population);
SMSSDTSolution GetIndividuProportionnelle(deque<SMSSDTSolution> population, double totalInverseTardiness);
void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> oldPopulation, deque<SMSSDTSolution>* newPopulation, SMSSDTProblem* problem);

deque<SMSSDTSolution> Mutation(deque<SMSSDTSolution> population, double alpha, SMSSDTProblem* problem, int maxPopulation);
deque<SMSSDTSolution> Memetique(deque<SMSSDTSolution> population, double alphaMem, SMSSDTProblem* problem, int maxPopulation, int shutoff);
deque<SMSSDTSolution> NextGeneration(deque<SMSSDTSolution> population, double totalInverseTardiness, int sizePopulation);

SMSSDTSolution GetBestSolution(deque<SMSSDTSolution>  population);

