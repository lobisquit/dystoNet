#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "simulated_annealing.h"
#include "functionCSV.h"

int main(int argc, char* argv[]) {
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

	SimulatedAnnealing SA = SimulatedAnnealing(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* starting_temperature */ 1000.0,
		/* cooling_rate */ 0.9,
		/* max_iterations */ 50000,
		/* steps_coefficient */ 5e3,
		/* acceptance_coefficient */ 200
	);

	std::cout << SA << "\n";

	ostringstream progress_name_stream;
	progress_name_stream << "results/ADFC-progress/SA"
											 << "-K=" << K
											 << "-N=" << N
											 << "-c=" << c
											 << "-delta=" << delta
											 << "-seed=" << seed
											 << ".csv";

	vector<double> best_redundancy = SA.run_search(progress_name_stream.str());

	ostringstream file_name_stream;
	file_name_stream << "results/ADFC/SA"
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	writeCSV(best_redundancy, file_name_stream.str());
}
