#include <stdio.h>
#include <math.h>

const int N = 2000; // Number of nodes
const int K = 1000; // Number of sensing nodes
const double deltad = 0.05; // Violation probability

// Constants for Robust Soliton Distribution
const double c = 0.01;
const double delta = 0.05;
const double R = c*log(K/delta)*sqrt(K);

double getmu(double R, int d);asdasdasd
double getro(int d);
double gettau(double R, int d);

main() {
	int d = 1; // Node-degree
	double mu = getmu(R, d);
	printf("%f\n", mu);
	return 0;
}

/**
* Get Ideal Soliton distribution
* @param d node degree
* @return Robust Soliton probabilit
*/
double getro(int d){
	double ro;

	if(d > 1) {
		ro = 1./(d*(d-1));
	}
	else {
		ro = 1./K;
	}
	return ro;
}

/**
* Tau weighting function for Robust Soliton
* @param R Robust soliton parameter
* @param d node degree
* @return Robust Soliton probability for d
*/
double gettau(double R, int d){
	double tau;

	if(d>=1 && d<K/R) {
		tau = R/(d*K);
	}
	else if(d == K/R) {
		tau = R*log(R/delta)/K;
	}
	else {
		tau = 0;
	}
	return tau;
}

/**
* Compute Robust Soliton Distribution for a node-degree d
* @param R Robust soliton parameter
* @param d node degree
* @return Robust Soliton probability for d
*/
double getmu(double R, int d){
	double ro;
	double tau;
	double beta = 0;
	double mu;

	// compute ro
	ro = getro(d);

	// compute tau
	tau = gettau(R, d);

	// compute beta
	for(int i=1; i<=K; i++) {
		beta += getro(i) + gettau(R, i);
	}

	// compute mu
	mu = (ro+tau)/beta;
	return mu;
}
