/*!
* \file
* Author: Davide Peron, Enrico Lovisotto
*/
#include <limits>
#include <stdexcept>
#include <math.h>
#include <random>
#include <string>

#include "binomial.h"
#include "soliton.h"
#include "heuristicSearch.h"

SimulatedAnnealing::SimulatedAnnealing(	int _K,
										int _N,
										double _starting_temperature,
										double _cooling_rate,
										int _max_iterations,
										RobustSoliton* _robust_soliton,
										double _max_failure_probability) {
	K = _K;
	N = _N;
	temperature = _starting_temperature;
	cooling_rate = _cooling_rate;
	max_iterations = _max_iterations;
	robust_soliton = _robust_soliton;
	max_failure_probability = _max_failure_probability;
}

double SimulatedAnnealing::objective_function(double* x) {
	double obj = 0;
	for(int d = 1; d <= K; d++){
		obj += x[d - 1] * d * robust_soliton->get(d);
	}
	return obj;
}

void SimulatedAnnealing::get_initial_solution(double* x) {
	for (int d=0; d<K; d++) {
		x[d] = 10; // just an example
	}
	if (!respect_constraints(x)) {
		throw std::runtime_error("Invalid initial solution for simulated annealing");
	}
}

bool SimulatedAnnealing::respect_constraints(double* candidate_x) {
	/**
	* ------------------------
	* ### Algorithm
	* for each componenets of candidate solution x
	*/
	double E = objective_function(candidate_x);
	for (int d=0; d++; d<K) {
		/** - check if $ x_d \ge 1 $ */
		if(candidate_x[d] < 1) {
			return false;
		}
		/**
		* - check that EDFC failure probability is below pre-set threshold
		* (see equations 8 and 10 in Lin's paper)
		*/
		double p = 1 - pow((1 - candidate_x[d] * d / (N * E)),(N * E / K));
		double failure_probability = 1 - binomial_CDF(K, d, p);
		if(failure_probability > max_failure_probability) {
			return false;
		}
	}
	return true;
}

void SimulatedAnnealing::get_neighbour(double* x, double* new_x) {
	/**
	* ------------------------
	* ### Algorithm
	*  Find new point candidate modifying previous solution with random
	* quantities, performing the so called _perturbation_.
	*/
	for(int d=0; d<K; d++){
		new_x[K] = x[K] + 0.1;
	}

	/** Return candidate if it is a valid solution, */
	if( respect_constraints(new_x) ) {}
	/** else perturb previous point again */
	get_neighbour(x, new_x);
}

double SimulatedAnnealing::acceptance_probability(double* old_x, double* new_x) {
	double delta = objective_function(new_x) - objective_function(old_x);
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
	return exp(-delta / temperature);
}

double SimulatedAnnealing::new_temperature() {
	// exponential decay of temperature
	return temperature * cooling_rate;
}

double SimulatedAnnealing::temperature_steps() {
	return 750000.0/temperature;
}

void SimulatedAnnealing::run_search(double* x) {
	get_initial_solution(x);

	int current_iteration = 0;

	// save best current result in this variable
	double* best_x[K];
	double best_score = std::numeric_limits<double>::infinity();

	// current candidates
	double* new_x;
	double new_score;

	while(current_iteration <= max_iterations){
		// round of search for current temperature
		for(int i = 0; i < temperature_steps(); i++){
			current_iteration++;

			// search new candidate, save it to new_x
			get_neighbour(x, new_x);

			// accept or reject according to acceptance probability
			if( acceptance_probability(x, new_x) > 0.1 ) {
				x = new_x;

				// update best result (up to now) if needed
				new_score = objective_function(new_x);
				if (new_score > best_score) {
					best_score = new_score;
					*best_x = x;
				}
			}
		}
		// update temperature
		temperature = new_temperature();
	}
}
