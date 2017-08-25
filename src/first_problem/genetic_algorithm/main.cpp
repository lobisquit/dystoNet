#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "genetic_algorithm.h"
#include "functionCSV.h"

using namespace std;

int main() {
	int K = 20;
	int N = 100;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 9);

	GeneticAlgorithm GA = GeneticAlgorithm(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* num_generations */ 1500,
		/* dim_population*/ 24,
		/* survival_rate */ 0.25
	);

	vector<double> no_redundancy(K, 1);

	cout << GA << "\n";

	vector<double> best_redundancy = GA.run_search();
	cout << "Arriving to score "
		<< (
			GA.objective_function(best_redundancy) / GA.objective_function(no_redundancy)
		) << "\n";
	cout << "x_d = [";
	for(int i = 0; i < K; i++){
		cout << best_redundancy[i] << ", ";
	}
	cout << "]\n";

	// save result to CSV
	writeCSV(best_redundancy, "results/EDFC/GA.csv");
}
