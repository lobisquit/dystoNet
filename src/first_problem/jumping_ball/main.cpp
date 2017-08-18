#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "jumping_ball.h"
#include "functionCSV.h"

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 1);

	JumpingBall JB = JumpingBall(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* starting_temperature */ 100.0,
		/* cooling_rate */ 0.9,
		/* max_iterations */ 500000,
		/* steps_coefficient */ 5e4,
		/* acceptance_coefficient */ 200,
		/* max_worsening_steps */ 100
	);

	vector<double> no_redundancy(K, 1);

	std::cout << JB << "\n";

	vector<double> best_redundancy = JB.run_search();
	std::cout << "g1 = "
						<< (
								JB.objective_function(best_redundancy) /
								JB.objective_function(no_redundancy)
								) << "\n";

	// save result to CSV
	writeCSV(best_redundancy, "results/JB.csv");
}
