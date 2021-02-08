#include "RecuitSimule.h"

void RecuitSimule(int iteration, SMSSDTProblem* leProb, int critereArret, int fctObjectif)
{
	clock_t	Start, Current;
	SMSSDTSolution* Solution = new SMSSDTSolution(leProb->getN(), true);

	double Tinitial = 50.0f;
	double Tk = Tinitial;
	int k = 1;
	double condition = 1;

	for (int j = 0; j < iteration; j++) {
		Start = clock();
		Current = clock();
		SMSSDTSolution	BestSolution(leProb->getN());

		do {
			Permute(Solution, 1);
			Tools::Evaluer(leProb, *Solution);
			int r = rand();

			if (r < p(Tk, (double) fctObjectif, (double) Solution->getObj())) {
				BestSolution = *Solution;
				fctObjectif = BestSolution.getObj();
			}

			if (k >= 20000 && k >= condition) {
				cout << "je passe dedans " << k << endl;
				cout << "température " << Tk << endl;
				cout << "fctobjectif " << fctObjectif << endl;
				cout << "============= " << endl;
				Tk = g(Tk);
				condition = k * 1.1;
			}
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
	double alpha = 0.90;
	return Tk * alpha;
}


