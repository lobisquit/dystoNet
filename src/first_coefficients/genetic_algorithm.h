/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "theoretic_bound.h"

#ifndef _GENETIC_ALGORITHM_H_
#define _GENETIC_ALGORITHM_H_

class GeneticAlgorithm : public TheoreticBound {
	public:
		/** Random numbers generator */
		std::mt19937 rng;

		/** Number of generations created from starting population */
		int num_generations;

		/** Number of possible solutions in the population */
		int dim_population;

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

		GeneticAlgorithm(	int _K,
							int _N,
							RobustSoliton* _robust_soliton,
							double _max_failure_probability,
							int _num_generations,
							int _dim_population);

		// these functions are taken as they are from upper class
		using TheoreticBound::objective_function;

		using TheoreticBound::respect_constraints;

		// these are implemented in cpp
		/**
		* Create the first generation, with random values within the range of
		* the possible solution.
		*/
		//void GeneticAlgorithm::get_initial_population(std::vector < std::vector<double> >* population);
		void get_initial_population(double** population);

		void get_individual(double individual[]);

		bool sortByObjFunction(double individual_i[], double individual_j[]);

		void run_search(double x[]);

};

#endif
