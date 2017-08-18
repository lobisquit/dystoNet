#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "simulated_annealing.h"
#include "functionCSV.h"

int main() {
	int K = 20;
	int N = 100;
	int seed = 1;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.1,
		/* delta */ 0.5,
		            K,
		            seed);

	SimulatedAnnealing SA = SimulatedAnnealing(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* starting_temperature */ 150.0,
		/* cooling_rate */ 0.9,
		/* max_iterations */ 500000,
		/* steps_coefficient */ 5e4,
		/* acceptance_coefficient */ 200
	);

	vector<double> no_redundancy(K, 1);

	std::cout << SA << "\n";

	vector<double> best_redundancy = SA.run_search();
	std::cout << "Arriving to   score "
		<< (
			SA.objective_function(best_redundancy) / SA.objective_function(no_redundancy)
		) << "\n";

	// save result to CSV
	writeCSV(best_redundancy, "results/SA.csv");
}
