#include "Network.h"
#include "Node.h"
#include "functionCSV.h"
#include <iostream>
#include "FC.h"
#include "vector_utils.h"

int main(int argc, char* argv[]) {
	int K;
	int N;
	double c;
	double delta;
	int seed;
	double len_x;
	double len_y;
	double neigh_threshold;

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

	istringstream slen_x(argv[6]);
	if (!(slen_x >> len_x)) { cerr << "Invalid len_x " << argv[6] << '\n'; }

	istringstream slen_y(argv[7]);
	if (!(slen_y >> len_y)) { cerr << "Invalid K " << argv[7] << '\n'; }

	istringstream sneigh_threshold(argv[8]);
	if (!(sneigh_threshold >> neigh_threshold)) { cerr << "Invalid neigh_treshold " << argv[8] << '\n'; }

	ostringstream file_name_stream;
	file_name_stream << "results/"
									 << argv[9]  // problem to solve (EDFC or ADFC)
									 << "/"
									 << argv[10] // solutor employed to solve problem
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	cout
		<< "... working on "
		<< file_name_stream.str()
		<< "\n";

	vector<double> x = readCSV(file_name_stream.str());

	Distribution* d = NULL;
	if (string(argv[9]).compare("EDFC") == 0) {
	  d = new OverheadRobustSoliton(x, c, delta, K, seed);
	}
	else if (string(argv[9]).compare("ADFC") == 0) {
	  d = new Distribution(x, seed);
	}
	else {
		cerr << "Invalid problem: " << argv[9] << "\n";
	}

	// avoid very bad solutions, i.e. distribution where the
	// expected number of packets from each node is too high

	if (d->expectation() > 10 || 1) {
		cout
			<< "Solution provided by "
			<< file_name_stream.str()
			<< " is suboptimal! E[d] = "
			<< d->expectation()
			<< "\n";
		exit(0);
	}

	// create Network and perform encoding
	Network net = Network(N, K, len_x, len_y, neigh_threshold, d);
	net.spread_packets();

	int number_of_etas = 10;
	int number_of_trials = 100;

	vector<double> decoding_probs;

	// loop through all eta values wanted, e.g. 10
	for (double eta: linspace(1, 2.5, number_of_etas)) {
		double decoding_prob = 0;

		vector<vector<int>> en_matrix;
		unsigned int h = (unsigned int) K * eta;

		// repeat process many times, to compute successful probability
		for (int i=0; i<number_of_trials; i++) {
			// collect packets from h nodes randomly across N
			en_matrix = net.collector(h);
			decoding_prob += message_passing(h, en_matrix) ? 1 : 0;
		}
		decoding_prob /= number_of_trials;

		// add probability to output vector
		decoding_probs.push_back(decoding_prob);
	}

	ostringstream output_file_stream;
	output_file_stream << "results/simulator/"
									 << "etas"
									 << "-problem=" << argv[9]  // problem to solve (EDFC or ADFC)
									 << "-solutor=" << argv[10] // solutor employed to solve problem
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	writeCSV(decoding_probs, output_file_stream.str());
}
