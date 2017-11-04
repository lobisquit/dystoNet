/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "first_problem.h"

#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

using namespace std;

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

		vector<double> get_neighbour(vector<double> x);

		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		vector<double> run_search(string progress_file_name);
		/**
		* Create the first generation, with random values within the range of
		* the possible solution.
		*/
		vector<vector<double>> get_initial_population();
};

#endif
