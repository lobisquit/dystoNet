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

	// create K sensing nodes
	for(int node_id=0; node_id<K; node_id++) {
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao
		double b = N * distribution->exp() / K;           // see (14) in Lin, Liao

		this->nodes.push_back(Node(x(rng), y(rng), degree, pi, b));

		// note that this way b is taken as floor(b)
		for (int j=0; j<b; j++) {
			this->packets.push_back(Packet(node_id, j));
		}
	}

	// create N-K normal nodes
	for(int i=K; i<N; i++) {
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao

		this->nodes.push_back(Node(x(rng), y(rng), degree, pi, 0));
	}

	// once all nodes are created, create the neighbors network
	// for (Node node : this->nodes) {
	for (unsigned int i=0; i<this->nodes.size(); i++) {
		for (unsigned int j=0; j<this->nodes.size(); j++) {
			double distance = this->nodes[i].distance(this->nodes[j]);

			// distance == 0 is when other == node: case to avoid
			if (distance > 0 && distance <= max_distance) {
				this->nodes[i].add_neighbour(j);
			}
		}
	}
}

vector<Node> Network::get_nodes() {
	return this->nodes;
}

vector<Packet> Network::get_packets() {
	return this->packets;
}
