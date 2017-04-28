/*!
* \file
* Author: Davide Peron, Enrico Lovisotto
*/
#include <limits>
#include <stdexcept>
#include <math.h>
#include <random>
#include "HeuristicSearch.h"
#include "soliton.h"

SimulatedAnnealing::SimulatedAnnealing(	int _K,
										int _N,
										double _starting_temperature,
										double _cooling_rate,
										int _max_iterations,
										RobustSoliton* _robust_soliton
										double _max_failure_probability) {
	K = _K;
	N = _N;
	starting_temperature = _starting_temperature;
	cooling_rate = _cooling_rate;
	max_iterations = _max_iterations;
	robust_soliton = _robust_soliton;
	failure_probability = _failure_probability:
}

double SimulatedAnnealing::objective_function(double* x) {
	double obj = 0;
	for(int d = 1; d <= K; d++){
		obj += x[d - 1] * d * robust_soliton.get(d);
	}
	return obj;
}

double* SimulatedAnnealing::get_initial_solution() {
	double x[K];
	for (int d=0; d<K; d++) {
		x[d] = 10; // just an example
	}
	if (!respect_constraints(x)) {
		throw std::runtime_error("Invalid initial solution for simulated annealing");
	}
	return x;
}

bool SimulatedAnnealing::respect_constraints(double* candidate_x) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution x
	*/
	double E = objectiveFunction(candidate_x);
	for (int d=0; d++; d<K) {
		/** - check if $ x_d \ge 1 $ */
		if(candidate_x[d] < 1) {
			return false;
		}foldd
		/**
		* - check that EDFC failure probability is below pre-set threshold
		* (see equations 8 and 10 in Lin's paper)
		*/
		double p = 1 - pow((1 - candidate_x[d] * d / (N * E)),(N * E / K));
		failure_probability = 1 - binomial_CDF(K, d, p);
		if(failure_probability > max_failure_probability) {
			return false;
		}
	}
	return true;
}

double* SimulatedAnnealing::get_neighbour(double* x) {
	double new_x[K];
	/**
	* ------------------------
	* ### Algorithm
	*  Find new point candidate modifying previous solution with random
	* quantities, performing the so called _perturbation_.
	*/
	for(int d=0; d<K; d++){
		new_x[K] = x[K] + (... random number in [0; 1) ...);
	}

	/**
	* Return candidate if it is a valid solution,
	* else perturb previous point again
	*/
	if( respectConstraints(new_x) ) {
		return new_x;
	}
	return getNeighbor(x);
}

double SimulatedAnnealing::acceptance_probability(double* old_x, double* new_x) {
	double delta = objective_function(new_x, old_x);
	/**
	* ---------------
	* ### Algorithm
	* - accept better solutions w.p. 1
	*/
	if(delta <= 0){
		return 1.0;
	}
	/**
	* - accept worse solutions w.p. $e^{- \frac{\Delta}{T} }$
	*/
	return exp(-delta / T);
}

double SimulatedAnnealing::new_temperature() {
	// exponential decay of temperature
	return temperature * cooling_rate;
}

double SimulatedAnnealing::temperature_steps() {
	return 750000.0/temperature;
}

double* SimulatedAnnealing::run_search() {
	double* x = get_initial_solution();

	int current_iteration = 0;

	// save best current result in this variable
	double* best_x[K];
	double best_F = std::numeric_limits<double>::infinity();

	double* new_x;

	while(current_iteration <= max_iterations){
		// round of search for current temperature
		for(int i = 0; i < temperature_steps(); i++){
			current_iteration++;

			// search new candidate
			new_x = get_neighbour(x);

			// accept or reject according to probability
			if( acceptance_probability(x, new_x) > rand(0, 1) ) {
				x = new_x;

				// update best result (up to now) if needed
				if (objective_function(new_x) > best_F) {
					best_F = new_F;
					best_X = x;
				}
			}
		}
		// update temperature
		temperature = new_temperature();
	}

	return *bestx;
}
