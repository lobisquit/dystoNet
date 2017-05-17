#include <sstream>
#include <iostream>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <random>
#include <algorithm>

#include "soliton.h"
#include "first_genetic_algorithm.h"
#include "first_theoretic_bound.h"

void array_copy(double* x, double* y, int dim){
	for(int i = 0; i < dim; i++){
		x[i] = y[i];
	}
}

GeneticAlgorithm::GeneticAlgorithm(	int _K,
									int _N,
									RobustSoliton* _robust_soliton,
									double _max_failure_probability,
									int _num_generations,
									int _dim_population)
										// constructor of upper class to trigger
										: TheoreticBound::TheoreticBound(
											_K,
											_N,
											_robust_soliton,
											_max_failure_probability) {
	//Genetic specific parameters
	num_generations = _num_generations;
	dim_population = _dim_population;

	// random seed is set to a default value, for reproducibility
	rng.seed( time(NULL) );
	//rng.seed(1);
}

void GeneticAlgorithm::get_individual(double individual[]){
	std::uniform_real_distribution<double> generator(1, 40);

	while(!respect_constraints(individual)){
		for(int j=0; j<K; j++){
			individual[j] = generator(rng);
		}
	}
}

void GeneticAlgorithm::get_initial_population(double** population) {

	for(int i = 0; i<dim_population; i++){
		get_individual(population[i]);
	}
}

bool GeneticAlgorithm::sortByObjFunction(double individual_i[], double individual_j[]){
	return objective_function(individual_i) > objective_function(individual_j);
}

void GeneticAlgorithm::run_search(double x[]) {

	int generation = 0;
	/** Initialization */
	double** population;
	population = new double*[dim_population];
	for (int i = 0; i < dim_population; i++){
		population[i] = new double[K];
	}

	get_initial_population(population);

	/** Selection */
	/** fraction of individuals picked for the next generation */
	double fraction_rate = 0.25;
	int first_part = ceil(dim_population*fraction_rate);
	double score[dim_population];
	int chosen_d;
	std::uniform_real_distribution<double> mutation(-20, 20);
	std::uniform_int_distribution<int> index_choice(0, K-1);

	double x_farlocco[K];
	for(int i=0; i<K; i++) {
		x_farlocco[i] = 1;
	}

	double b0 = objective_function(x_farlocco);


	while(generation < num_generations){

		/** Sorting of the population */
		std::sort(population, population + dim_population,
			[this](double s1[], double s2[]) -> bool {
				return this->objective_function(s1) < this->objective_function(s2);
			});

		for(int i = 0; i<dim_population; i++){
			/** Compute the score with the actual population */
			score[i] = objective_function(population[i]);
		}
		std::cout << score[1]/b0 << "\n";

		/** Create the new generation */
		//double** new_generation;
		dim_population = first_part*round(1/fraction_rate);


		for(int j = 1; j < round(1/fraction_rate); j++){

			for(int i = 0; i<first_part; i++){
				chosen_d = index_choice(rng);
				array_copy(population[j*first_part+i], population[i], K);

				/** Mutation */
				do{
					population[j*first_part+i][chosen_d] = population[j*first_part+i][chosen_d] + mutation(rng);
				}while(!respect_constraints(population[j*first_part+i]));

			}
		}
		generation++;
	}

}
