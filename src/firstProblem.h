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

class TheoreticBound : public HeuristicSearch {
	public:
		/** problem specific parameters */
		int K;
		int N;
		double max_failure_probability;

		/** robust soliton reference distribution */
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
				"robust_soliton=" 			<< *obj.robust_soliton 			<< ", " <<
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


class SimulatedAnnealing : public TheoreticBound {
	public:
		/** random numbers generator */
		std::mt19937 rng;

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
				"max_iterations=" 			<< obj.max_iterations 			<<
				")>";
			return strm;
		}

		SimulatedAnnealing(	int _K,
							int _N,
							RobustSoliton* _robust_soliton,
							double _max_failure_probability,
							double _starting_temperature,
							double _cooling_rate,
							int _max_iterations);

		// function taken as they are from upper class
		using TheoreticBound::objective_function;

		using TheoreticBound::respect_constraints;

		void get_initial_solution(double x[]);

		void get_neighbour(double x[], double new_x[]);

		double acceptance_probability(double old_x[], double new_x[]);

		void run_search(double x[]);

		/**
		* Compute temperature for next round of search
		* @return temperature value
		*/
		double new_temperature();

		/**
		* Search steps to perform for current temperature value
		* @return real number whose floor is the number of steps
		*/
		double temperature_steps();
};

#endif
