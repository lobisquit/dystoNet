#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>

#include "soliton.h"
#include "binomial.h"
#include "first_problem.h"
#include "jumping_ball.h"

JumpingBall::JumpingBall(   int _K,
									    int _N,
										RobustSoliton* _robust_soliton,
										double _max_failure_probability,
										double _starting_temperature,
										double _cooling_rate,
										int _max_iterations,
										double _steps_coefficient)
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

	minkiata=0;

	// random seed is set to a default value, for reproducibility
	rng.seed(1);
}

void JumpingBall::get_initial_solution(double x[]) {
	// employ theoretic bound to get a suitable starting point
	// x value has already been proven valid in superclass
	TheoreticBound::run_search(x);
}

void JumpingBall::get_neighbour(double x[], double new_x[], double best_score) {
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

	double a = 0;

	double b = 0;

	a = best_score-objective_function(x);

	b = minkiata;

	if (a<0) {
		minkiata=minkiata+1;
	}
	else{
		minkiata=0;
	}

	if (minkiata>1000) {
		std::uniform_int_distribution<int> number_of_variation(0, K-1);
		for(int d=0; d<number_of_variation(rng); d++) {
			std::uniform_real_distribution<double> perturbation(-50, 50);
			std::uniform_int_distribution<int> index_choice(0, K-1);
			int chosen_d = index_choice(rng);
			new_x[chosen_d] = x[chosen_d] + perturbation(rng);
		}
		minkiata=0;
		std::cout << "jump" <<'\n';
		std::cout << "Dove cazzo sto andando?" << objective_function(new_x) << '\n';
		std::cout << "Miglior risultato?" << best_score << '\n';
	}
	else {
		std::uniform_real_distribution<double> perturbation(-50, 50);
		std::uniform_int_distribution<int> index_choice(0, K-1);
		int chosen_d = index_choice(rng);
		new_x[chosen_d] = x[chosen_d] + perturbation(rng);
	}

	/** Stop if perturbation jumps inside valid solutions space,
		otherwise perturb \f$ \vec{x} \f$ again */

	if( !respect_constraints(new_x) ) {
		// std::cout << "ops" << '\n';
		minkiata=b;
		get_neighbour(x, new_x, best_score);
	}
}

double JumpingBall::acceptance_probability(double x[], double new_x[]) {
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
	* - accept worse solutions with probability \f$ e^{- \frac{200\Delta}{T} } \f$, where \f$ 200\Delta \f$ is to make this probability
	* uniformely distributed in (0,1)
	*/
	//std::cout << exp(-200*delta / temperature) << '\n';
	return exp(-200*delta / temperature);
}

double JumpingBall::new_temperature() {
	// exponential decay of temperature
	return temperature * cooling_rate;
}

double JumpingBall::temperature_steps() {
	return steps_coefficient / temperature;
}

void JumpingBall::run_search(double x[]) {
	get_initial_solution(x);

	int current_iteration = 0;
	std::uniform_real_distribution<double> die(0,1);

	// save best current result in this variable
	double* best_x = (double*) malloc(K * sizeof(double));
	double best_score = std::numeric_limits<double>::infinity();

	// current candidates
	double* tmp = (double*) malloc(K * sizeof(double));

	// double new_x[K];
	double* new_x = (double*) malloc(K * sizeof(double));
	double new_score;

	while(current_iteration <= max_iterations && temperature > 0.5) {

		std::cout << "Temperature " << temperature << " at iteration " <<
		current_iteration << "/" << max_iterations << "\n";
		std::cout << "Best score: " << best_score << "\n";

		// round of search for current temperature
		double mean = 0;
		int acc = 0;
		for(int i = 0; i < temperature_steps(); i++) {
			// better organization of cycles needed
			current_iteration++;

			// search new candidate, save it to new_x
			get_neighbour(x, new_x, best_score);

			// accept or reject according to acceptance probability
			if(acceptance_probability(x, new_x) != 1){
				mean += acceptance_probability(x, new_x);
				acc++;
			}
			if( acceptance_probability(x, new_x) >= die(rng) ) {
				// subistitute x with new value
				tmp = x;
				x = new_x;
				new_x = tmp;

				// update best result (up to now) if needed
				new_score = objective_function(x);
				if (new_score < best_score) {
					std::cout << "New best " << new_score << " at iteration " << current_iteration << "\n";
					best_score = new_score;
					// save current x to best_x location
					for(int j=0; j<K; j++) {
						best_x[j] = x[j];
					}
				}
			}
		}
		mean /= acc;
		std::cout << "acceptanceProbability mean = " << mean << '\n';
		// update temperature
		temperature = new_temperature();
	}
	// save to variable x the best result found up to now
	x = best_x;
}
