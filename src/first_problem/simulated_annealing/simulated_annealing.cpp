#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>
#include <chrono>
#include <fstream>

using namespace std::chrono;
using namespace std;

#include "soliton.h"
#include "binomial.h"
#include "simulated_annealing.h"
#include "first_problem.h"


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
		: FirstProblem::FirstProblem(
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

vector<double> SimulatedAnnealing::get_neighbour(vector<double> x) {
	// initialize distributions
	uniform_real_distribution<double>
		perturbation(- this->temperature*2, this->temperature);
	uniform_int_distribution<int> index_choice(0, this->K-1);

	vector<double> candidate;
	do {
		// copy x to new array before perturbation
		candidate = x;

		/**
		* ------------------------
		* ### Algorithm
		* Find new point candidate modifying previous solution.
		* Component \f$ d \sim \mathcal{U}[1, K] \f$ of \f$ \vec{x} \f$ is perturbed
		* summing it a quantity \f$ \xi \sim \mathcal{U}[-2T, T] \f$
		*/
		int chosen_d = index_choice(rng);
		candidate[chosen_d] = x[chosen_d] + perturbation(rng);
	} while (!FirstProblem::respect_constraints(candidate));

	return candidate;
}

double SimulatedAnnealing::acceptance_probability(
	vector<double> old_x, vector<double> new_x) {

	double delta = objective_function(new_x) - objective_function(old_x);
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

vector<double> SimulatedAnnealing::run_search(string progress_file_name) {
	vector<double> x = FirstProblem::get_initial_solution();

	// prepare stream for output timing file
	ofstream progress_file;
	progress_file.open(progress_file_name);
  	progress_file << "Time,score" << "\n";

	// compute normalization factor for score
	vector<double> no_redundancy(K, 1);
	double norm_obj_function = this->objective_function(no_redundancy);

	milliseconds begin_time
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	// keep trace of how much jumps have been done since the beginning
	int current_iteration = 0;

	// save current best result in these variables
	vector<double> best_x(this->K, 1);
	double best_score = numeric_limits<double>::infinity();

	// store here the best value of x when swapping old with new one (see below)
	vector<double> tmp;

	// store here new point explored with its score
	vector<double> new_x(this->K, 1);
	double new_score;

	// random variable that set the threshold for accepting worse solutions
	uniform_real_distribution<double> acceptance_threshold(0,1);

	while(current_iteration <= this->max_iterations) {
		// round of search for current temperature
		cerr << "=====> "
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
			new_x = get_neighbour(x);

			// increment mean probability and counter when new_x is worse than x
			if(acceptance_probability(x, new_x) != 1){
				acceptance_mean += acceptance_probability(x, new_x);
				worsening_proposals++;
			}

			// accept or reject according to acceptance probability
			if( acceptance_probability(x, new_x) >= acceptance_threshold(rng) ) {
				// subistitute x with new value (note that assignment with vector is
				// equivalent to a copy)
				tmp = x;
				x = new_x;
				new_x = tmp;

				new_score = objective_function(x);
				// update best result (up to now) if needed
				if (new_score < best_score) {
					best_score = new_score;
					best_x = x;
				}
			}
		}

		// save current best score in output file
		milliseconds current_time
			= duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	  progress_file << (current_time - begin_time).count() << ","
									<< (best_score / norm_obj_function) << "\n";

		// report mean of acceptance probability up to now
		acceptance_mean = acceptance_mean / worsening_proposals;
		//cout << "mean of acceptance_probability = " << acceptance_mean << '\n';

		// update temperature for new round
		this->temperature = new_temperature();
	}

	progress_file.close();
	return best_x;
}
