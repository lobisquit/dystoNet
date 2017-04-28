/*!
* \file
* Author: Enrico Lovisotto
*/

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
		* @returns valid point in solutions space
		*/
		double* get_initial_solution();

		/**
		* Check if a new possible solution respects problem constratins
		* @param candidate_x new candidate solution
		* @returns true if conditions are met, false otherwise
		*/
		bool respect_constraints(double* candidate_x);

		/**
		* Retrieve a suitable solution starting from the previous one
		* @returns valid point in solutions space
		*/
		double* get_neighbour(double* x);

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
		* @returns best solution found by the algorithm
		*/
		double* run_search();
};

class SimulatedAnnealing : public HeuristicSearch {
	public:
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
