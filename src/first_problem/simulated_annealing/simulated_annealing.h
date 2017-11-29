/*!
* \file
* Provide solution methods for Lin's paper problem in equation 13
* (first problem in our study)
* Author: Enrico Lovisotto
*/
#include <sstream>

#include "soliton.h"
#include "first_problem.h"

#ifndef _SIMULATED_ANNEALING_H_
#define _SIMULATED_ANNEALING_H_

using namespace std;

class SimulatedAnnealing : public FirstProblem {
	public:
		/** Random numbers generator */
		mt19937 rng;

		/** Current system temperature */
		double temperature;

		/** Number of attempts with temperature value \f$ T \f$ is computed as
		*	\f$ \frac{steps\_coefficient}{T} \f$ */
		double steps_coefficient;

		/** Parameter that adjusts probability of accepting a worse point
		*	see #acceptance_probability */
		double acceptance_coefficient;

		/** After round with temperature \f$ T \f$, next one has temperature
		*	\f$ cooling\_rate \cdot T \f$ */
		double cooling_rate;

		/** Maximal numbers of steps taken from starting point */
		int max_iterations;

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend ostream& operator<<(ostream &strm, SimulatedAnnealing &obj) {
			strm << "<SimulatedAnnealing("
				"K="                       << obj.K                       << ", " <<
				"N="                       << obj.N                       << ", " <<
				"max_failure_probability=" << obj.max_failure_probability << ", " <<
				"robust_soliton="          << *obj.robust_soliton         << ", " <<
				"temperature="             << obj.temperature             << ", " <<
				"cooling_rate="            << obj.cooling_rate            << ", " <<
				"max_iterations="          << obj.max_iterations          << ", " <<
				"steps_coefficient="       << obj.steps_coefficient       << ", " <<
				"acceptance_coefficient="  << obj.acceptance_coefficient  <<
				")>";
			return strm;
		}

		SimulatedAnnealing(
			int K,
			int N,
			RobustSoliton* robust_soliton,
			double max_failure_probability,
			double starting_temperature,
			double cooling_rate,
			int max_iterations,
			double steps_coefficient,
			double acceptance_coefficient);

		// these functions are taken as they are from upper class
		using FirstProblem::objective_function;
		using FirstProblem::respect_constraints;

		// this is implemented in cpp
		vector<double> get_neighbour(vector<double> x);

		double acceptance_probability(vector<double> old_x, vector<double> new_x);

		vector<double> run_search(string progress_file_name);

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
