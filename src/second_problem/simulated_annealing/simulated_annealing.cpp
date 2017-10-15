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

individual SimulatedAnnealing::get_neighbour(individual old_individual) {
	// initialization distributions
	uniform_int_distribution<int> index_choice(0, this->K-1);

	individual new_individual;

	vector<double> candidate = old_individual.values;
	int first_d;
	int second_d;
	do {
		// copy x to new array before perturbation
		do {
			first_d = index_choice(rng);
			// start_d must have a non-zero probability
		} while(candidate[first_d] == 0);

		do {
			second_d = index_choice(rng);
			// end_d can't go beyond 1 after move
		} while(second_d == first_d || candidate[first_d] + candidate[second_d] > 1);

		// "move" some probability from start to end point
		uniform_real_distribution<double> perturbation(0,
																									 min(candidate[first_d],
																											 candidate[second_d]));

		double delta = perturbation(rng);
		candidate[first_d]  -= delta;
		candidate[second_d] += delta;

	} while (!SecondProblem::respect_constraints(candidate));

	new_individual = update_objective_function(old_individual,
																						 candidate,
																						 first_d,
																						 second_d);

	return new_individual;
}

double SimulatedAnnealing::acceptance_probability(double delta) {
	/**
	* ---------------
	* ### Algorithm
	* - accept better solutions with probability 1
	*/
	if(delta <= 0) {
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

	individual new_individual;

	individual old_individual = approximate_objective_function(v);
	double old_score = old_individual.obj_function;

	while(current_iteration <= this->max_iterations) {
		// round of search for current temperature
		for(int i = 0; i < temperature_steps(); i++) {
			current_iteration++;

			// search new candidate, save it to new_x
			new_individual = get_neighbour(old_individual);
			new_score = new_individual.obj_function;
			new_v = new_individual.values;

			double delta = new_score - old_score;
			// accept or reject according to acceptance probability
			if( acceptance_probability(delta) >= acceptance_threshold(rng) ) {
				// subistitute v with new value (note that assignment with vector is
				// equivalent to a copy)
				tmp = v;
				v = new_v;
				new_v = tmp;

				Distribution* v_distribution = new Distribution(v, 1);
				double g2 =
					v_distribution->expectation() /
					robust_soliton->expectation();

				cerr << "=====> "
					<< current_iteration << "/" << this->max_iterations
					<< " ==> temperature = " << this->temperature
					<< " ==> score = " << new_score
					<< " ==> g2 = " << g2 << "\r";

				// update best result (up to now) if needed
				if (new_score < best_score) {
					best_score = new_score;
					best_v = v;

					cerr << "=====> "
						<< current_iteration << "/" << this->max_iterations
						<< " ==> temperature = " << this->temperature
						<< " ==> score = " << new_score
						<< " ==> g2 = " << g2 << "\n";
				}
			}
			// keep current score for the next cycle
			old_score = new_score;
			old_individual = new_individual;
		}

		// update temperature for new round
		this->temperature = new_temperature();
	}

	return best_v;
}
