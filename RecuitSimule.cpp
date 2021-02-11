#include "RecuitSimule.h"

void RecuitSimule(int iteration, SMSSDTProblem* leProb, int critereArret, int fitness)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true), shaked = NULL;

	double Tinitial = 100.0f;
	double Tk = Tinitial;
	int k = 1;
	double condition = 1;

	for (int j = 0; j < iteration; j++) {
		Start = clock();
		Current = clock();
		SMSSDTSolution	BestSolution(leProb->getN());

		do {
			shaked = Shaking(leProb, *Solution, 0);
			Tools::Evaluer(leProb, shaked);
			double  r = (((double)rand() / ((double)RAND_MAX + 1.0)) * (1 - 0));

			if (r < p(Tk, (double) fitness, (double) shaked.getObj())) {
				BestSolution = shaked;
				fitness = BestSolution.getObj();
			}

			if (k >= 5000 && k >= condition) {
				Tk = g(Tk);
				condition = k * 1.05;
			}
			k++;
			
			
			Current = clock();
		} while (Tk >= 0.0f && (((double(Current) - double(Start)) / CLOCKS_PER_SEC) < critereArret));

		StopAndLog(Start, clock(), BestSolution, leProb->getNomFichier());
		showLeS(&BestSolution);
		fitness = INT_MAX;
	}
}

double p(double Tk, double solutionCourante, double solutionVoisine)
{
	double nominateur = solutionCourante - solutionVoisine;
	double denominateur = Tk;
	return exp(nominateur/denominateur);
}

double g(double Tk)
{
	double alpha = 0.95;
	if (Tk > 15) {
		alpha = 0.95;
	}
	else {
		alpha = 0.70;
	}
	
	return Tk * alpha;
}


