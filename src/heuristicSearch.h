/*!
* \file
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"

#ifndef _HEURISTIC_SEARCH_H_
#define _HEURISTIC_SEARCH_H_

/**
* This class interface describes in the most general way the
* algorithm family of heuristic search tecniques, whose aim is to find the minimum
* of a given objective function with some constraints on valid solutions.
*
* The meta-algorithm proposed here is inspired by procedure in figure 1 from
*
* S. Nahar, S. Sahni and E. Shragowitz, <i>Simulated Annealing and Combinatorial Optimization</i>,
* 23rd ACM/IEEE Design Automation Conference, 1986, pp. 293-299.
*/
class HeuristicSearch {
	public:

		/**
		* Compute objective function value in point x
		* @param x coordinate in solution space
		* @returns objective function computed in x
		*/
		double objective_function(double* x);

		/**
		* Retrieve an initial suitable starting point for heuristic search
		* @param x array to fill with solution
		*/
		void get_initial_solution(double* x);

		/**
		* Check if a new possible solution respects problem constratins
		* @param candidate_x new candidate solution
		* @returns true if conditions are met, false otherwise
		*/
		bool respect_constraints(double* candidate_x);

		/**
		* Retrieve a new suitable solution nearby current one
		* @param x valid point in solutions space
		* @param new_x array to fill with valid neighbour of x
		*/
		void get_neighbour(double* x, double* new_x);

		/**
		* Find the probability that the previous solution is replaced by a new
		* candidate solution
		* @param old_x previous solution found
		* @param new_x candidate next solution
		* @returns probability that new_x takes old_x place in the next step
		*/
		double acceptance_probability(double* old_x, double* new_x);

		/**
		* Try to find the valid solution that minimizes objective function
		* @param best_x array to fill with best point found by the algorithm
		*/
		void run_search(double* best_x);
};

class SimulatedAnnealing : public HeuristicSearch {
	public:
		/** random numbers generator */
		std::mt19937 my_rng;

		/** problem specific parameters */
		int K;
		int N;
		double max_failure_probability;

		/** robust soliton reference distribution */
		RobustSoliton* robust_soliton;

		/** specific parameters */
		double temperature;
		double cooling_rate;
		int max_iterations;

		/**
		* Custom overload of << operator, for printing debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, SimulatedAnnealing &obj) {
			strm << "<SimulatedAnnealing("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<< ", " <<
				"temperature=" 				<< obj.temperature 				<< ", " <<
				"cooling_rate=" 			<< obj.cooling_rate 			<< ", " <<
				"max_iterations=" 			<< obj.max_iterations
											<< ")>";
		}

		SimulatedAnnealing(	int _K,
							int _N,
							double _starting_temperature,
							double _cooling_rate,
							int _max_iterations,
							RobustSoliton* _robust_soliton,
							double _max_failure_probability);

		double objective_function(double* x);

		void get_initial_solution(double* x);

		bool respect_constraints(double* candidate_x);

		void get_neighbour(double* x, double* new_x);

		double acceptance_probability(double* old_x, double* new_x);

		void run_search(double* x);

		/**
		* Compute temperature for next round of search
		* @return temperature value
		*/
		double new_temperature();

		/**
		* Search steps to perform for current temperature value
		* @return temperature value
		*/
		double temperature_steps();
};

#endif
