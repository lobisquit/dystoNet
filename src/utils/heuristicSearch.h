/*!
* \file
* Author: Enrico Lovisotto
*/
#include "soliton.h"

#ifndef _HEURISTIC_SEARCH_H_
#define _HEURISTIC_SEARCH_H_

using namespace std;

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
		double objective_function(vector<double> x);

		/**
		* Retrieve an initial suitable starting point for heuristic search
		* @param x array to fill with solution
		*/
		vector<double> get_initial_solution();

		/**
		* Check if a new possible solution respects problem constratins
		* @param candidate_x new candidate solution
		* @returns true if conditions are met, false otherwise
		*/
		bool respect_constraints(vector<double> candidate_x);

		/**
		* Retrieve a new suitable solution nearby current one
		* @param x valid point in solutions space
		* @param new_x array to fill with valid neighbour of x
		*/
		vector<double> get_neighbour(vector<double> x);

		/**
		* Find the probability that the previous solution is replaced by a new
		* candidate solution
		* @param old_x previous solution found
		* @param new_x candidate next solution
		* @returns probability that new_x takes old_x place in the next step
		*/
		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		/**
		* Try to find the valid solution that minimizes objective function
		* @param best_x array to fill with best point found by the algorithm
		*/
		vector<double> run_search(string progress_file_name);
};

#endif
