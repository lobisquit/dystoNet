#include <sstream>

#include "soliton.h"
#include "heuristicSearch.h"

#ifndef _FIRST_PROBLEM_H_
#define _FIRST_PROBLEM_H_

/**
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* \authors Enrico Lovisotto, Davide Peron, Federico Mason
*/

class FirstProblem : public HeuristicSearch {
	public:
		/** Random numbers generator */
		mt19937 rng;

		/** Number of sensor nodes */
		int K;

		/** Total number of nodes */
		int N;

		/** Maximal probability that a node receive less packets than we wanted */
		double max_failure_probability;

		/** Robust Soliton reference distribution */
		RobustSoliton* robust_soliton;

		FirstProblem(
			int K,
			int N,
			RobustSoliton* robust_soliton,
			double max_failure_probability);

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, FirstProblem &obj) {
			strm << "<FirstProblem("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<<
				")>";
			return strm;
		}

		/**
		* Computes the objective function for first problem. Optimizations in the calculation
		* are not necessary since is yet enough fast.
		* \param v Vector on which the objective function is computed.
		*/
		double objective_function(vector<double> x);

		/**
		* Create a random initial solution to start the searching algorithms.
		*	\return Initial random solution
		*/
		vector<double> get_initial_solution();

		/**
		* Check if the input vector respects the constraints required in Equation 13 (Lin's Paper)
		*/
		bool respect_constraints(vector<double> candidate_x);

		/**
		* Search for a neighbour solution, possible with a better objective function.
		* This function is implemented in lower classes
		* \return Neighbour solution
		*/
		vector<double> get_neighbour(vector<double> x);

		/**
		* Computes the acceptance probability used to choose a neighbour function
		* with a worst objective function.
		* This function is implemented in lower classes
		* \return Acceptance probability
		*/
		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		/**
		* Search an optimal solution for the problem starting from an initial random
		* solution and using an heuristic algorithm.
		* This function is completely different for each heuristic algorithm, so is
		* implemented in the correspondent algorithm's class.
		* \return Sub-optimal solution and its objective function
		*/
		vector<double> run_search(string progress_file_name);
};

#endif
