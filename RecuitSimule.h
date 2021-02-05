#include "Tools.h"
#include "Utils.h"
void RecuitSimule(int iteration, SMSSDTProblem* leProb, int critereArret, int fctObjectif);

double p(double Tk, double solutionCourante, double solutionVoisine);
double g(double Tk);