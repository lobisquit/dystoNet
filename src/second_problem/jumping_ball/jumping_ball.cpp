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

individual JumpingBall::get_neighbour(individual old_individual) {
	// copy x to new array before perturbation
	vector<double> candidate = old_individual.values;

	individual new_individual;

	int first_d = 0;
	int second_d = 0;

	// count number of steps taken that lead to a
	// solution worse than the current top
	do {
		// setup random variables

		uniform_int_distribution<int> index_choice(0, this->K-1);

		uniform_int_distribution<int> number_of_variations(0, this->K-1);

		// if we were are not improving for too long, jump!
		if (this->worsening_steps > this->max_worsening_steps) {
			cerr << "JUMP" << '\n';
			while (!respect_constraints(candidate)) {
				for(int i=0; i<number_of_variations(this->rng); i++) {
					do {
						first_d = index_choice(rng);
						// start_d must have a non-zero probability
					} while(candidate[first_d] == 0);

					do {
						second_d = index_choice(rng);
						// end_d can't go beyond 1 after move
					} while(second_d == first_d || candidate[first_d] + candidate[second_d] > 1);

					uniform_real_distribution<double> perturbation(0, min(candidate[first_d], candidate[second_d]));

					double delta = perturbation(this->rng);

					candidate[first_d] += delta;
					candidate[second_d] -= delta;
				}
			}
			this->worsening_steps = 0;
		}
		else {
			// classic step if counter is too low
			do {
				first_d = index_choice(rng);
				// start_d must have a non-zero probability
			} while(candidate[first_d] == 0);

			do {
				second_d = index_choice(rng);
				// end_d can't go beyond 1 after move
			} while(second_d == first_d || candidate[first_d] + candidate[second_d] > 1);

			uniform_real_distribution<double> perturbation(0, min(candidate[first_d], candidate[second_d]));

			double delta = perturbation(this->rng);

			candidate[first_d] += delta;
			candidate[second_d] -= delta;
		}

	} while (!SecondProblem::respect_constraints(candidate));

	new_individual = update_objective_function(old_individual, candidate, first_d, second_d);

	return new_individual;
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

	individual new_individual;

	individual old_individual = approximate_objective_function(v);
	double old_score = old_individual.obj_function;

	while(current_iteration <= this->max_iterations) {
		// round of search for current temperature
		cerr << current_iteration << '\n';
		cerr << "=====> "
			<< current_iteration << "/" << this->max_iterations
			<< " ==> temperature: " << this->temperature
			<< " ==> Best score: " << this->best_score << "\n";

		// keep trace of mean value of acceptance_probability
		// when new point is worse (for better points it is 1)
		double acceptance_mean = 0;
		int worsening_proposals = 0;

		for(int i = 0; i < temperature_steps(); i++) {

			current_iteration++;

			new_individual = get_neighbour(old_individual);

			new_score = new_individual.obj_function;

			new_v = new_individual.values;

			double delta = new_score - old_score;

			// increment mean probability and counter when new_v is worse than v
			if ( acceptance_probability(delta) != 1 ) {
				acceptance_mean += acceptance_probability(delta);
				worsening_proposals++;
			}

			// accept or reject according to acceptance probability
			if ( acceptance_probability(delta) >= acceptance_threshold(rng) ) {
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
					<< " ==> g2 = " << g2 << "\n";

				// update best result (up to now) if needed
				if (new_score < best_score) {
					best_score = new_score;
					best_v = v;

					this->worsening_steps = 0;

					cerr << "=====> "
						<< current_iteration << "/" << this->max_iterations
						<< " ==> temperature = " << this->temperature
						<< " ==> score = " << new_score
						<< " ==> g2 = " << g2 << "\n";
				}
				else{
					this->worsening_steps++;
				}
			}
			// keep current score for the next cycle
			old_score = new_score;
			old_individual = new_individual;

		}

		// report mean of acceptance probability up to now
		acceptance_mean = acceptance_mean / worsening_proposals;
		//cout << "mean of acceptance_probability = " << acceptance_mean << '\n';

		// update temperature for new round
		this->temperature = new_temperature();
	}

	return best_v;
}
