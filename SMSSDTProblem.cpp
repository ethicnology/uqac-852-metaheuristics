#ifndef __SMSSDTSOLUTION_CPP
#define __SMSSDTSOLUTION_CPP

#include "SMSSDTProblem.h"
#include <cmath>
#include <conio.h>
using namespace std;

/**
* Constructeur
*/
SMSSDTProblem::SMSSDTProblem(char* Fichier) {
	Import(Fichier);
}

/**
* Destructeur
*/
SMSSDTProblem::~SMSSDTProblem() {
	d.clear();
	depart.clear();
	p.clear();
	for (int i = 0; i < N; i++)
		s[i].clear();
	s.clear();
}

/**
 * Import(char* FileName)
 * Importation des données allant servir à alimenter le problème
 * @param FileName : Nom du fichier dans lequel sont les données relatives au problème
 * Création (22 novembre 2007) Aymen SIoud
**/
void SMSSDTProblem::Import(char* FileName) {
	std::vector < int >  vChaine;
	ifstream fich(FileName, ios::in);
	if (!fich) {// Validation de l'ouverture du fichier.
		cerr << "Erreur lors de l'ouverture du fichier " << endl;
	}
	else
	{
		while (!fich.eof())
		{
			int  Tampon;
			fich >> Tampon;
			vChaine.push_back(Tampon);
		}
		strcpy_s(Nomfichier, FileName);
		fich.clear();
		fich.close();
		// Affectation des donnees
		// Number of job
		N = vChaine[0];
		int i;
		// Processing times
		for (i = 3; i < 3 + N; i++) {
			p.push_back(vChaine[i]);			
		}
		// Due-date times
		for (i = 3 + N; i < 3 + (2 * N); i++) {
			d.push_back(vChaine[i]);
		}
		// Depart
		for (i = 3 + (2 * N); i < 3 + (3 * N); i++) 
		{
			depart.push_back(vChaine[i]);
		}

		// Setup times
		s.resize(N);
		for (i = 0; i < (short)s.size(); i++)
			s[i].resize(N);
		i = 3 + (3 * N);
		for (unsigned short j = 0; j < N; j++) {
			for (unsigned short k = 0; k < N; k++) {
				s[j][k] = vChaine[i];
				i++;
			}
		}
	}
	vChaine.clear();

}

/**
* printing...
**/
void SMSSDTProblem::printOn(ostream& os)
{
	int i, j;
	os << "Nom du fichier  : " << Nomfichier << endl << endl;
	os << " N = " << N << endl;
	os << "*** Processing times" << endl;
	for (j = 0; j < N; j++)
	{
		os << p[j] << " ";
	}
	os << endl << endl;
	os << "*** due-dates" << endl;
	for (j = 0; j < N; j++)
	{
		os << d[j] << " ";
	}
	os << endl << endl;

	os << "*** depart" << endl;
	for (j = 0; j < N; j++)
	{
		os << depart[j] << " ";
	}
	os << endl << endl;
	os << "*** Setup times" << endl;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			os << s[i][j] << " ";
		}
		os << endl;
	}
	os << endl << endl;
}


#endif  //__SMSSDTPROBLEM_CPP