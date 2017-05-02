#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <random>
#include <string>

#include "soliton.h"
#include "binomial.h"
#include "firstProblem.h"

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
	* for each componenets of candidate solution x
	*/
	double E = objective_function(candidate_x);
	for (int d = 1; d <= K; d++) {
		/** - check if $ x_d \ge 1 $ */
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
			// std::ostringstream stream;
			// stream << "failure " << failure_probability << ", max_failure "
			// 	<< max_failure_probability << " for x[" << d << "] = " << candidate_x[d-1];
			// throw std::logic_error(stream.str());
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
		// throw std::logic_error("Theoretic bound is not a valid solution");
	}
}

SimulatedAnnealing::SimulatedAnnealing(	int _K,
										int _N,
										RobustSoliton* _robust_soliton,
										double _max_failure_probability,
										double _starting_temperature,
										double _cooling_rate,
										int _max_iterations)
										// constructor of upper class to trigger
										: TheoreticBound::TheoreticBound(
											_K,
											_N,
											_robust_soliton,
											_max_failure_probability) {
	// annealing specific parameters
	temperature = _starting_temperature;
	cooling_rate = _cooling_rate;
	max_iterations = _max_iterations;

	// random seed is set to a default value, for reproducibility
	my_rng.seed(1);
}

void SimulatedAnnealing::get_initial_solution(double x[]) {
	// employ theoretic bound to get a suitable starting point
	// x value has already been proven valid in superclass
	TheoreticBound::run_search(x);
}

void SimulatedAnnealing::get_neighbour(double x[], double new_x[]) {
	/**
	* ------------------------
	* ### Algorithm
	*  Find new point candidate modifying previous solution with random
	* quantities, performing the so called _perturbation_.
	*/
	// copy x
	for(int d=0; d<K; d++) {
		new_x[d] = x[d];
	}

	std::uniform_real_distribution<double> perturbation(-1, 1);
	//ALT for(int d=0; d<K; d++) {
	//ALT 	new_x[d] = x[d] + perturbation(my_rng);
	//ALT }
	std::uniform_int_distribution<int> index_choice(0, K - 1);

	int chosen_d = index_choice(my_rng);
	//DEBUG std::cout << "d = " << chosen_d << "\n";
	double asd = perturbation(my_rng);

	//DEBUG std::cout << "Before " << (new_x[chosen_d] - x[chosen_d]) << "\n";
	new_x[chosen_d] = x[chosen_d] + asd;

	//DEBUG std::cout << "After " << (new_x[chosen_d] - x[chosen_d]) << "\n";
	//DEBUG std::cout << "Variation of " << asd << "\n";

	/** Return candidate if it is a valid solution,
		else perturb previous point again */
	if( !respect_constraints(new_x) ) {
		//DEBUG std::cout << "Failed neighbour, trying again\n";
		get_neighbour(x, new_x);
	}
}

double SimulatedAnnealing::acceptance_probability(double x[], double new_x[]) {
	double delta = objective_function(new_x) - objective_function(x);
	/**
	* ---------------
	* ### Algorithm
	* - accept better solutions w.p. 1
	*/
	if(delta <= 0){
		return 1.0;
		//DEBUG std::cout << "\n";
	}
	/**
	* - accept worse solutions w.p. $e^{- \frac{\Delta}{T} }$
	*/
	//DEBUG std::cout << "proposing suboptimal with probability " << exp(-delta / temperature) << "\n";
	return exp(-delta / temperature);
}

double SimulatedAnnealing::new_temperature() {
	// exponential decay of temperature
	return temperature * cooling_rate;
}

double SimulatedAnnealing::temperature_steps() {
	return 750000.0/temperature;
}

void copy_to(double new_location[], double array[], int len) {
	for(int i=0; i<len; i++) {
		new_location[i] = array[i];
	}
}

void print_array(double x[], int K) {
	std::cout << "[";
	for(int i=0; i<K-1; i++) {
		std::cout << x[i] << ", ";
	}
	std::cout << x[K-1] << "]\n";
}

void SimulatedAnnealing::run_search(double x[]) {
	get_initial_solution(x);

	int current_iteration = 0;

	// save best current result in this variable
	double* best_x = (double*) malloc(K * sizeof(double));
	double best_score = std::numeric_limits<double>::infinity();

	// current candidates
	double* tmp = (double*) malloc(K * sizeof(double));

	double* new_x = (double*) malloc(K * sizeof(double));
	double new_score;

	while(current_iteration <= max_iterations) {
		// round of search for current temperature
		for(int i = 0; i < temperature_steps(); i++) {
			// better organization of cycles needed
			current_iteration++;
			std::cout << "Iteration " << current_iteration << "\n";

			// if (current_iteration > max_iterations) break;
			//DEBUG std::cout << "iteration " << current_iteration << ": ";

			// search new candidate, save it to new_x

			get_neighbour(x, new_x);
			//DEBUG print_array(new_x, K);
			//HERE new_x is overwritten

			// accept or reject according to acceptance probability
			if( acceptance_probability(x, new_x) > 0.9999 ) {
				//DEBUG std::cout << "new point accepted at " << current_iteration << "\n";

				//HERE x and new_x are swapped
				tmp = x;
				x = new_x;
				new_x = tmp;

				// update best result (up to now) if needed
				new_score = objective_function(new_x);
				if (new_score < best_score) {
					best_score = new_score;
					std::cout << "New best score is " << best_score << "\n";
					// save current x to best_x location
					copy_to(best_x, x, K);
					// print_array(best_x, K);
				}
			}
		}
		// update temperature
		temperature = new_temperature();
	}
	// save to variable x best result found up to now
	x = best_x;
}
