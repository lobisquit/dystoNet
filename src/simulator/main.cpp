#include "Network.h"
#include "Node.h"
#include <iostream>

int main() {
	int N = 2000;
	int K = 1000;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 15;
	double c = 0.01;
	double delta = 0.05;
	double seed = 6;

	/** Overhead coefficients */
	vector<double> x(K, 1.1);

	Distribution* d = new RobustSoliton(c, delta, K, seed);
	Network net = Network(N, K, len_x, len_y, neighThresh, d);

	int b0 = net.get_packets_size();
	std::cout << "b0 = " << b0 << '\n';
	net.spread_packets();
	cout << net << "\n";

	// for (Packet* p: net.get_packets()) {
	// 	std::cout << p-> << "\n";
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

	// net.collector();
}
