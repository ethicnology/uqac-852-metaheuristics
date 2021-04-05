#include "AntColony.h"
#include <numeric>
#include "math.h"
using namespace std;

void ShowPheromones(int** pheromone, int size) {
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

int Distance(std::vector<std::vector<int>> distance, int x, int y) {
	return distance[x][y];
}

int Smell(int** pheromone, int x, int y){
	return pheromone[x][y];
}

int Attractivity(int** pheromone, int from, int toX, int toY) {
	int X = pheromone[from][toX];
	int Y = pheromone[from][toY];
	if (X > Y) {
		return toX;
	}
	else {
		return toY;
	}
}


void Trace(int** pheromone, int from, int to) {
	pheromone[from][to] += 1;
}

void Evaporate(int** pheromone, int size) {
	for (int i = 0; i < size; ++i){
		for (int j = 0; j < size; ++j){
			pheromone[i][j] -= 1;
		}
	}
}

void Init(int** pheromone, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int x = RandomValue(0, 10);
			pheromone[i][j] = x;
		}
	}
}

vector<double> ComputeProbs(vector<double> taueta) {
	vector<double> probs(taueta.size(), 0.0);
	double sum = std::accumulate(taueta.begin(), taueta.end(), 0.0);
	for (int i = 0; i < taueta.size(); i++){
		probs[i] = taueta[i] / sum;
	}
	return probs;
}

int SelectPath(vector<double> probs) {
	double R = ((double)rand() / (RAND_MAX));
	cout << "rand" << R << endl;
	double sum = 0;
	for (int i = 0; i < probs.size(); i++) {
		sum += probs[i];
		if (R < sum) {
			return i;
		}
	}
}

void Cumulative(vector<double> cumul) {

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
	double alpha = 3.0, beta = 2.0;

	/* Setup Matrix represents distances in Ant Colony context */
	vector<std::vector<int>> distances = problem->getS();

	/* Define pheromones as a matrix */
	int rows = n, cols = n;
	int** pheromones = new int* [rows];
	for (int i = 0; i < rows; ++i)
		pheromones[i] = new int[cols];

	/* Pick a random solution */
	solution = new SMSSDTSolution(problem->getN(), true);

	for (int j = 0; j < iteration; j++) {
		/* Init clock and empty solution */
		start = clock();
		SMSSDTSolution bestSolution(problem->getN());
		vector<double> taueta(n, 0);
		vector<int> trail(n, 0);
		vector<double> probs(n, 0);
		vector<double> cumul(n+1, 0);
		vector<int>::iterator it;

		Init(pheromones, n);
		ShowPheromones(pheromones, n);
		showLeS(solution);
		ShowDistances(distances, n);

		for (int i = 0; i < n-2; i++) {
			for (int j = i + 1; j < n - 1; j++) {
				double pher = pheromones[solution->Solution[i]][solution->Solution[j]];
				double dist = distances[solution->Solution[i]][solution->Solution[j]];
				cout << "path " << solution->Solution[i] << " to-> " << solution->Solution[j] << endl;
				cout << "pher " << pher << " dist " << dist << endl;
				taueta[solution->Solution[j]] = pow(pher,alpha)*pow((1 /dist),beta);
				cout << "taueta " << taueta[solution->Solution[j]] << endl;
			}
			vector<double> probs = ComputeProbs(taueta);
			int selected = SelectPath(probs);
			cout << selected << endl;
			it = find(solution->Solution.begin(), solution->Solution.end(), selected);
			if (it != solution->Solution.end()){
				std::cout << "Element " << selected << " found at position : ";
				std::cout << it - solution->Solution.begin() << " (counting from zero) \n";
				swap(solution->Solution[i + 1], solution->Solution[it - solution->Solution.begin()]);
			}
			showLeS(solution);
			//Trace(pheromone, i - 1, i);
			//Evaporate
		}

		//do {
		//	// choisir un parcourt a faire

		//	Tools::Evaluer(problem, *solution);
		//	if (solution->getObj() < fitness) {
		//		bestSolution = *solution;
		//		fitness = (int)bestSolution.getObj();
		//	}
		//	current = clock();
		//} while (((double(current) - double(start)) / CLOCKS_PER_SEC) < shutoff);
		//StopAndLog(start, clock(), bestSolution, problem->getNomFichier());
		//showLeS(&bestSolution);
		//fitness = INT_MAX;

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
