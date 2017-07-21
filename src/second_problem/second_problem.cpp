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
	for(int i = 1; i <= this->K; i++){
		double v_first = 0;
		for(int d = 1; d<= this->K; d++){
			double p = 1 - pow((1 - d / (this->N * E)), (this->N * E / this->K));
			v_first += v[d-1]*(binomial_CDF(this->K, i, p) - binomial_CDF(this->K, i-1, p));
		}
		obj += pow(v_first - this->robust_soliton->probability(i),2);
	}
	return obj;
}

vector<double> SecondProblem::get_initial_solution() {
	vector<double> v(this->K, 1);
	uniform_real_distribution<double> generator(1, 100);

	do{
		double sum = 0;
		// create a new point ex-novo, until constrants are met
		for(int j=0; j<this->K; j++){
			v[j] = generator(this->rng);
			sum += v[j];
		}

		for(int j=0; j<this->K; j++){
			v[j] /= sum;
		}
	}while (!this->respect_constraints(v));

	for(int i=0; i<10; i++)
		cout << v[i] << ' ';
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
		/**
		* - check that chosen degree distribution sum to 1
		* (see equation 18 in Lin's paper)
		*/
		double sum = 0;
		for(int i = 1; i<=this->K; i++){
			sum += candidate_v[i-1];
		}
		if(sum != 1) {
			return false;
		}
	}
	return true;
}
