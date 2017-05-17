#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "soliton.h"
#include "binomial.h"
#include "first_theoretic_bound.h"

TheoreticBound::TheoreticBound(	int _K,
								int _N,
								RobustSoliton* _robust_soliton,
								double _max_failure_probability) {
	K = _K;
	N = _N;
	robust_soliton = _robust_soliton;
	max_failure_probability = _max_failure_probability;
}

double TheoreticBound::objective_function(double x[]) {
	double obj = 0;
	for(int d = 1; d <= K; d++){
		obj += x[d - 1] * d * robust_soliton->get(d);
	}
	return obj;
}

void TheoreticBound::get_initial_solution(double x[]) {
	throw std::logic_error("Not implemented for this class");
}

bool TheoreticBound::respect_constraints(double* candidate_x) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution \f$ \vec{x} \f$,
	*/
	double E = objective_function(candidate_x);
	for (int d = 1; d <= K; d++) {
		/** - check if \f$ x_d \ge 1 \f$ */
		if(candidate_x[d - 1] < 1) {
			return false;
		}
		/**
		* - check that EDFC failure probability is below pre-set threshold
		* (see equations 8 and 10 in Lin's paper)
		*/
		double p = 1 - pow((1 - candidate_x[d - 1] * d / (N * E)), (N * E / K));
		double failure_probability = 1 - binomial_CDF(K, d, p);
		if(failure_probability > max_failure_probability) {
			return false;
		}
	}
	return true;
}

void TheoreticBound::get_neighbour(double x[], double new_x[]) {
	throw std::logic_error("Not implemented for this class");
}

double TheoreticBound::acceptance_probability(double old_x[], double new_x[]) {
	throw std::logic_error("Not implemented for this class");
}

void TheoreticBound::run_search(double x[]) {
	double candidate;
	for(int d = 1; d <= K; d++) {
		// set value only if it is not less than 1
		candidate = - log(max_failure_probability) / (double) d - log(d / (double) K);
		// dirty trick to make it work: multiply by 10
		x[d - 1] = 10 * ((candidate < 1) ? 1 : candidate);
	}

	// check obtained bound is valid (you never know...)
	if(!respect_constraints(x)) {
		throw std::logic_error("Theoretic bound is not a valid solution");
	}
}
