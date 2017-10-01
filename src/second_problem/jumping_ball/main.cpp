#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "jumping_ball.h"
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

	JumpingBall JB = JumpingBall(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* starting_temperature */ 150.0,
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

	ostringstream file_name_stream;
	file_name_stream << "results/ADFC/JB"
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	writeCSV(best_redundancy, file_name_stream.str());
}
