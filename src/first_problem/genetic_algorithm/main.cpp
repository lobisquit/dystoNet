#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include <sstream>
#include "soliton.h"
#include "genetic_algorithm.h"
#include "functionCSV.h"

using namespace std;

int main(int argc, char* argv[]) {
	// note that first argv is the program executable itself
	if (argc - 1 != 5) {
		cerr << "Usage: algorithm needs five parameters to run, while " << argc << " are given\n";
		return 1;
	}

	int K;
	int N;
	double c;
	double delta;
	int seed;

	istringstream sK(argv[1]);
	if (!(sK >> K)) { cerr << "Invalid K " << argv[1] << '\n'; }

	istringstream sN(argv[2]);
	if (!(sN >> N)) { cerr << "Invalid N " << argv[2] << '\n'; }

	istringstream sc(argv[3]);
	if (!(sc >> c)) { cerr << "Invalid c " << argv[3] << '\n'; }

	istringstream sdelta(argv[4]);
	if (!(sdelta >> delta)) { cerr << "Invalid delta " << argv[4] << '\n'; }

	istringstream sseed(argv[5]);
	if (!(sseed >> seed)) { cerr << "Invalid seed " << argv[5] << '\n'; }

	RobustSoliton rs = RobustSoliton(c, delta, K, seed);

	GeneticAlgorithm GA = GeneticAlgorithm(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* num_generations */ 100000,
		/* dim_population*/ 24,
		/* survival_rate */ 0.25
	);

	cout << GA << "\n";

	vector<double> no_redundancy(K, 1);
	vector<double> best_redundancy = GA.run_search();
	cout << "Arriving to score "
		<< (
			GA.objective_function(best_redundancy) / GA.objective_function(no_redundancy)
		) << "\n";
	cout << "x_d = [";

	// for(int i = 0; i < K; i++){
	// 	cout << best_redundancy[i] << ", ";
	// }
	// cout << "]\n";

	ostringstream file_name_stream;
	file_name_stream << "results/EDFC/GA"
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	writeCSV(best_redundancy, file_name_stream.str());
}
