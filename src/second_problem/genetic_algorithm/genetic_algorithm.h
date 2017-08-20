/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "second_problem.h"

#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

using namespace std;

struct by_obj_function {
	bool operator()(individual const &a, individual const &b) {
		return a.obj_function < b.obj_function;
	}
};

class GeneticAlgorithm : public SecondProblem {
	public:
		/** Random numbers generator */
		std::mt19937 rng;

		/** Number of generations created from starting population */
		int num_generations;

		/** Number of possible solutions in the population */
		int dim_population;

		/** Percentage of population that survives the next turn */
		double survival_rate;

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, GeneticAlgorithm &obj) {
			strm << "<GeneticAlgorithm("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<< ", " <<
				"num_generations=" 			<< obj.num_generations 			<<
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
		using SecondProblem::objective_function;
		using SecondProblem::respect_constraints;
		using SecondProblem::get_initial_solution;

		vector<double> get_neighbour(vector<double> v);
		double acceptance_probability(vector<double> old_v, vector<double> new_v);

		/**
		* Create the first generation, with random values within the range of
		* the possible solution.
		*/
		vector<individual> get_initial_population();

		vector<double> get_individual();

		vector<double> run_search();
};

#endif
