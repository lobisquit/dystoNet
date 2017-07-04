#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Node.h"

class Network {
	private:

		std::vector<Node> nodeList;

		/** Random numbers generator */
		std::mt19937 rng;

		Distribution* distribution;

	public:
		Network(int num, double x, double y);

		std::vector<Node> getNodeList();
		void describeNetwork();
};
