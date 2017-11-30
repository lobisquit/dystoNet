#include <sstream>

#include "soliton.h"
#include "second_problem.h"
#include "simulated_annealing.h"

#ifndef _JUMPING_BALL_H_
#define _JUMPING_BALL_H_

using namespace std;

class JumpingBall : public SimulatedAnnealing {
	public:
		// Counter of consecutive perturbations without improvements
		int worsening_steps;
		// Limit of consecutive perturbations without improvements
		int max_worsening_steps;
		// The best result obtained so far
		double best_score;

		JumpingBall(
			int K,
			int N,
			RobustSoliton* robust_soliton,
			double max_failure_probability,
			double starting_temperature,
			double cooling_rate,
			int max_iterations,
			double steps_coefficient,
			double acceptance_coefficient,
			int max_worsening_steps);

		// these functions are taken as they are from upper class
		using SecondProblem::objective_function;
		using SecondProblem::respect_constraints;
		using SecondProblem::get_initial_solution;
		using SimulatedAnnealing::acceptance_probability;

		// these functions are implemented in cpp

		vector<double> run_search(string progress_file_name);
		individual get_neighbour(individual old_individual);

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
