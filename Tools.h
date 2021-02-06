#pragma once
#ifndef __TOOLS_H
#define __TOOLS_H

#include <stdio.h>
#include <direct.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <wchar.h>
#include <cstring> 
#include <conio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <sstream>
#include "SMSSDTProblem.h"
#include "SMSSDTSolution.h"



using namespace std;


/**
 * classe comportant des méthodes communes pouvant être utilisé dans plusieurs algorithmes
 **/
class Tools
{
	private :

		//Méthode servant à calculer les completion time CI des jobs
		static vector < int > completionTime(SMSSDTProblem* LeProb, SMSSDTSolution& Sol) {
			int N = LeProb->getN();
			vector < int > C(N, 0);
			vector < int > p = LeProb->getP();
			vector < int > dep = LeProb->getDepart();
			double sum = 0;
			vector <vector < int > > s = LeProb->getS();
			C[0] = p[Sol.Solution[0]] + dep[Sol.Solution[0]];
			Sol.ST[0] = dep[Sol.Solution[0]];
			sum = Sol.ST[0];
			for (int j = 1; j < N; j++) {
				C[j] = C[j - 1] + p[Sol.Solution[j]] + s[Sol.Solution[j - 1]][Sol.Solution[j]];
				Sol.ST[j] = s[Sol.Solution[j - 1]][Sol.Solution[j]];
				sum += Sol.ST[j];
			}
			Sol.CT = C;
			return C;
		}

		//Méthode servant à calculer Le total Tardiness de la solution
		static double tardiness(SMSSDTProblem* LeProb, SMSSDTSolution& Sol) {
			int N = (int)Sol.Solution.size();
			vector < int > d = LeProb->getD();
			// C[j] = temps de fin du job j
			vector < int > C = completionTime(LeProb, Sol);
			// Calcul de la tardiness
			double sum = 0;
			for (int j = 0; j < N; j++) {
				Sol.TT[j] = (int)std::max(0, (int)(C[j] - d[Sol.Solution[j]]));
				sum += Sol.TT[j];
			}
			return sum;
		}

	public:

			
		//Fonction servant à évaluer une solution
		static void Evaluer(SMSSDTProblem* LeProb, SMSSDTSolution& Sol) {
			Sol.setObj(0);
			double obj = tardiness(LeProb, Sol);
			Sol.setObj(obj);
		}

		//Fonction  servant à logguer
		static void WriteReportLog(double Elapsed, SMSSDTSolution BestSolution, char* ProblemName) {
			
			char DateStr[9];
			char TimeStr[9];
			_mkdir(R"(Results)");
			string FileName = "Results/Report_" + string(ProblemName) + ".log";
			fstream Streamf(FileName, ios::out | ios::app);

			_strdate_s(DateStr, sizeof(DateStr));
			_strtime_s(TimeStr, sizeof(DateStr));
			Streamf << DateStr << ", " << TimeStr;

			Streamf << "\n\t";
			BestSolution.Save(Streamf);
			Streamf << "; " << BestSolution.getObj() << " ; " << Elapsed;
			Streamf << "\n";
			Streamf.clear();
			Streamf.close();
			time_t rawtime;
			time(&rawtime);

			WriteResultsToCSV(Elapsed, BestSolution, ProblemName);
		}

		static void WriteResultsToCSV(double Elapsed, SMSSDTSolution BestSolution, char* ProblemName) {
			char Date[9];
			char Time[9];

			string FileName = "Results/Report_" + string(ProblemName) + ".csv";
			fstream Streamf(FileName, ios::out | ios::app);

			_strdate_s(Date, sizeof(Date));
			_strtime_s(Time, sizeof(Date));
			Streamf << Date << ", " << Time;

			Streamf << "\t";
			BestSolution.Save(Streamf);
			Streamf << " " << BestSolution.getObj() << " " << Elapsed;
			Streamf << "";
			Streamf.clear();
			Streamf.close();
			time_t rawtime;
			time(&rawtime);
		}

};


#endif 
