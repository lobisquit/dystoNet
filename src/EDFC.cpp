#include <math.h>
#include "binomial.h"

double getp(int d, double xd, int K){
	return 1-exp(-xd*d/K);
}

double getViolationProb(int d, double xd, int K){
	double prob = 1 - binomial_CDF(K, d-1, getp(d,xd,K));
	return prob;
}
