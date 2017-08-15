#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "genetic_algorithm.h"

using namespace std;

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 1);

	GeneticAlgorithm GA = GeneticAlgorithm(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* num_generations */ 80000,
		/* dim_population*/ 20,
		/* survival_rate */ 0.25
	);

	vector<double> no_redundancy(K, 1);

	std::cout << GA << "\n";

	vector<double> best_redundancy = GA.run_search();
		std::cout << "Arriving to score "
			<< (
				GA.objective_function(best_redundancy) / GA.objective_function(no_redundancy)
			) << "\n";
}
