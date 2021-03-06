#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>

using namespace std::chrono;

#include "soliton.h"
#include "genetic_algorithm.h"
#include "second_problem.h"

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(int K,
									int N,
									RobustSoliton* robust_soliton,
									double max_failure_probability,
									int num_generations,
									int dim_population,
									double survival_rate)
										// constructor of upper class to trigger
										: SecondProblem::SecondProblem(
											K,
											N,
											robust_soliton,
											max_failure_probability) {
	//Genetic specific parameters
	this->num_generations = num_generations;
	this->dim_population = dim_population;
	this->survival_rate = survival_rate;
}

vector<individual> GeneticAlgorithm::get_initial_population() {
	vector<individual> population(this->dim_population);
	for(int i = 0; i < this->dim_population; i++){
		vector<double> rnd_solution = SecondProblem::get_initial_solution();
		population[i] = this->approximate_objective_function(rnd_solution);
		cout
			<< "Created "
			<< i+1 << "/" << this->dim_population
			<< " elements \r";
	}
	cout << "\n";

	return population;
}

vector<double> GeneticAlgorithm::run_search(string progress_file_name) {
	int generation = 0;

	// prepare stream for output timing file
	ofstream progress_file;
	progress_file.open(progress_file_name);
	progress_file << "Time,score" << "\n";

	milliseconds begin_time
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	// remember how long the obj function has not improved significantly
	int worsening_counter = 0;

	vector<individual> population = get_initial_population();

	/** Selection */
	/** fraction of individuals picked for the next generation */
	int part_size = ceil(this->dim_population * this->survival_rate);

	uniform_int_distribution<int> index_choice(0, this->K-1);
	double E = robust_soliton->expectation();
	while(generation < this->num_generations){
		double old_obj_function = population[0].obj_function;

		/** Sorting of the population */
		std::sort(population.begin(), population.end(), by_obj_function());

		Distribution v_distribution = Distribution(population[0].values, 1);
		double current_obj_function = population[0].obj_function;
		cout << "=====> "
				 << generation << "/" << this->num_generations
				 << " ==> f = " << current_obj_function
				 << " ==> g2 = "
			/** Best score for this generation, since vectors are sorted */
				 << v_distribution.expectation() / E << "\n";

		/** Copy of the best individuals in the whole population, and then perturbe it, checking you are respecting
		* constraints.
		*/
		int first_d, second_d;
		for(int j = 1; j < round(1/this->survival_rate); j++){
			for(int i = 0; i<part_size; i++){
				individual old_individual = population[i];
				/** Mutation of */
				vector<double> candidate;
				do {
					candidate = population[i].values;
					do {
						first_d = index_choice(rng);
						// first_d must have a non-zero probability
					} while(candidate[first_d] == 0);
					do {
						second_d = index_choice(rng);
						// second_d can't go beyond 1 after move
					} while(second_d == first_d);
					// "move" some probability from start to end point
					uniform_real_distribution<double> mutation(0, min(candidate[first_d], candidate[second_d]));
					double delta = mutation(rng);
					candidate[first_d] -= delta;
					candidate[second_d] += delta;
				} while(!respect_constraints(candidate));

				// population[j*part_size+i] = this->approximate_objective_function(candidate);
				population[j*part_size+i] = this->update_objective_function(old_individual, candidate, first_d, second_d);
				// cout << "New obj function: " << population[j*part_size+i].obj_function << "\n";
			}
		}

		if (current_obj_function > old_obj_function - 0.00001) {
			worsening_counter++;
		}
		else {
			worsening_counter = 0;
		}
		if (worsening_counter > 1000) {
			break;
		}

		// save current best score in output file
		milliseconds current_time
			= duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		progress_file << (current_time - begin_time).count() << ","
									<< population[0].obj_function << "\n";

		generation++;
	}
	progress_file.close();

	return population[0].values;
}

vector<double> get_neighbour(vector<double> v) {
	throw invalid_argument("Not implemented for this class");
}

double acceptance_probability(vector<double> old_v, vector<double> new_v) {
	throw invalid_argument("Not implemented for this class");
}
