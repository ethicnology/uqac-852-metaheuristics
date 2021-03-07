#include "Tools.h"
#include "Utils.h"
deque<SMSSDTSolution> InitializeRandomPlebe(SMSSDTProblem* problem, int size);
deque<SMSSDTSolution> InitializeSemiRandomPlebe(SMSSDTProblem* problem, int size, int shutoff);
deque<SMSSDTSolution> MemeticsPlebe(deque<SMSSDTSolution> population, double alphaMem, SMSSDTProblem* problem, int populationNoChildren, int shutoff);
void LocalDescent(SMSSDTProblem* problem, SMSSDTSolution* solution, int shutoff);
SMSSDTSolution GetBestSolution(deque<SMSSDTSolution>  population);
SMSSDTSolution GetIndividuProportionnelle(deque<SMSSDTSolution> population, double totalInverseTardiness);
deque<SMSSDTSolution> NextGeneration(deque<SMSSDTSolution> population, double totalInverseTardiness, int sizePopulation);
deque<SMSSDTSolution> Mutation(deque<SMSSDTSolution> population, double alpha, SMSSDTProblem* problem, int populationNoChildren);
double GetInverseTotalTardinessPopulation(deque<SMSSDTSolution> population);
void Crossover(SMSSDTSolution firstParent, SMSSDTSolution secondParent, deque<SMSSDTSolution> oldPopulation, deque<SMSSDTSolution>* newPopulation, SMSSDTProblem* problem);
int CompareIndividuals(SMSSDTSolution s0, SMSSDTSolution s1);
void SortPopulation(deque<SMSSDTSolution>* population);
void ShowPlebe(deque<SMSSDTSolution>* population);