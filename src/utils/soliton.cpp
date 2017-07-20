#include <math.h>
#include <random>
#include "soliton.h"
#include <iostream>

using namespace std;

Distribution::Distribution(int seed) {
	// set random number generator
	this->rng.seed(seed);
}

Distribution::Distribution(vector<double> probabilities, int seed) {
	// TODO check probabilities
	this->probabilities = probabilities;

	// set random number generator
	this->rng.seed(seed);
}

double Distribution::probability(int degree) {
	if (degree >= 1 && degree <= (int) this->probabilities.size()) {
		return this->probabilities[degree - 1];
	}
	return 0;
}

double Distribution::expectation() {
	double E = 0;
	for(unsigned int degree = 1; degree <= this->probabilities.size(); degree++) {
		E += degree * this->probability(degree);
	}
	return E;
}

int Distribution::realization() {
	// divide (0, 1) into pieces distributed as probabilities,
	// then pick uniformly an interval, with choice variable
	std::uniform_real_distribution<double> x(0, 1);
	double choice = x(this->rng);

	double sum = 0;

	for(unsigned int degree = 1; degree <= this->probabilities.size(); degree++) {
		// get to the next step
		sum += this->probability(degree);

		// if in current degree interval, return
		if (choice < sum) {
			return degree;
		}
	}
	// unreachable code (in theory)
	return -1;
}

IdealSoliton::IdealSoliton(int K, int seed) : Distribution(seed) {
	// initialize probabilities vector

	// set capacity of probabilities to K
	this->probabilities.reserve(K);

	for (int degree = 1; degree <= K; degree++) {
		// compute probability of current degree
		double prob;
		if (degree == 1) {
			prob = 1. / K;
		}
		else if (degree > 1 && degree <= K) {
			prob = 1. / (degree * (degree - 1));
		}
		else {
			prob = 0;
		}

		// put prob into the array, at the end
		this->probabilities.push_back(prob);
	}
}

RobustSoliton::RobustSoliton(
	double c,
	double delta,
	int K,
	int seed) : Distribution(seed) {
	// save parameters into object variable for debug purpose
	this->c = c;
	this->delta = delta;
	this->K = K;

	// generate related Ideal Soliton distribution
	IdealSoliton rho = IdealSoliton(K, seed);

	// compute relevant quantities (beta and R), needed later
	double R = c * log(K / delta) * sqrt(K);
	double beta = 0;
	for(int d = 1; d <= K; d++) {
		beta += rho.probability(d) + get_tau(d, R);
	}

	// set capacity of probabilities to K
	this->probabilities.reserve(K);

	// initialize probabilities vector
	for (int degree = 1; degree <= K; degree++) {
		// compute probability of current degree
		double prob;
		if (degree >= 1 && degree <= K ) {
			prob = (rho.probability(degree) + this->get_tau(degree, R)) / beta;
		}
		else {
			prob = 0;
		}

		// put it into the array (which is 0-indexed)
		this->probabilities.push_back(prob);
	}
}

double RobustSoliton::get_tau(int degree, double R) {
	int K = this->probabilities.size();

	if (degree >= 1 && degree <= K/R - 1) {
		return R / (degree * K);
	}
	if (degree == K/R) {
		return R * log(R / delta) / K;
	}
	return 0;
}

OverheadRobustSoliton::OverheadRobustSoliton(
	vector<double> x,
	double c,
	double delta,
	int K,
	int seed) : RobustSoliton(c, delta, K, seed) {

	if (x.size() != (unsigned int) K) {
		throw invalid_argument( "x (overheads vector) must be of lenght K" );
	}
	this->x = x;
}

int OverheadRobustSoliton::realization() {
	int realization = RobustSoliton::realization();
	// add overhead, round to nearest integer
	return (int) ( realization * this->x[realization - 1] );
}

double OverheadRobustSoliton::expectation() {
	double E = 0;
	for(unsigned int degree = 1; degree <= this->probabilities.size(); degree++) {
		E += degree * RobustSoliton::probability(degree) * this->x[degree - 1];
	}
	return E;
}

double OverheadRobustSoliton::probability(int degree) {
	std::cerr
		<< "WARNING: probability of OverheadRobustSoliton has un-overheaded degree as its input\n";
	return RobustSoliton::probability(degree);
}
