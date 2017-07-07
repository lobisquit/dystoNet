#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Node.h"

class Network {
	private:

		/** List of nodes in the network */
		std::vector<Node> nodeList;

		/** Maximum distance of the neighbours of each node */
		double neighThresh;

		/** Random numbers generator */
		std::mt19937 rng;

		/** TO DO */
		// Distribution* distribution;

		void find_neigh();

	public:
		Network(int num, double x, double y, double neighThresh);

		std::vector<Node> getNodeList();
		void describeNetwork();
};
