#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>
#include <chrono>
#include <fstream>

using namespace std::chrono;

#include "soliton.h"
#include "binomial.h"
#include "first_problem.h"
#include "jumping_ball.h"

using namespace std;

JumpingBall::JumpingBall(
	int K,
	int N,
	RobustSoliton* robust_soliton,
	double max_failure_probability,
	double starting_temperature,
	double cooling_rate,
	int max_iterations,
	double steps_coefficient,
	double acceptance_coefficient,
	int max_worsening_steps)
		: SimulatedAnnealing::SimulatedAnnealing(
			K,
			N,
			robust_soliton,
			max_failure_probability,
			starting_temperature,
			cooling_rate,
			max_iterations,
			steps_coefficient,
			acceptance_coefficient) {
	this->max_worsening_steps = max_worsening_steps;
	this->worsening_steps = 0;
}

vector<double> JumpingBall::get_neighbour(vector<double> x) {
	vector<double> candidate(K, 1);

	do {
		// copy x to new array before perturbation
		candidate = x;

		// initialize distributions
		uniform_real_distribution<double>
			perturbation(-2*this->temperature, this->temperature);
		uniform_int_distribution<int> number_of_variations(0, this->K-1);
		uniform_int_distribution<int> index_choice(0, this->K-1);


		/**
		* ------------------------
		* ### Algorithm
		* Find new point candidate modyfing previous solution.
		* The search mode of the candidate depends on the number of time
		* the candidate did not improve.
		* If \f$ worsening_steps > max_worsening_steps \f$, I make the jump.
		* If not I proceed with the normal search method.
		*/

		if (worsening_steps > this->max_worsening_steps) {
			while (!respect_constraints(candidate)) {

				// A set of random components of the candidate is perturbed.

				for(int i=0; i<number_of_variations(this->rng); i++) {

					/*
					* Component \f$ d \sim \mathcal{U}[1, K] \f$ of \f$ \vec{x} \f$ is perturbed
					* summing it a quantity \f$ \xi \sim \mathcal{U}[-2T, T] \f$
					* I do this for number_of_variations times.
					*/

					int chosen_d = index_choice(this->rng);
					candidate[chosen_d] = x[chosen_d] + perturbation(this->rng);
				}
			}
			// After the jump I reset the parameters worsening_steps and num_jump.

			this->worsening_steps = 0;
			this->num_jump = this->num_jump+1;
		}
		else {

			/*
			* Only one component of the candidate is perturbed.
			* Component \f$ d \sim \mathcal{U}[1, K] \f$ of \f$ \vec{x} \f$ is perturbed
			* summing it a quantity \f$ \xi \sim \mathcal{U}[-2T, T] \f$
			*/

			uniform_real_distribution<double>
				perturbation(-2*this->temperature, this->temperature);
			uniform_int_distribution<int> index_choice(0, K-1);
			int chosen_d = index_choice(rng);
			candidate[chosen_d] = x[chosen_d] + perturbation(rng);
		}
	} while (!respect_constraints(candidate));

	return candidate;
}

vector<double> JumpingBall::run_search(string progress_file_name) {
	vector<double> x = FirstProblem::get_initial_solution();

	// prepare stream for output timing file
	ofstream progress_file;
	progress_file.open(progress_file_name);
	progress_file << "Time,score" << "\n";

	milliseconds begin_time
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	// compute normalization factor for score
	vector<double> no_redundancy(K, 1);
	double norm_obj_function = this->objective_function(no_redundancy);

	// keep trace of how much jumps have been done since the beginning
	int current_iteration = 0;

	// save current best result in these variables
	vector<double> best_x(this->K, 1);
	this->num_jump = 0;
	this->best_score = numeric_limits<double>::infinity();

	// store here the best value of x when swapping old with new one (see below)
	vector<double> tmp;

	// store here new point explored with its score
	vector<double> new_x(this->K, 1);
	double new_score;

	// random variable that set the threshold for accepting worse solutions
	uniform_real_distribution<double> acceptance_threshold(0,1);

	while(current_iteration <= this->max_iterations) {
		// round of search for current temperature
		std::cerr << "=====> "
			<< current_iteration << "/" << this->max_iterations
			<< " ==> temperature: " << this->temperature
			<< "==> jump num." << this->num_jump
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
				if (new_score < this->best_score) {
					this->best_score = new_score;
					best_x = x;
					this->worsening_steps = 0;
				}
				else{
					this->worsening_steps++;
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
