#include "Network.h"
#include "Node.h"
#include "functionCSV.h"
#include <iostream>
#include "FC.h"

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
									 << argv[9] // problem to solve (EDFC or ADFC)
									 << "/"
									 << argv[10] // solutor employed to solve problem
									 << "-K=" << K
									 << "-N=" << N
									 << "-c=" << c
									 << "-delta=" << delta
									 << "-seed=" << seed
									 << ".csv";

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

	Network net = Network(N, K, len_x, len_y, neigh_threshold, d);

	net.spread_packets();

	Node* nodes = net.get_nodes();
	for (int i=0; i<net.get_nodes_size(); i++) {
		cout
			<< nodes[i]
			<< " -> "
			<< nodes[i].get_packets().size()
			<< "\n";
	}

	cout << "N = " << N << ", K = " << K << ", Ps = [";
	/** Number of times in which I pick randomly h nodes */
	int m = 10;
	/** Number of times I repeat the random process, to ensure the convergence,
	* take the mean of the set of taken measures */
	int t = 10;
	int ms, h, steps = 16;
	/** Compute delta to build the linspace */
	double delta_step = (2.5 - 1)/(steps - 1);
	for (int j = 0; j<steps; j++) {
		cout << "j= " << j << "\n";
		h = round(K * (1+j*delta_step));
		vector<vector<int>> en_matrix;
		/** Random number generator used in random_shuffle function */
		srand(time(0));
		double mean = 0;
		for(int z = 0; z < t; z++){
			cout << "z= " << z << "\r";
			ms = 0;
			for(int i = 0; i < m; i++){
				en_matrix = net.collector(h);
				ms += message_passing(h, en_matrix);
			}
			mean += (double)ms/m;
		}
		cout << mean/t << ", ";
	}
	cout << "]\n";
}
