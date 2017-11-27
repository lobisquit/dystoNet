#include <sstream>

#include "soliton.h"
#include "first_problem.h"

#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

using namespace std;

/*!
* Implements Genetic Algorithm to find a sub-optimal solution, here called \a individual,
* for the first problem.
*
* The main idea is to have always a set of possible solution, called \a population.
* At each iteration, a new \a generation of \a individuals is computed, i.e. best individuals
* are taken and mixed, to create new individuals.
* \authors Enrico Lovisotto, Davide Peron, Federico Mason
*/

class GeneticAlgorithm : public FirstProblem {
	public:
		/** Percentage of population that survives the next turn */
		double survival_rate;

		/** Number of generations created from starting population */
		int num_generations;

		/** Number of possible solutions in the population */
		int dim_population;

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend ostream& operator<<(ostream &strm, GeneticAlgorithm &obj) {
			strm << "<GeneticAlgorithm("
				"K="                        << obj.K                       << ", " <<
				"N="                        << obj.N                       << ", " <<
				"max_failure_probability="  << obj.max_failure_probability << ", " <<
				"robust_soliton="           << *obj.robust_soliton         << ", " <<
				"num_generations="          << obj.num_generations         <<
				")>";
			return strm;
		}

		GeneticAlgorithm(int K,
							int N,
							RobustSoliton* robust_soliton,
							double max_failure_probability,
							int num_generations,
							int dim_population,
							double survival_rate);

		// these functions are taken as they are from upper class
		using FirstProblem::objective_function;
		using FirstProblem::respect_constraints;
		using FirstProblem::get_initial_solution;

		/**
		* Search for a neighbour solution, possible with a better objective function.
		* \return Neighbour solution
		*/
		vector<double> get_neighbour(vector<double> x);

		/**
		* Computes the acceptance probability used to choose a neighbour function
		* with a worst objective function.
		* \return Acceptance probability
		*/
		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		/**
		* Search an optimal solution for the problem starting from an initial random
		* solution and using Genetic Algorithm.
		* \return Sub-optimal solution and its objective function
		*/
		vector<double> run_search(string progress_file_name);

		/**
		* Create the first generation, with random values within the range of
		* the possible solution.
		*/
		vector<vector<double>> get_initial_population();
};

#endif
