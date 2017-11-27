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
				(binomial_CDF(this->K, i-1, p) - binomial_CDF(this->K, i, p));
		}
		// sum to objective function for each component of v
		obj += pow(v_prime - this->robust_soliton->probability(i), 2);
	}
	return obj;
}

individual SecondProblem::approximate_objective_function(vector<double> v) {

	double E = 0;
	individual obj;
	obj.v_prime.resize(this->K,0);

	for(int j=1; j<=this->K; j++){
		E += j*v[j-1];
	}

	double increment;

	for(int i = 1; i <= this->K; i++) {
		for(int d = 1; d <= this->K; d++) {

			double p = 1 - pow((1 - d / (this->N * E)), (this->N * E / this->K));
			increment = v[d - 1] *
				// compute pdf using difference of subsequent CDF
				(binomial_CDF(this->K, i, p) - binomial_CDF(this->K, i+1, p));
			// stop if tail of distribution is reached
			if (increment == 0 && d > 2) {
				break;
			}
			obj.v_prime[i-1] += increment;
			// cout << "\n" << obj.v_prime[i-1];
		}
		// break;
		// sum to objective function for each component of v
		increment = pow(obj.v_prime[i-1] - this->robust_soliton->probability(i), 2);
		obj.obj_function += increment;
		// stop if increment becomes negligible
		if (increment < 1e-12) {
			break;
		}
	}
	obj.values = v;
	return obj;
}

individual SecondProblem::update_objective_function(individual old_individual, vector<double> v, int first_d, int second_d) {
	double E, old_E = 0;
	double increment;
	individual new_individual = old_individual;
	new_individual.values = v;
	new_individual.obj_function = 0;

	for(int j=1; j<=this->K; j++){
		old_E += j*old_individual.values[j-1];
	}
	E = old_E + (first_d+1)*(v[first_d] - old_individual.values[first_d]) + (second_d+1)*(v[second_d] - old_individual.values[second_d]);

	// Update objective function
	for(int i = 1; i <= this->K; i++) {
		//----- first_d -----
		double old_p = 1 - pow((1 - (first_d+1) / (this->N * old_E)), (this->N * old_E / this->K));
		increment = old_individual.values[first_d] *
			// compute pdf using difference of subsequent CDF
			(binomial_CDF(this->K, i, old_p) - binomial_CDF(this->K, i+1, old_p));
		if (increment != 0 || first_d <= 2) {
			new_individual.v_prime[i-1] -= increment;
		}
		double new_p = 1 - pow((1 - (first_d+1) / (this->N * E)), (this->N * E / this->K));
		increment = new_individual.values[first_d] *
			// compute pdf using difference of subsequent CDF
			(binomial_CDF(this->K, i, new_p) - binomial_CDF(this->K, i+1, new_p));
		if (increment != 0 || first_d <= 2) {
			new_individual.v_prime[i-1] += increment;
		}

		//----- second_d -----
		old_p = 1 - pow((1 - (second_d+1) / (this->N * old_E)), (this->N * old_E / this->K));
		increment = old_individual.values[second_d] *
			// compute pdf using difference of subsequent CDF
			(binomial_CDF(this->K, i, old_p) - binomial_CDF(this->K, i+1, old_p));
		if (increment != 0 || second_d <= 2) {
			new_individual.v_prime[i-1] -= increment;
		}
		new_p = 1 - pow((1 - (second_d+1) / (this->N * E)), (this->N * E / this->K));
		increment = new_individual.values[second_d] *
			// compute pdf using difference of subsequent CDF
			(binomial_CDF(this->K, i, new_p) - binomial_CDF(this->K, i+1, new_p));
		if (increment != 0 || second_d <= 2) {
			new_individual.v_prime[i-1] += increment;
		}

		increment = pow(new_individual.v_prime[i-1] - this->robust_soliton->probability(i), 2);
		new_individual.obj_function += increment;
		// stop if increment becomes negligible
		if (increment < 1e-12) {
			break;
		}
	}

	return new_individual;
}

vector<double> SecondProblem::get_initial_solution() {
	/**
	* ---------------
	* ### Algorithm
	*/
	vector<double> v(this->K, 1);
	uniform_real_distribution<double> generator(1, 100);

	double sum = 0;
	/** - create a new point ex-novo with real random values between 0 and 100 and normalize it */
	for (int j=0; j<this->K; j++) {
		v[j] = generator(this->rng);
		sum += v[j];
	}

	for (int j=0; j<this->K; j++) {
		v[j] /= sum;
	}

	/** - constraints are surely met, since components are > 0 and sum to 1 by construction */
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
	if(sum - 1.0 > 1e-14) {
		return false;
	}
	return true;
}

vector<double> SecondProblem::normalize(vector<double> v){
	double sum = 0;
	for(int j=0; j<this->K; j++){
		sum += v[j];
	}

	for(int j=0; j<this->K; j++){
		v[j] /= sum;
	}

	return v;
}
