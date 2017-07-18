#include "Network.h"
#include "soliton.h"

using namespace std;

Network::Network(
	int N,
	int K,
	double len_x,
	double len_y,
	double max_distance,
	Distribution* distribution) {

	// setup random number generation
	mt19937 rng = distribution->get_rng();

	uniform_real_distribution<double> x(0, len_x);
	uniform_real_distribution<double> y(0, len_y);

	// reserve space for all nodes
	this->nodes.reserve(N);

	// create K sensing nodes
	int P = 0;
	for(int i=0; i<K; i++) {
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao
		double b = N * distribution->exp() / K;           // see (14) in Lin, Liao

		// keep track of number of packets
		P += b;
		this->nodes.push_back(Node(x(rng), y(rng), degree, pi, b));
	}

	// create N-K normal nodes
	for(int i=K; i<N; i++) {
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao

		this->nodes.push_back(Node(x(rng), y(rng), degree, pi, 0));
	}

	// once all nodes are created, create the neighbors network
	// for (Node node : this->nodes) {
	for (int i=0; i<this->nodes.size(); i++) {
		Node* node = &this->nodes[i];
		std::cout << node << " @ " << *node << '\n';

		for (int j=0; j<this->nodes.size(); j++) {
			Node other = this->nodes[j];

			double distance = node->distance(other);
			if (distance == 0) {
				// std::cout << node << " close to " << other << '\n';
			}

			// distance == 0 is when other == node: case to avoid
			if (distance > 0 && distance <= max_distance) {
				// for (Node* n: node.get_neighbours()) {
				// 	std::cout << n << '\n';
				// }
				node->add_neighbour(&other);
			}
		}
		// for (Node* n : node.get_neighbours()) {
		// 	std::cout << *n << '\n';
		// }
	}

	// reserve space for all the packets
	// std::cout << P << '\n';
	this->packets.reserve(P);

	for (Node node : this->nodes) {
		// generate all packets of given node
		for (int id=0; id<node.get_b(); id++) {
			this->packets.push_back(Packet(&node, id));

			// retrieve pointer to packet just created and add it to node packets
			node.add_packet(&this->packets.back());
		}
	}
}

vector<Node> Network::get_nodes() {
	return this->nodes;
}
