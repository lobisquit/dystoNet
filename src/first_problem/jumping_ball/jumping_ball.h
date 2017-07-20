#include <sstream>

#include "soliton.h"
#include "first_problem.h"

#ifndef _FIRST_JUMPING_BALL_H_
#define _FIRST_JUMPING_BALL_H_

class JumpingBall : public TheoreticBound {
	public:
		/** Random numbers generator */
		std::mt19937 rng;

		/** Current system temperature */
		double temperature;
		/** Number of attempts with temperature value \f$ T \f$ is computed as
		* \f$ \frac{steps\_coefficient}{T} \f$
		*/
		double steps_coefficient;

		/** After round with temperature \f$ T \f$, next one has temperature
		* \f$ cooling\_rate \cdot T \f$
		*/
		double cooling_rate;

		/** Maximal numbers of steps taken from starting point */
		int max_iterations;

		int minkiata;

		/**
		* Custom overload of << operator, to print debug info
		*/
		/*friend std::ostream& operator<<(std::ostream &strm, SimulatedAnnealing &obj) {
			strm << "<SimulatedAnnealing("
				"K="						<< obj.K 						<< ", " <<
				"N="						<< obj.N 						<< ", " <<
				"max_failure_probability=" 	<< obj.max_failure_probability 	<< ", " <<
				"robust_soliton=" 			<< *obj.robust_soliton 			<< ", " <<
				"temperature=" 				<< obj.temperature 				<< ", " <<
				"cooling_rate=" 			<< obj.cooling_rate 			<< ", " <<
				"max_iterations=" 			<< obj.max_iterations 			<< ", " <<
				"steps_coefficient="		<< obj.steps_coefficient		<<
				")>";
			return strm;
		}*/

		JumpingBall(	int _K,
							int _N,
							RobustSoliton* _robust_soliton,
							double _max_failure_probability,
							double _starting_temperature,
							double _cooling_rate,
							int _max_iterations,
							double _steps_coefficient);

		// these functions are taken as they are from upper class
		using TheoreticBound::objective_function;

		using TheoreticBound::respect_constraints;

		// these are implemented in cpp
		void get_initial_solution(double x[]);

		void get_neighbour(double x[], double new_x[], double best_score);

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
