#include <math.h>
#include "soliton.h"

IdealSoliton::IdealSoliton() {
	K = 1000;
}

IdealSoliton::IdealSoliton(int _K) {
	K = _K;
}

double IdealSoliton::get(int degree) {
	if(degree > 1) {
		return 1. / (degree * (degree - 1));
	}
	else {
		return 1. / K;
	}
}

RobustSoliton::RobustSoliton(double _c, double _delta, int _K) {
	// save parameters
	c = _c;
	delta = _delta;
	K = _K;

	// generate related Ideal Soliton distribution
	rho = IdealSoliton(_K);

	// compute relevant quantities, beta and R) and store them
	R = c * log(K / delta) * sqrt(K);
	for(int d=1; d<=K; d++) {
		beta += rho.get(d) + get_tau(d);
	}
}

double RobustSoliton::get(int degree){
	return (rho.get(degree) + get_tau(degree)) / beta;
}

double RobustSoliton::get_tau(int degree) {
	if (degree >= 1 && degree < K/R) {
		return R / (degree * K);
	}
	else if (degree == K/R) {
		return R * log(R / delta) / K;
	}
	else {
		return 0;
	}
}
