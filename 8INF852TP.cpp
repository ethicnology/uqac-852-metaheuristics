#include "VNS.h"

int main(int argc, char* argv[]){
	if (argc != 4) {
		cout << endl << "######## MISSING ARGUMENTS ########" << endl;
		cout << "something.exe Iteration File Algorithm" << endl;
		cout << "ex : 8INF852TP.exe 20 PROB40.txt vns" << endl;
		return 0;
	}

	if (strcmp(argv[3], "vns") == 0) {
		VNS(atoi(argv[1]), argv[2]);
	}
}


