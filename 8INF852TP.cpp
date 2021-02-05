#include "VNS.h"
#include "DescenteLocale.h"
#include "RecuitSimule.h"

int main(int argc, char* argv[]){
	if (argc != 4) {
		cout << endl << "######## MISSING ARGUMENTS ########" << endl;
		cout << "something.exe Iteration File Algorithm" << endl;
		cout << "ex : 8INF852TP.exe 20 PROB40.txt vns" << endl;
		return 0;
	}
	else {
		srand(time(NULL));
	}

	int iteration = atoi(argv[1]);
	char* dataset = argv[2];
	char* algorithme = argv[3];
	
	clock_t	Start, Current;
	int critereArret = 30, Fitness = INT_MAX;
	SMSSDTProblem* LeProb;

	LeProb = new SMSSDTProblem(dataset);

	if (strcmp(algorithme, "vns") == 0) {
		VNS(iteration, dataset);
	} else if (strcmp(algorithme, "recuit") == 0) {
		RecuitSimule(iteration, LeProb, critereArret, Fitness);
	} else if (strcmp(algorithme, "tabou") == 0) {
		VNS(iteration, dataset);
	} else if (strcmp(algorithme, "descente") == 0) {
		Descentelocale(iteration, LeProb, critereArret, Fitness);
	} 
	else {
		cout << "Algorithme inconu" << endl;
	}
}


