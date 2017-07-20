#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>

#include "soliton.h"
#include "binomial.h"
#include "simulated_annealing.h"
#include "first_problem.h"

SimulatedAnnealing::SimulatedAnnealing(	int _K,
										int _N,
										RobustSoliton* _robust_soliton,
										double _max_failure_probability,
										double _starting_temperature,
										double _cooling_rate,
										int _max_iterations,
										double _steps_coefficient,
										double _acceptance_coefficient)
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
	steps_coefficient = _steps_coefficient;
	acceptance_coefficient = _acceptance_coefficient;

	// random seed is set to a default value, for reproducibility
	rng.seed(time(NULL));
}

void SimulatedAnnealing::get_initial_solution(double x[]) {
	// employ theoretic bound to get a suitable starting point
	// x value has already been proven valid in superclass
	TheoreticBound::run_search(x);
}

void SimulatedAnnealing::get_neighbour(double x[], double new_x[]) {
	// copy x to new array before perturbation
	for(int d=0; d<K; d++) {
		new_x[d] = x[d];
	}

	/**
	* ------------------------
	* ### Algorithm
	* Find new point candidate modifying previous solution.
	* component \f$ d \sim \mathcal{U}[1, K] \f$ of \f$ \vec{x} \f$ is perturbed
	* summing it a quantity \f$ \xi \sim \mathcal{U}[-1, 1] \f$
	*/
	std::uniform_real_distribution<double> perturbation(-50, 50);
	std::uniform_int_distribution<int> index_choice(0, K-1);
	int chosen_d = index_choice(rng);
	new_x[chosen_d] = x[chosen_d] + perturbation(rng);

	/** Stop if perturbation jumps inside valid solutions space,
		otherwise perturb \f$ \vec{x} \f$ again */
	if( !respect_constraints(new_x) ) {
		get_neighbour(x, new_x);
	}
}

double SimulatedAnnealing::acceptance_probability(double x[], double new_x[]) {
	double delta = objective_function(new_x) - objective_function(x);
	/**
	* ---------------
	* ### Algorithm
	* - accept better solutions with probability 1
	*/
	if(delta <= 0){
		return 1.0;
	}
	/**
	* - accept worse solutions with probability \f$ e^{- \frac{acceptance_coefficient \Delta}{T} } \f$
	*/
	return exp(-acceptance_coefficient * delta / temperature);
}

double SimulatedAnnealing::new_temperature() {
	// exponential decay of temperature
	return temperature * cooling_rate;
}

double SimulatedAnnealing::temperature_steps() {
	return steps_coefficient / temperature;
}

void SimulatedAnnealing::run_search(double x[]) {
	get_initial_solution(x);

	// keep trace of how much jumps have been done since the beginning
	int current_iteration = 0;

	// save current best result in this variable
	double* best_x = (double*) malloc(K * sizeof(double));
	double best_score = std::numeric_limits<double>::infinity();

	// store here the best value of x when swapping old with new one (see below)
	double* tmp; // = (double*) malloc(K * sizeof(double));

	// store here new point explored with its score
	double* new_x = (double*) malloc(K * sizeof(double));
	double new_score;

	// random variable that set the threshold for accepting worse solutions
	std::uniform_real_distribution<double> acceptance_threshold(0,1);

	while(current_iteration <= max_iterations && temperature > 0.5) {
		// round of search for current temperature
		// std::cout << "Temperature " << temperature << " at iteration " <<
		// 	current_iteration << "/" << max_iterations << "\n";
		// std::cout << "Best score: " << best_score << "\n";

		// keep trace of mean value of acceptance_probability when new point is worse
		double acceptance_mean = 0;
		int worsening_proposals = 0;

		for(int i = 0; i < temperature_steps(); i++) {
			// better organization of cycles needed
			current_iteration++;

			// search new candidate, save it to new_x
			get_neighbour(x, new_x);

			// increment mean probability and counter when new_x is worse than x
			if(acceptance_probability(x, new_x) != 1){
				acceptance_mean += acceptance_probability(x, new_x);
				worsening_proposals++;
			}

			// accept or reject according to acceptance probability
			if( acceptance_probability(x, new_x) >= acceptance_threshold(rng) ) {
				// subistitute x with new value
				tmp = x;
				x = new_x;
				new_x = tmp;

				// update best result (up to now) if needed
				new_score = objective_function(x);
				if (new_score < best_score) {
					best_score = new_score;
					// save current x to best_x location
					for(int j=0; j<K; j++) {
						best_x[j] = x[j];
					}
				}
			}
		}

		// report mean of acceptance probability up to now
		acceptance_mean = acceptance_mean / worsening_proposals;
		//std::cout << "mean of acceptance_probability = " << acceptance_mean << '\n';

		// update temperature for new round
		temperature = new_temperature();
	}

	// save to variable x the best result found up to now, then exit
	x = best_x;
}
