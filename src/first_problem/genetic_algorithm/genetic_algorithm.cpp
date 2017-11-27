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
using namespace std;

#include "soliton.h"
#include "genetic_algorithm.h"
#include "first_problem.h"

GeneticAlgorithm::GeneticAlgorithm(int K,
									int N,
									RobustSoliton* robust_soliton,
									double max_failure_probability,
									int num_generations,
									int dim_population,
									double survival_rate)
										// constructor of upper class to trigger
										: FirstProblem::FirstProblem(
											K,
											N,
											robust_soliton,
											max_failure_probability) {
	//Genetic specific parameters
	this->num_generations = num_generations;
	this->dim_population = dim_population;
	this->survival_rate = survival_rate;
}

vector<vector<double>> GeneticAlgorithm::get_initial_population() {
	vector<vector<double>>
		population(this->dim_population, vector<double>(this->K));
	for(int i = 0; i < this->dim_population; i++){
		population[i] = FirstProblem::get_initial_solution();
		cerr
			<< "Created "
			<< i+1 << "/" << this->dim_population
			<< " elements \r";
	}

	return population;
}

vector<double> GeneticAlgorithm::run_search(string progress_file_name) {
	/**
	* ------------------------
	* ### Algorithm
	*/
	int generation = 0;

	// prepare stream for output timing file
	ofstream progress_file;
	progress_file.open(progress_file_name);
	progress_file << "Time,score" << "\n";

	milliseconds begin_time
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	vector<vector<double>> population = get_initial_population();
	/** - Compute the number of individuals to keep from a generation to the other which is \f$ part\_size = \left \lceil{dim\_population \cdot survival\_rate}\right \rceil \f$ */
	int part_size = ceil(this->dim_population * this->survival_rate);

	uniform_real_distribution<double> mutation(-20, 20);
	uniform_int_distribution<int> index_choice(0, K - 1);

	vector<double> no_redundancy(this->K, 1);

	/**
	* - keep memory of best result of last iteration
	*/
	double current_g1 = 0;
	int worsening_counter = 0;

	while(generation < this->num_generations){
		current_g1 = this->objective_function(population[0]) / this->objective_function(no_redundancy);

		/**
		* - sorting of the population
		*/
		std::sort(population.begin(), population.end(),
			[this](vector<double> s1, vector<double> s2) -> bool {
				return this->objective_function(s1) < this->objective_function(s2);
			});

		/**
		* - copy the best individuals in the rest of the population, and then
		* perturb them checking constraints are still met.
		* To perturb an individual, an increasing number of perturbations are introducted,
		* the number of perturbations kept are those that takes to a lower objective function.
		*/
		for(int j = 1; j < round(1/this->survival_rate); j++){
			for(int i = 0; i < part_size; i++){
				population[j * part_size + i] = population[i];

				vector<double> candidate;

				do {
					candidate = population[j * part_size + i];

					int chosen_d_1 = index_choice(rng);
					int chosen_d_2 = index_choice(rng);
					int chosen_d_3 = index_choice(rng);

					candidate[chosen_d_1] = population[j * part_size + i][chosen_d_1] + mutation(rng);
					vector<double> candidate_1 = candidate;
					candidate[chosen_d_2] = population[j * part_size + i][chosen_d_2] + mutation(rng);
					vector<double> candidate_2 = candidate;
					candidate[chosen_d_3] = population[j * part_size + i][chosen_d_3] + mutation(rng);
					vector<double> candidate_3 = candidate;

					vector<vector<double>> first_selection;

					first_selection.push_back(candidate_1);
					first_selection.push_back(candidate_2);
					first_selection.push_back(candidate_3);

					std::sort(first_selection.begin(), first_selection.end(),
						[this](vector<double> f1, vector<double> f2) -> bool {
							return this->objective_function(f1) < this->objective_function(f2);
						});

					candidate=first_selection[0];

				} while(!respect_constraints(candidate));

				population[j * part_size + i] = candidate;
			}
		}

		/**
		 * - if improvement is too small for too many iterations, stop.
		 */
		double new_g1 = this->objective_function(population[0]) / this->objective_function(no_redundancy);
		if (new_g1 > current_g1 - 0.00001) {
			worsening_counter++;
		}
		else {
			worsening_counter = 0;
		}
		if (worsening_counter > 100) {
			break;
		}

		cerr << "=====> "
				 << generation << "/" << this->num_generations
				 << " ==> g1 = " << current_g1
				 << " ==> new_g1 = " << new_g1
				 << " ==> worsening_steps = " << worsening_counter << "\n";

		// save current best score in output file
		milliseconds current_time
			= duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		progress_file << (current_time - begin_time).count() << ","
									<< new_g1 << "\n";

		generation++;
	}
	progress_file.close();

	return population[0];
}

vector<double> get_neighbour(vector<double> x) {
	throw invalid_argument("Not implemented for this class");
}

double acceptance_probability(vector<double> old_x, vector<double> new_x) {
	throw invalid_argument("Not implemented for this class");
}
