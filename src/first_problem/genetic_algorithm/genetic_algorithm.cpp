#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>
#include <algorithm>

#include "soliton.h"
#include "genetic_algorithm.h"
#include "first_problem.h"

using namespace std;

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

vector<individual> GeneticAlgorithm::get_initial_population() {
	vector<individual> population(this->dim_population);
	// vector<vector<double>>
	// 	population(this->dim_population, vector<double>(this->K));
	for(int i = 0; i < this->dim_population; i++){
		population[i].values = FirstProblem::get_initial_solution();
		population[i].obj_function = this->objective_function(population[i].values);
		cout
			<< "Created "
			<< i+1 << "/" << this->dim_population
			<< " elements \r";
	}
	cout << "\n";

	return population;
}

vector<double> GeneticAlgorithm::run_search() {
	int generation = 0;

	vector<individual> population = get_initial_population();
	/** Selection */
	/** fraction of individuals picked for the next generation */
	int part_size = ceil(this->dim_population * this->survival_rate);

	uniform_real_distribution<double> mutation(-20, 20);
	uniform_int_distribution<int> index_choice(0, K - 1);

	while(generation < this->num_generations){
		cout << generation << "/" << this->num_generations << "\n";
		/** Sorting of the population */
		// std::sort(population.begin(), population.end(),
		// 	[this](vector<double> s1, vector<double> s2) -> bool {
		// 		return this->objective_function(s1) < this->objective_function(s2);
		// 	});
		std::sort(population.begin(), population.end(), by_obj_function());

		/** Copy the best individuals in the population, and then
		* perturbe them checking constraints are still met.
		*/
		for(int j = 1; j < round(1/this->survival_rate); j++){
			for(int i = 0; i < part_size; i++){
				population[j * part_size + i] = population[i];

				individual candidate;

				/** Mutation of  */
				do {
					candidate = population[j * part_size + i];

					int chosen_d_1 = index_choice(rng);
					int chosen_d_2 = index_choice(rng);
					int chosen_d_3 = index_choice(rng);

					candidate.values[chosen_d_1] = population[j * part_size + i].values[chosen_d_1] + mutation(rng);
					candidate.obj_function = this->update_objective_function(candidate.values, population[j * part_size + i]);
					individual candidate_1 = candidate;
					candidate.values[chosen_d_2] = population[j * part_size + i].values[chosen_d_2] + mutation(rng);
					candidate.obj_function = this->update_objective_function(candidate.values, population[j * part_size + i]);
					individual candidate_2 = candidate;
					candidate.values[chosen_d_3] = population[j * part_size + i].values[chosen_d_3] + mutation(rng);
					candidate.obj_function = this->update_objective_function(candidate.values, population[j * part_size + i]);
					individual candidate_3 = candidate;

					vector<individual> first_selection;

					first_selection.push_back(candidate_1);
					first_selection.push_back(candidate_2);
					first_selection.push_back(candidate_3);

					std::sort(first_selection.begin(), first_selection.end(), by_obj_function());

					candidate=first_selection[0];

				} while(!update_respect_constraints(candidate,  population[j * part_size + i]));

				population[j * part_size + i] = candidate;
			}
		}
		generation++;
	}
	return population[0].values;
}

vector<double> get_neighbour(vector<double> x) {
	throw invalid_argument("Not implemented for this class");
}

double acceptance_probability(vector<double> old_x, vector<double> new_x) {
	throw invalid_argument("Not implemented for this class");
}
