/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "heuristicSearch.h"

#ifndef _FIRST_PROBLEM_H_
#define _FIRST_PROBLEM_H_

class FirstProblem : public HeuristicSearch {
	public:
		/** Number of sensor nodes */
		int K;

		/** Total number of nodes */
		int N;

		/** Maximal probability that a node receive less packets than we wanted */
		double max_failure_probability;

		/** Robust Soliton reference distribution */
		RobustSoliton* robust_soliton;

		FirstProblem(int _K,
						int _N,
						RobustSoliton* _robust_soliton,
						double _max_failure_probability);

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

		double objective_function(vector<double> x);

		vector<double> get_initial_solution();

		bool respect_constraints(vector<double> candidate_x);

		vector<double> get_neighbour(vector<double> x);

		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		vector<double> run_search();
};

#endif
