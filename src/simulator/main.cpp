#include "Network.h"
#include "Node.h"
#include <iostream>

int main() {
	int N = 100;
	int K = 20;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 15;
	double c = 0.01;
	double delta = 0.05;
	int seed = 1;

	/** Overhead coefficients */
	vector<double> x(K, 1.1);

	Distribution* d = new RobustSoliton(c, delta, K, seed);
	Network net = Network(N, K, len_x, len_y, neighThresh, d);

	int b0 = net.get_packets_size();
	std::cout << "b0 = " << b0 << '\n';
	net.spread_packets();
	// cout << net << "\n";

	// Node* nodes = net.get_nodes();
	// for (int i = 0; i < net.get_nodes_size(); i++) {
	// 	cout << nodes[i].get_packets().size() << "\n";
	// }
	//
	// Distribution* d1 = new OverheadRobustSoliton(x, c, delta, K, seed);
	// Network net1 = Network(N, K, len_x, len_y, neighThresh, d1);
	// int b = net1.get_packets_size();
	// std::cout << "b = " << b << '\n';
	//
	// std::cout << "g1 = " << (b / (double) b0) << "\n";
	//
	// net.spread_packets();
	// for (vector<int> row: en_matrix) {
	// 	int degree = 0;
	// 	for (int i: row) {
	// 		degree += i;
	// 		cout << i << ", ";
	// 	}
	// 	cout << "\n";
	// 	cout << "degree = " << degree;
	// 	cout << "\n";
	// }
	cout << "N = " << N << ", K = " << K << ", Ps = [";
	int ms, h, m=500, steps = 16;
	/** Compute delta to build the linspace */
	double delta_step = (2.5 - 1)/(steps - 1);
	for(int j = 0; j<steps; j++){
		ms = 0;
		h = round(K * (1+j*delta_step));
		vector<vector<int>> en_matrix;
		/** Random number generator used in random_shuffle function */
		srand(time(0));
		for(int i = 0; i < m; i++){
			en_matrix = net.collector(h);
			ms += net.message_passing(h, en_matrix);
		}

		cout << (double)ms/m << ", ";
	}
	cout << "]\n";

	// for (vector<int> row: en_matrix) {
	// 	int degree = 0;
	// 	for (int i: row) {
	// 		degree += i;
	// 		cout << i << ", ";
	// 	}
	// 	cout << "\n";
	// 	cout << "degree = " << degree;
	// 	cout << "\n";
	// }
}
