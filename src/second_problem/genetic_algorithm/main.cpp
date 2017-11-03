#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "genetic_algorithm.h"
#include "functionCSV.h"
#include <chrono>

using namespace std::chrono;
using namespace std;

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

	GeneticAlgorithm GA = GeneticAlgorithm(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* num_generations */ 2.5e5,
		/** ATTENTION!!!! In this version, dim_population has to be integer multiple
		* of the inverse of the survival_rate */
		/* dim_population*/ 20,
		/* survival_rate */ 0.25
	);

	std::cout << GA << "\n";

	milliseconds begin
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	vector<double> best_redundancy = GA.run_search();

	milliseconds end
		= duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	std::cout << "Time spent for EDFC GA = " << (end - begin).count() << "ms \n";

	ostringstream file_name_stream;
	file_name_stream << "results/ADFC/GA"
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	writeCSV(best_redundancy, file_name_stream.str());
}
