

#include <iostream>
#include "Tools.h"
using namespace std;


int main(int argc, char* argv[]) 
{	
	clock_t	Start, End;	//Déclaration de variable afin de calculer le temps écoulé
	double Elapsed = 0;	//Variable servant à calculer le temps écoulé (Différence entre End et Start
	double	dTheBestFitness = 100000;	//Fitness de la meilleure solution
	SMSSDTProblem* LeProb;	//Déclaration d'un problème	
	LeProb = new SMSSDTProblem(argv[2]);	//Lecture du deuxi;eme paramètre à partir de la console
	//LeProb->printOn(cout);	// Imprimer le Problème
	SMSSDTSolution* pSolution = NULL;	//Solution intermédiaire


	// argv[1] exécutions de la génération aléatoire
	for (int j = 0; j < atoi(argv[1]); j++)
	{
		Start = clock();	//Démarrer l'horloge	
		SMSSDTSolution	Smeilleur(LeProb->getN());	//Sauvegarde de la meilleure solution
		for (int i = 0; i < 1000; i++)
		{
			pSolution = new SMSSDTSolution(LeProb->getN(), true);	//Une solution aléatoire
			Tools::Evaluer(LeProb, *pSolution);	//Évaluer la solution
			if (pSolution->getObj() < dTheBestFitness) // Si améliore meilleure solution, la garder
			{
				Smeilleur = *pSolution;
				dTheBestFitness = Smeilleur.getObj();
			}
			delete pSolution;
		}
		End = clock(); // Arrêter le clock
		Elapsed = (double(End - Start)) / CLOCKS_PER_SEC;	//Calculer le temps écoulé
		Tools::WriteReportLog(Elapsed, Smeilleur, LeProb->getNomFichier());	//Logguer le temps et la meilleure solution
		dTheBestFitness = 100000;

	}
	

	return 0;

}


