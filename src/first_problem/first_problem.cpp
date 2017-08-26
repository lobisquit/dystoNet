#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "soliton.h"
#include "binomial.h"
#include "first_problem.h"

FirstProblem::FirstProblem(
	int K,
	int N,
	RobustSoliton* robust_soliton,
	double max_failure_probability) {
	this->K = K;
	this->N = N;
	this->robust_soliton = robust_soliton;
	this->max_failure_probability = max_failure_probability;
	this->rng = robust_soliton->get_rng();
}

double FirstProblem::objective_function(vector<double> x) {
	double obj = 0;
	for(int d = 1; d <= this->K; d++){
		obj += x[d - 1] * d * this->robust_soliton->probability(d);
	}
	return obj;
}

double FirstProblem::update_objective_function(vector<double> x, individual old_x){
	double new_obj = old_x.obj_function;
	for(int d = 1; d <= this->K; d++){
		if(old_x.values[d - 1] != x[d - 1]){
			double mu_d = this->robust_soliton->probability(d);
			new_obj -= old_x.values[d - 1] * d * mu_d;
			new_obj += x[d - 1] * d * mu_d;
		}
	}
	return new_obj;
}

vector<double> FirstProblem::get_initial_solution() {
	vector<double> x(this->K, 1);
	uniform_real_distribution<double> generator(1, 100);

	while (!this->respect_constraints(x)) {
		// create a new point ex-novo, until constrants are met
		for(int j=0; j<this->K; j++){
			x[j] = generator(this->rng);
		}
	}
	return x;
}

bool FirstProblem::respect_constraints(vector<double> candidate_x) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution \f$ \vec{x} \f$,
	*/
	double E = objective_function(candidate_x);
	for (int d = 1; d <= this->K; d++) {
		/** - check if \f$ x_d \ge 1 \f$ */
		if(candidate_x[d - 1] < 1) {
			return false;
		}
		/**
		* - check that EDFC failure probability is below pre-set threshold
		* (see equations 8 and 10 in Lin's paper)
		*/
		double p = 1 - pow((1 - candidate_x[d - 1] * d / (this->N * E)), (this->N * E / this->K));
		double failure_probability = 1 - binomial_CDF(this->K, d, p);
		if(failure_probability > this->max_failure_probability) {
			return false;
		}
	}
	return true;
}

bool FirstProblem::update_respect_constraints(individual candidate_x, individual old_x) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution \f$ \vec{x} \f$,
	*/
	double E = candidate_x.obj_function;
	for (int d = 1; d <= this->K; d++) {
		if(candidate_x.values[d - 1] != old_x.values[d - 1]){
			/** - check if \f$ x_d \ge 1 \f$ */
			if(candidate_x.values[d - 1] < 1) {
				return false;
			}
			/**
			* - check that EDFC failure probability is below pre-set threshold
			* (see equations 8 and 10 in Lin's paper)
			*/
			double p = 1 - pow((1 - candidate_x.values[d - 1] * d / (this->N * E)), (this->N * E / this->K));
			double failure_probability = 1 - binomial_CDF(this->K, d, p);
			if(failure_probability > this->max_failure_probability) {
				return false;
			}
		}
	}
	return true;
}
