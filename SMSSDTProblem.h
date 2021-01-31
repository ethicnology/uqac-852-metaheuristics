#pragma once
#ifndef __SMSSDTPROBLEM_H
#define __SMSSDTPROBLEM_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <queue>


using namespace std;


class SMSSDTProblem {
protected:

	/** nom du fichier */
	char Nomfichier[30 + 2];

	/** nombre de travaux */
	int N;

	/** p[j] = processing times du travail j */
	std::vector < int > p;

	/** d[j] = due-date du travail j */
	std::vector < int > d;

	/** depart[j] = setup initial */
	std::vector < int > depart;

	/** s[i][j] = setup time du travail j venant après le travail i */
	std::vector < std::vector < int> > s;

public:

	/**
	* Constructeur
	*/
	SMSSDTProblem(char* Fichier);

	/**
	* Destructeur
	*/
	~SMSSDTProblem();

	/**
	 * Import(char* FileName)
	 * Importation des données allant servir à alimenter le problème
	 * @param FileName : Nom du fichier dans lequel sont les données relatives au problème
	 * Création (22 novembre 2007) Aymen SIoud
	**/
	void Import(char* FileName);

	/**
	* printOn (ostream & _os)
	* Imprime les donnes relatives au probleme
	*/
	void printOn(ostream& _os);


	/* Debut des accesseurs (fonction permettant d'acceder au variables privées de la classes*/

	/**
	* getNomFichier()
	* Retourne le nom du fichier
	*/
	char* getNomFichier()
	{
		return Nomfichier;
	}


	/**
	* getN ()
	* Retourne le nombre de travaux
	*/
	inline const int getN() {
		return N;
	}

	/**
	* getP ()
	* Retourne les temps d'execution des  travaux
	*/
	inline const std::vector < int > getP() {
		return p;
	}

	/**
	* getS ()
	* Retourne la matrice de setup
	*/
	inline const std::vector <std::vector < int > > getS() {
		return s;
	}

	/**
	* getD ()
	* Retourne les dates dues des travaux
	*/
	inline const std::vector < int > getD() {
		return d;
	}

	/**
	* getDepart ()
	* Retourne les dates dues des travaux
	*/
	inline const std::vector < int > getDepart() {
		return depart;
	}
	//fin des accesseurs	


};

#endif // Fin __SMSSDTPROBLEM_H



