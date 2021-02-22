#pragma once
#ifndef __SMSSDTSOLUTION_H
#define __SMSSDTSOLUTION_H
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

class SMSSDTSolution
{
protected:
	double FctObj;

public:

	vector<int > Solution;
	vector<int > CT;
	vector<int > TT;
	vector<int > ST;

	
	/**
	 * SMSSDTSolution()
	 * Constructeur de la classe
	**/
	SMSSDTSolution(int N) {
		Solution.resize(N);
		TT.resize(N);
		CT.resize(N);
		ST.resize(N);
		FctObj = -1;

	};

	/**
	 * SMSSDTSolution(int N)
	 * Constructeur de la classe
	 * Assigne les valeurs aléatoires initiales à la solution
	**/
	SMSSDTSolution();
	SMSSDTSolution(int N, bool test);

	
	/* Destructeur*/
	~SMSSDTSolution();


	/**
	 * Save
	 * Fonction permettant d'écrire la solution dans un flux
	 * @param Stream : Flux dans lequel on doit écrire
	**/
	ostream& Save(ostream& Stream);

	
	/**  definition des accesseurs **/

	/* Retourne la valeur de l'objectif */
	double getObj() {
		return FctObj;
	}

	
	void setObj(double obj) {
		FctObj = obj;
	}
};




#endif
