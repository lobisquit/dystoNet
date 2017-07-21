#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>

#include "soliton.h"
#include "binomial.h"
#include "second_problem.h"
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
	double acceptance_coefficient)
		: SimulatedAnnealing::SimulatedAnnealing(
			K,
			N,
			robust_soliton,
			max_failure_probability,
			starting_temperature,
			cooling_rate,
			max_iterations,
			steps_coefficient,
			acceptance_coefficient) {}

vector<double> JumpingBall::get_neighbour(vector<double> v, int worsening_steps) {
	// copy x to new array before perturbation
	vector<double> candidate(K, 1);

	// count number of steps taken that lead to a
	// solution worse than the current top
	do {
		// copy x to candidate
		candidate = v;

		// setup random variables
		uniform_real_distribution<double> perturbation(-50, 50);
		uniform_int_distribution<int> number_of_variations(0, this->K-1);
		uniform_int_distribution<int> index_choice(0, this->K-1);

		// if we were are not improving for too long, jump!
		if (worsening_steps>1000) {
			// perturb an uniformly distributed number of components
			for(int i=0; i<number_of_variations(this->rng); i++) {
				int chosen_d = index_choice(this->rng);
				candidate[chosen_d] = v[chosen_d] + perturbation(this->rng);
			}
		}
		else {
			// classic step if counter is too low
			uniform_real_distribution<double> perturbation(-50, 50);
			uniform_int_distribution<int> index_choice(0, K-1);
			int chosen_d = index_choice(rng);
			candidate[chosen_d] = v[chosen_d] + perturbation(rng);
		}
		double norm = 0;
		for(int j=1; j<=candidate.size(); j++){
			norm = norm + candidate[j-1];
		}
		for(int j=1; j<=candidate.size(); j++){
			candidate[j-1]=candidate[j-1]/norm;
		}
	} while (!respect_constraints(candidate));

	return candidate;
}

vector<double> JumpingBall::run_search() {
	vector<double> v = SecondProblem::get_initial_solution();

	// keep trace of how much jumps have been done since the beginning
	int current_iteration = 0;

	// save current best result in these variables
	vector<double> best_v(this->K, 1);
	this->best_score = numeric_limits<double>::infinity();

	// store here the best value of v when swapping old with new one (see below)
	vector<double> tmp;

	// store here new point explored with its score
	vector<double> new_v(this->K, 1);
	double new_score;

	// random variable that set the threshold for accepting worse solutions
	uniform_real_distribution<double> acceptance_threshold(0,1);

	int num_wrong_steps = 0;

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
			// search new candidate, save it to new_v
			new_v = get_neighbour(v, num_wrong_steps);

			if (this->best_score - objective_function(v) < 0) {
				num_wrong_steps++;
			}
			else {
				num_wrong_steps = 0;
			}

			// increment mean probability and counter when new_v is worse than v
			if(acceptance_probability(v, new_v) != 1){
				acceptance_mean += acceptance_probability(v, new_v);
				worsening_proposals++;
			}

			// accept or reject according to acceptance probability
			if( acceptance_probability(v, new_v) >= acceptance_threshold(rng)) {
				// subistitute v with new value (note that assignment with vector is
				// equivalent to a copy)
				tmp = v;
				v = new_v;
				new_v = tmp;

				new_score = objective_function(v);
				// update best result (up to now) if needed
				if (new_score < best_score) {
					this->best_score = new_score;
					best_v = v;
				}
			}
		}

		// report mean of acceptance probability up to now
		acceptance_mean = acceptance_mean / worsening_proposals;
		//cout << "mean of acceptance_probability = " << acceptance_mean << '\n';

		// update temperature for new round
		this->temperature = new_temperature();

		std::cout << temperature << '\n';
	}

	return best_v;
}