#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>

#include "soliton.h"
#include "binomial.h"
#include "simulated_annealing.h"
#include "second_problem.h"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing(
	int K,
	int N,
	RobustSoliton* robust_soliton,
	double max_failure_probability,
	double starting_temperature,
	double cooling_rate,
	int max_iterations,
	double steps_coefficient,
	double acceptance_coefficient)
	// constructor of upper class to trigger
		: SecondProblem::SecondProblem(
			K,
			N,
			robust_soliton,
			max_failure_probability) {
	// annealing specific parameters
	this->temperature = starting_temperature;
	this->cooling_rate = cooling_rate;
	this->max_iterations = max_iterations;
	this->steps_coefficient = steps_coefficient;
	this->acceptance_coefficient = acceptance_coefficient;
}

vector<double> SimulatedAnnealing::get_neighbour(vector<double> v) {

	// initialization distributions
	uniform_real_distribution<double>
	perturbation(- this->temperature*2, this->temperature);
	uniform_int_distribution<int> index_choice(0, this->K-1);

	vector<double> candidate;
	do {
		// copy x to new array before perturbation
		candidate = v;

		/**
		* ------------------------
		* ### Algorithm
		* Find new point candidate modifying previous solution.
		* component \f$ d \sim \mathcal{U}[1, K] \f$ of \f$ \vec{x} \f$ is perturbed
		* summing it a quantity \f$ \xi \sim \mathcal{U}[-2T, T] \f$
		*/
		int chosen_d = index_choice(rng);
		candidate[chosen_d] = v[chosen_d] + perturbation(rng)/(this->temperature*this->K);

		candidate = SecondProblem::normalize(candidate);
	} while (!SecondProblem::respect_constraints(candidate));

	return candidate;
}

double SimulatedAnnealing::acceptance_probability(
	vector<double> old_v, vector<double> new_v) {
	double delta = objective_function(new_v) - objective_function(old_v);
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
	return exp(- this->acceptance_coefficient * delta / this->temperature);
}

double SimulatedAnnealing::new_temperature() {
	// exponential decay of temperature
	return this->temperature * this->cooling_rate;
}

double SimulatedAnnealing::temperature_steps() {
	return this->steps_coefficient / this->temperature;
}

vector<double> SimulatedAnnealing::run_search() {
	vector<double> v = SecondProblem::get_initial_solution();

	// keep trace of how much jumps have been done since the beginning
	int current_iteration = 0;

	// save current best result in these variables
	vector<double> best_v(this->K, 1);
	double best_score = numeric_limits<double>::infinity();

	// store here the best value of x when swapping old with new one (see below)
	vector<double> tmp;

	// store here new point explored with its score
	vector<double> new_v(this->K, 1);
	double new_score;

	// random variable that set the threshold for accepting worse solutions
	uniform_real_distribution<double> acceptance_threshold(0,1);

	while(current_iteration <= this->max_iterations) {
		// round of search for current temperature
		std::cerr << "=====> "
			<< current_iteration << "/" << this->max_iterations
			<< " ==> temperature: " << this->temperature
			<< " ==> Best score: " << best_score << "\n";

		// keep trace of mean value of acceptance_probability
		// when new point is worse (for better points it is 1)
		double acceptance_mean = 0;
		int worsening_proposals = 0;

		for(int i = 0; i < temperature_steps(); i++) {
			current_iteration++;
			// search new candidate, save it to new_x
			new_v = get_neighbour(v);
			// increment mean probability and counter when new_x is worse than x
			if(acceptance_probability(v, new_v) != 1){

				acceptance_mean += acceptance_probability(v, new_v);
				worsening_proposals++;
			}

			// accept or reject according to acceptance probability
			if( acceptance_probability(v, new_v) >= acceptance_threshold(rng) ) {
				// subistitute x with new value (note that assignment with vector is
				// equivalent to a copy)
				tmp = v;
				v = new_v;
				new_v = tmp;
				new_score = objective_function(v);
				// update best result (up to now) if needed
				if (new_score < best_score) {
					best_score = new_score;
					best_v = v;
				}
			}
		}

		// report mean of acceptance probability up to now
		acceptance_mean = acceptance_mean / worsening_proposals;

		// update temperature for new round
		this->temperature = new_temperature();
	}

	return best_v;
}
