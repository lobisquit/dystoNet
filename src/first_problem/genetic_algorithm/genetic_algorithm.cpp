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
									int dim_population)
										// constructor of upper class to trigger
										: FirstProblem::FirstProblem(
											K,
											N,
											robust_soliton,
											max_failure_probability) {
	//Genetic specific parameters
	this->num_generations = num_generations;
	this->dim_population = dim_population;
}

vector<double> GeneticAlgorithm::get_individual(){
	uniform_real_distribution<double> generator(1, 40);
	vector<double> individual(this->K,0);

	while(!respect_constraints(individual)){
		for(int j=0; j<this->K; j++){
			individual[j] = generator(rng);
		}
	}

	return individual;
}

vector<vector<double>> GeneticAlgorithm::get_initial_population() {
	vector<vector<double>> population(this->dim_population, vector<double>(this->K));
	for(int i = 0; i<this->dim_population; i++){
		population[i] = get_individual();
	}

	return population;
}

bool GeneticAlgorithm::sortByObjFunction(
	vector<double> individual_i, vector<double> individual_j){
	return objective_function(individual_i) > objective_function(individual_j);
}

vector<double> GeneticAlgorithm::run_search() {
	int generation = 0;
	vector<vector<double>> population = get_initial_population();

	/** Selection */
	/** fraction of individuals picked for the next generation */
	double fraction_rate = 0.25;
	int first_part = ceil(this->dim_population*fraction_rate);
	int chosen_d;
	uniform_real_distribution<double> mutation(-20, 20);
	uniform_int_distribution<int> index_choice(0, K-1);

	vector<double> no_redundancy(this->K,1);

	double b0 = objective_function(no_redundancy);

	while(generation < this->num_generations){
		/** Sorting of the population */
		std::sort(population.begin(), population.end(),
			[this](vector<double> s1, vector<double> s2) -> bool {
				return this->objective_function(s1) < this->objective_function(s2);
			});

		/** Best score for this generation, since vectors are sorted */
		cout << objective_function(population[1])/b0 << "\n";

		/** Copy of the best individuals in the whole population, and then perturbe it, checking you are respecting
		* constraints.
		*/
		for(int j = 1; j < round(1/fraction_rate); j++){
			for(int i = 0; i<first_part; i++){
				chosen_d = index_choice(rng);
				population[j*first_part+i] = population[i];

				vector<double> iterable = population[j*first_part+i];
				/** Mutation */
				do {
					iterable[chosen_d] = population[j*first_part+i][chosen_d] + mutation(rng);
				} while(!respect_constraints(iterable));

				population[j*first_part+i] = iterable;
			}
		}
		generation++;
	}
	return population[1];
}

vector<double> get_neighbour(vector<double> x) {
	throw invalid_argument("Not implemented for this class");
}

double acceptance_probability(vector<double> old_x, vector<double> new_x) {
	throw invalid_argument("Not implemented for this class");
}
