/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "heuristicSearch.h"

#ifndef _SECOND_PROBLEM_H_
#define _SECOND_PROBLEM_H_

class SecondProblem : public HeuristicSearch {
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

		SecondProblem(
			int K,
			int N,
			RobustSoliton* robust_soliton,
			double max_failure_probability);

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, SecondProblem &obj) {
			strm << "<SecondProblem("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<<
				")>";
			return strm;
		}

		double objective_function(vector<double> v);
		vector<double> get_initial_solution();
		bool respect_constraints(vector<double> candidate_v);

		// functions to implement in lower classes
		vector<double> get_neighbour(vector<double> v);
		double acceptance_probability(vector<double> old_v, vector<double> new_v);
		vector<double> run_search();
};

#endif
