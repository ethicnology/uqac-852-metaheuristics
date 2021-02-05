#include "RecuitSimule.h"

void RecuitSimule(int iteration, SMSSDTProblem* leProb, int critereArret, int fctObjectif)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true);

	double Tinitial = 500.0f;
	double Tk = Tinitial;
	int k = 1;

	for (int j = 0; j < iteration; j++) {
		Start = clock();
		SMSSDTSolution	BestSolution(leProb->getN());

		do {
			Permute(Solution, k);
			Tools::Evaluer(leProb, *Solution);
			int r = rand();

			if (r < p(Tk, (double) fctObjectif, (double) Solution->getObj())) {
				BestSolution = *Solution;
				fctObjectif = BestSolution.getObj();
			}

			Tk = g(Tk);
			k++;
			Current = clock();
		} while (Tk >= 0.0f && (((double(Current) - double(Start)) / CLOCKS_PER_SEC) < critereArret));

		StopAndLog(Start, clock(), BestSolution, leProb->getNomFichier());
		showLeS(&BestSolution);
		fctObjectif = INT_MAX;
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
	double alpha = 0.975;
	return Tk * alpha;
}


