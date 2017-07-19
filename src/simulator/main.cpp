#include "Network.h"
#include "Node.h"
#include <iostream>

int main() {
	int N = 10;
	int K = 2;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 10;
	double c = 0.01;
	double delta = 0.05;
	Distribution d = RobustSoliton(c, delta, K, 0);

	Network net = Network(N, K, len_x, len_y, neighThresh, &d);
	net.spread_packets();

	std::cout << net << "\n";
}
