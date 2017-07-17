#include <math.h>
#include "soliton.h"

IdealSoliton::IdealSoliton() : Distribution() {
	K = 1000;
}

IdealSoliton::IdealSoliton(int K) {
	this->K = K;
}

double IdealSoliton::get(int degree) {
	if (degree == 1) {
		return 1. / K;
	}
	if (degree > 1 && degree <=K) {
		return 1. / (degree * (degree - 1));
	}
	return 0;
}

RobustSoliton::RobustSoliton(double c, double delta, int K) : Distribution() {
	// save parameters
	this->c = c;
	this->delta = delta;
	this->K = K;

	// generate related Ideal Soliton distribution
	rho = IdealSoliton(K);

	// compute relevant quantities, beta and R) and store them
	R = c * log(K / delta) * sqrt(K);
	beta = 0;
	for(int d=1; d<=K; d++) {
		beta += rho.get(d) + get_tau(d);
	}
}

double RobustSoliton::get(int degree){
	if (degree >= 1 && degree <= K ) {
		return (rho.get(degree) + get_tau(degree)) / beta;
	}
	return 0;
}

double RobustSoliton::get_tau(int degree) {
	if (degree >= 1 && degree <= K/R - 1) {
		return R / (degree * K);
	}
	if (degree == K/R) {
		return R * log(R / delta) / K;
	}
	return 0;
}
