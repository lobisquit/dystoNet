#include <math.h>
#include "soliton.h"

double getrho(int K, int d){
	double rho;

	if(d > 1) {
		rho = 1. / (d * (d - 1));
	}
	else {
		rho = 1. / K;
	}
	return rho;
}

double gettau(int K, double R, int d){
	double tau;

	if(d>=1 && d<K/R) {
		tau = R / (d * K);
	}
	else if(d == K/R) {
		tau = R * log(R / delta) / K;
	}
	else {
		tau = 0;
	}
	return tau;
}

double getmu(int K, double R, int d){
	double rho;
	double tau;
	double beta = 0;
	double mu;

	// compute ro
	rho = getrho(K, d);

	// compute tau
	tau = gettau(K, R, d);

	// compute beta
	for(int i=1; i<=K; i++) {
		beta += getrho(K, i) + gettau(K, R, i);
	}

	// compute mu
	mu = (rho + tau) / beta;
	return mu;
}
