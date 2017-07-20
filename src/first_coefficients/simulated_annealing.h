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

class SimulatedAnnealing : public TheoreticBound {
	public:
		/** Random numbers generator */
		std::mt19937 rng;

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
		friend std::ostream& operator<<(std::ostream &strm, SimulatedAnnealing &obj) {
			strm << "<SimulatedAnnealing("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<< ", " <<
				"temperature=" 				<< obj.temperature 				<< ", " <<
				"cooling_rate=" 			<< obj.cooling_rate 			<< ", " <<
				"max_iterations=" 			<< obj.max_iterations 			<< ", " <<
				"steps_coefficient="		<< obj.steps_coefficient		<< ", " <<
				"acceptance_coefficient="	<< obj.acceptance_coefficient	<<
				")>";
			return strm;
		}

		SimulatedAnnealing(int _K,
							int _N,
							RobustSoliton* _robust_soliton,
							double _max_failure_probability,
							double _starting_temperature,
							double _cooling_rate,
							int _max_iterations,
							double _steps_coefficient,
							double _acceptance_coefficient);

		// these functions are taken as they are from upper class
		using TheoreticBound::objective_function;

		using TheoreticBound::respect_constraints;

		// these are implemented in cpp
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
