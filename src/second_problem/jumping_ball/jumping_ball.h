#include <sstream>

#include "soliton.h"
#include "second_problem.h"
#include "simulated_annealing.h"

#ifndef _JUMPING_BALL_H_
#define _JUMPING_BALL_H_

using namespace std;

class JumpingBall : public SimulatedAnnealing {
	public:

		double best_score;

		/**
		* Custom overload of << operator, to print debug info
		*/
		/*friend ostream& operator<<(ostream &strm, SimulatedAnnealing &obj) {
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

		JumpingBall(
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
		// these functions are taken as they are from upper class
		using SecondProblem::objective_function;
		using SecondProblem::respect_constraints;
		using SecondProblem::get_initial_solution;
		using SimulatedAnnealing::acceptance_probability;

		vector<double> run_search();
		vector<double> get_neighbour(vector<double> v, int worsening_steps);

		/**
		* Custom overload of << operator, to print debug info
		*/
		friend ostream& operator<<(ostream &strm, JumpingBall &obj) {
			strm << "<JumpingBall("
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
};

#endif
