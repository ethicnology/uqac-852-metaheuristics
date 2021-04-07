#include "AntColony.h"
#include <numeric>
#include <float.h>
#include "GeneticsUtils.h"
#include "math.h"
using namespace std;

void ShowPheromones(double** pheromone, int size) {
	cout << "\n#Pheromones : \n";
	for (int m = 0; m < size; ++m){
		for (int n = 0; n < size; ++n){
			cout << pheromone[m][n] << " ";
		}
		cout << "\n";
	}
}

void ShowDistances(std::vector<std::vector<int>> distances, int size) {
	cout << "\n#Distances : \n";
	for (int m = 0; m < size; ++m) {
		for (int n = 0; n < size; ++n) {
			cout << distances[m][n] << " ";
		}
		cout << "\n";
	}
}

void Trace(double** pheromone, SMSSDTSolution *s, double Q, double length) {
	double val = Q / length;
	if (val > 100000.0) {
		val = 100000.0;
	}
	for (int i = 0; i < s->Solution.size()-1; i++) {
		int from = s->Solution[i], to = s->Solution[i + 1];
		pheromone[from][to] = val;
		pheromone[to][from] = val;
	}
}

void Evaporate(double** pheromone, int size, double rho) {
	for (int i = 0; i < size; ++i){
		for (int j = 0; j < size; ++j){
			int val = (1.0 - rho) * pheromone[i][j];
			if (val < 0.01){
				val = 0.01;
			}
			pheromone[i][j] = val;
		}
	}
}

void Init(double** pheromone, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			pheromone[i][j] = 10;
		}
	}
}

vector<double> ComputeProbs(vector<double> taueta) {
	vector<double> probs(taueta.size(), 0.0);
	double sum = std::accumulate(taueta.begin(), taueta.end(), 0.0);
	//cout << "PROBS SIZE " << probs.size() << " | SUM " << sum << endl;
	for (int i = 0; i < taueta.size(); i++) {
		probs[i] = taueta[i] / sum;
		//cout << "PROB  : " << probs[i] << " TAUETA "<< taueta[i]<< endl;
	}
	return probs;
}

int ProportionalSelect(vector<double> probs) {
	double R = ((double)rand() / (RAND_MAX));
	double sum = 0;
	for (int i = 0; i < probs.size(); i++) {
		sum += probs[i];
		if (R < sum) {
			return i;
		}
	}
}

double EvaluatePath(double pheromone, double alpha, double distance, double beta, int n) {
	/* Avoid divide by zero */
	if (distance == 0) distance = 1;
	double result = pow(pheromone, alpha) * pow((1.0 / distance), beta);
	if (result < 0.01) {
		result = 0.01;
	} else if(result > (DBL_MAX / (n*100.0))){
		result = DBL_MAX / (n * 100.0);
	}
	return result;
}

void AntColony(int iteration, SMSSDTProblem* problem, int shutoff, int fitness) {
	/* On-The-Fly variables */
	string algoName = "AntColony";
	double avgSol = 0, wstTime = INT_MIN, avgTime = 0, bstTime = INT_MAX;
	int bstSol = INT_MAX, wstSol = INT_MIN;

	/* Common variables*/
	clock_t	start, current;
	SMSSDTSolution* solution = NULL, alternative = NULL;
	int n = problem->getN();
	double const alpha = 3.0, beta = 2.0, rho = 0.01, Q = 2.0;

	/* Setup Matrix represents distances in Ant Colony context */
	vector<std::vector<int>> distances = problem->getS();

	/* Define pheromones as a matrix */
	int rows = n, cols = n;
	double** pheromones = new double* [rows];
	for (int i = 0; i < rows; ++i)
		pheromones[i] = new double[cols];

	for (int j = 0; j < iteration; j++) {
		/* Init clock and empty solution */
		start = clock();
		SMSSDTSolution bestSolution(problem->getN());
		vector<int>::iterator it;

		/* Pick a random solution */
		solution = new SMSSDTSolution(problem->getN(), true);
		Tools::Evaluer(problem, *solution);

		Init(pheromones, n);
		do {
			for (int i = 0; i < n-3; i++) {
				vector<double> taueta(n, 0.0);
				for (int j = i + 1; j < n - 1; j++) {
					double attractivity = pheromones[solution->Solution[i]][solution->Solution[j]];
					double visibility = distances[solution->Solution[i]][solution->Solution[j]];
					taueta[solution->Solution[j]] = EvaluatePath(attractivity, alpha, visibility, beta, n);
					//cout << "FROM " << solution->Solution[i] << " TO-> " << solution->Solution[j] << " | PHER " << attractivity << " | DIST " << visibility << " | TAUETA " << taueta[solution->Solution[j]] << endl;
				}
				vector<double> probs = ComputeProbs(taueta);
				int selected = ProportionalSelect(probs);
				it = find(solution->Solution.begin(), solution->Solution.end(), selected);
				if (it != solution->Solution.end()){
					//std::cout << "Selected " << selected << " found at position : " << it - solution->Solution.begin() << endl;
					swap(solution->Solution[j], solution->Solution[it - solution->Solution.begin()]);
				}
			}
			/* Local descent after each ant trip */
			LocalDescent(problem, solution, 100);
			Tools::Evaluer(problem, *solution);
			Evaporate(pheromones, n, rho);
			Trace(pheromones, solution, Q, solution->getObj());
			if (solution->getObj() < fitness) {
				bestSolution = *solution;
				fitness = (int)bestSolution.getObj();
			}
			current = clock();
		} while (((double(current) - double(start)) / CLOCKS_PER_SEC) < shutoff);
		StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
		showLeS(&bestSolution);
		fitness = INT_MAX;

		/* Compute results On-The-Fly */
		double currentTimer = (double)((double)clock() - (double)start) / CLOCKS_PER_SEC;
		avgSol += (double)bestSolution.getObj();
		avgTime += currentTimer;
		if (bestSolution.getObj() < bstSol) {
			bstSol = bestSolution.getObj();
		}
		if (bestSolution.getObj() > wstSol) {
			wstSol = bestSolution.getObj();
		}
		if (currentTimer < bstTime) {
			bstTime = currentTimer;
		}
		if (currentTimer > wstTime) {
			wstTime = currentTimer;
		}
	}
	avgSol = avgSol / iteration;
	avgTime = avgTime / iteration;
	Tools::ResultsToCSV(algoName, problem->getNomFichier(), wstSol, avgSol, bstSol, wstTime, avgTime, bstTime);
}
