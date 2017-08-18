#include "Network.h"
#include "Node.h"
#include "functionCSV.h"
#include <iostream>

int main() {
	int N = 100;
	int K = 20;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 15;
	double c = 0.1;
	double delta = 0.5;
	int seed = 3;

	/** Overhead coefficients */
	vector<double> x = readCSV("results/GA.csv");
	for (double a: x) {
		cout << a << "\n";
	}
	// vector<double> x(K, 1);
	Distribution* d = new OverheadRobustSoliton(x, c, delta, K, seed);

	Network net = Network(N, K, len_x, len_y, neighThresh, d);

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
	int m = 1000;
	/** Number of times I repeat the random process, to ensure the convergence,
	* take the mean of the set of taken measures */
	int t = 20;
	int ms, h, steps = 16;
	/** Compute delta to build the linspace */
	double delta_step = (2.5 - 1)/(steps - 1);
	for (int j = 0; j<steps; j++) {
		h = round(K * (1+j*delta_step));
		vector<vector<int>> en_matrix;
		/** Random number generator used in random_shuffle function */
		srand(time(0));
		double mean = 0;
		for(int z = 0; z < t; z++){
			ms = 0;
			for(int i = 0; i < m; i++){
				en_matrix = net.collector(h);
				ms += net.message_passing(h, en_matrix);
			}
			mean += (double)ms/m;
		}
		cout << mean/t << ", ";
	}
	cout << "]\n";
}
