#include <iostream>
#include <fstream>

#include "functionCSV.h"
#include "Network.h"
#include "Node.h"
#include "FC.h"
#include "vector_utils.h"
#include "first_problem.h"
#include "second_problem.h"
#include "soliton.h"

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

	char* problem = argv[9]; // problem to solve (EDFC or ADFC)
	char* solver = argv[10]; // solver employed to solve problem

	ostringstream file_name_stream;
	file_name_stream << "results/"
									 << problem
									 << "/"
									 << solver
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

	// check if file exists
	ifstream infile(file_name_stream.str());
	if (!infile.good()) {
		cerr << "File not accessible!\n";
		exit(0);
	}

	// save to CSV file the setting for current run of simulator
	string summary_file_name = "results/simulator-configurations.csv";

	// check wheather file already exists or not
	ifstream summary_inspect(summary_file_name);
	bool file_exists = summary_inspect.good();
	summary_inspect.close();

	// actually open it in writing mode
	ofstream summary_file;
	if (file_exists) {
		cout << "____> append mode!\n";
		summary_file.open(summary_file_name, ofstream::app);
	}
	else {
		cout << "____> out mode!\n";
		summary_file.open(summary_file_name, ofstream::out);

		// if file was not already there, put titles
		summary_file
			<< "Problem, Solver, K, N, c, delta, g_1, g_2,"
			<< "\"Random walk length\", \"Number of packets\"\n";
	}

	summary_file << problem << ","
							 << solver << ","
							 << K << ","
							 << N << ","
							 << c << ","
							 << delta << ",";

	cout
		<< "... working on "
		<< file_name_stream.str()
		<< " - ";

	vector<double> x = readCSV(file_name_stream.str());

	Distribution* d = NULL;
	if (string(argv[9]).compare("EDFC") == 0) {
		d = new OverheadRobustSoliton(x, c, delta, K, seed);

		// distribution with no overhead
		vector<double> no_redundancy(K, 1);

		// print reference objective function
		FirstProblem problem = FirstProblem(
																				K,
																				N,
																				new RobustSoliton(c, delta, K, seed),
																				0.05);

		double g1 = problem.objective_function(x) / problem.objective_function(no_redundancy);

		summary_file << g1 << ",,";

		if (g1 > 10) {
			cout << "Score too bad, exiting! \n";

			// add empty columns for L_rw and n_pkt
			summary_file << ",\n";
			summary_file.close();
			exit(0);
		}
	}
	else if (string(argv[9]).compare("ADFC") == 0) {
		// optimal degree distribution
		d = new Distribution(x, seed);

		// reference degree distribution
		RobustSoliton* rs = new RobustSoliton(c, delta, K, seed);

		double g2 = d->expectation() / rs->expectation();
		summary_file << "," << g2 << ",";

		if (g2 > 10) {
			cout << "Score too bad, exiting! \n";

			// add empty columns for L_rw and n_pkt
			summary_file << ",\n";
			summary_file.close();
			exit(0);
		}
	}
	else {
		cerr << "Invalid problem: " << argv[9] << "\n";
	}

	// create Network and perform encoding
	Network net = Network(N, K, len_x, len_y, neigh_threshold, d);

	double random_walk_length = net.spread_packets();
	summary_file << random_walk_length << ",";

	// add to objective function number of packets spread in the network
	summary_file << net.get_packets_size();

	int number_of_etas = 10;
	int number_of_trials = 100;

	vector<double> decoding_probs;

	// loop through all eta values wanted, e.g. 10
	for (double eta: linspace(1, 2.5, number_of_etas)) {
		cerr << "testing eta = " << eta << "\n";
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
										 << "-solver=" << argv[10] // solver employed for the problem
										 << "-K=" << K
										 << "-N=" << N
										 << "-c=" << c
										 << "-delta=" << delta
										 << "-seed=" << seed
										 << ".csv";

	cout << "written file " << output_file_stream.str() << "\n";

	writeCSV(decoding_probs, output_file_stream.str());

	summary_file << "\n";
	summary_file.close();
}
