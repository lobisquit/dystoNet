#include "Network.h"
#include "Node.h"
#include <iostream>

int main() {
	int N = 200;
	int K = 100;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 5;
	double c = 0.01;
	double delta = 0.05;

	/** Overhead coefficients */
	vector<double> x(K, 1.1);

	Distribution* d = new RobustSoliton(c, delta, K, 10);
	Network net = Network(N, K, len_x, len_y, neighThresh, d);
	int b0 = net.get_packets().size();
	std::cout << "b0 = " << b0 << '\n';
	net.spread_packets();

	Distribution* d1 = new OverheadRobustSoliton(x, c, delta, K, 10);
	Network net1 = Network(N, K, len_x, len_y, neighThresh, d1);
	int b = net1.get_packets().size();
	std::cout << "b = " << b << '\n';

	std::cout << "g1 = " << (b / (double) b0) << "\n";
	std::cout << net << "\n";

	net.spread_packets();

	net.collector();
}
