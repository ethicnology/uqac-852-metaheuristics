#include "VNS.h"
#include "TabuSearch.h"
#include "DescenteLocale.h"
#include "RecuitSimule.h"
#include "Genetics.h"
#include "GeneticsUtils.h"

int main(int argc, char* argv[]){
	if (argc != 4) {
		cout << endl << "######## MISSING ARGUMENTS ########" << endl;
		cout << "something.exe Iteration File Algorithm" << endl;
		cout << "ex : 8INF852TP.exe 20 PROB40.txt VNS" << endl;
		return 0;
	}
	else {
		srand((int)time(NULL));
	}

	int iteration = atoi(argv[1]), shutoff = 15, fitness = INT_MAX;
	char *dataset = argv[2], *algorithm = argv[3];

	SMSSDTProblem* problem;
	problem = new SMSSDTProblem(dataset);

	if (strcmp(algorithm, "VNS") == 0) {
		VNS(iteration, problem, shutoff, fitness);
	} 
	else if (strcmp(algorithm, "TabuSearch") == 0) {
		TabuSearch(iteration, problem, shutoff, fitness);
	}
	else if (strcmp(algorithm, "SimulatedAnnealing") == 0) {
		RecuitSimule(iteration, problem, shutoff, fitness);
	}	
	else if (strcmp(algorithm, "LocalDescent") == 0) {
		Descentelocale(iteration, problem, shutoff, fitness);
	}
	else if (strcmp(algorithm, "RandomGenetics") == 0) {
		Genetics(iteration, problem, shutoff, fitness, "random");
	} 
	else if (strcmp(algorithm, "SemiRandomGenetics") == 0) {
		Genetics(iteration, problem, shutoff, fitness, "semiRandom");
	} 
	else if (strcmp(algorithm, "Memetique") == 0) {
		Genetics(iteration, problem, shutoff, fitness, "memetique");
	}
	else if (strcmp(algorithm, "TestUtils") == 0) {
		TestUtils(iteration, problem, shutoff, fitness);
	}
	else {
		cout << "Unknown algorithm" << endl;
	}
}


