/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "heuristicSearch.h"

#ifndef _FIRST_THEORETIC_BOUND_H_
#define _FIRST_THEORETIC_BOUND_H_

class TheoreticBound : public HeuristicSearch {
	public:
		/** Number of sensor nodes */
		int K;

		/** Total number of nodes */
		int N;

		/** Maximal probability that a node receive less packets than we wanted */
		double max_failure_probability;

		/** Robust Soliton reference distribution */
		RobustSoliton* robust_soliton;

		TheoreticBound(	int _K,
						int _N,
						RobustSoliton* _robust_soliton,
						double _max_failure_probability);

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, TheoreticBound &obj) {
			strm << "<TheoreticBound("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<<
				")>";
			return strm;
		}

		double objective_function(double x[]);

		void get_initial_solution(double x[]);

		bool respect_constraints(double candidate_x[]);

		void get_neighbour(double x[], double new_x[]);

		double acceptance_probability(double old_x[], double new_x[]);

		void run_search(double best_x[]);
};

#endif
