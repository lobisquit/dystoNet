#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "soliton.h"
#include "binomial.h"
#include "second_problem.h"

using namespace std;

SecondProblem::SecondProblem(
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

double SecondProblem::objective_function(vector<double> v) {
	double E = 0;
	for(int j=1; j<=this->K; j++){
		E += j*v[j-1];
	}

	double obj = 0;
	for(int i = 1; i <= this->K; i++) {
		double v_prime = 0;

		for(int d = 1; d <= this->K; d++) {
			double p = 1 - pow((1 - d / (this->N * E)), (this->N * E / this->K));
			v_prime += v[d - 1] *
				// compute pdf using different of subsequent CDF
				(binomial_CDF(this->K, i, p) - binomial_CDF(this->K, i - 1, p));
		}
		// sum to objective function for each component of v
		obj += pow(v_prime - this->robust_soliton->probability(i), 2);
	}
	return obj;
}

double SecondProblem::approximate_objective_function(vector<double> v) {
	double E = 0;
	for(int j=1; j<=this->K; j++){
		E += j*v[j-1];
	}

	double obj = 0;
	double increment;

	for(int i = 1; i <= this->K; i++) {
		double v_prime = 0;

		for(int d = 1; d <= this->K; d++) {
			double p = 1 - pow((1 - d / (this->N * E)), (this->N * E / this->K));
			increment = v[d - 1] *
				// compute pdf using difference of subsequent CDF
				(binomial_CDF(this->K, i, p) - binomial_CDF(this->K, i-1, p));
			// stop if tail of distribution is reached
			if (increment == 0 && d > 2) {
				break;
			}
			v_prime += increment;
		}
		// sum to objective function for each component of v
		increment = pow(v_prime - this->robust_soliton->probability(i), 2);
		obj += increment;
		// stop if increment becomes negligible
		if (increment < 1e-12) {
			break;
		}
	}
	return obj;
}

vector<double> SecondProblem::get_initial_solution() {
	vector<double> v(this->K, 1);
	uniform_real_distribution<double> generator(1, 100);

	double sum = 0;
	// create a new point ex-novo, until constrants are met
	for (int j=0; j<this->K; j++) {
		v[j] = generator(this->rng);
		sum += v[j];
	}
	for (int j=0; j<this->K; j++) {
		v[j] /= sum;
	}
	// constraints are surely met, since components
	// are > 0 and sum to 1 by construction
	return v;
}

bool SecondProblem::respect_constraints(vector<double> candidate_v) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution \f$ \vec{x} \f$,
	*/
	for (int d = 1; d <= this->K; d++) {
		/** - check if \f$ x_d \ge 1 \f$ */
		if(candidate_v[d - 1] < 0) {
			return false;
		}
	}
	/**
	* - check that chosen degree distribution sum to 1
	* (see equation 18 in Lin's paper)
	*/
	double sum = 0;
	for(int i = 1; i<=this->K; i++){
		sum += candidate_v[i-1];
	}
	if(sum - 1.0 > 1e-15) {
		return false;
	}
	return true;
}

vector<double> SecondProblem::normalize(vector<double> v){
	double sum = 0;
	/** Then normalize it */
	for(int j=0; j<this->K; j++){
		sum += v[j];
	}

	for(int j=0; j<this->K; j++){
		v[j] /= sum;
	}

	return v;
}
