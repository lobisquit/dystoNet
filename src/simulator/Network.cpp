#include<algorithm>
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
	cerr << "======> Building network\n";

	// setup random number generation
	this->rng = distribution->get_rng();

	this->N = N;
	this->K = K;

	uniform_real_distribution<double> x(0, len_x);
	uniform_real_distribution<double> y(0, len_y);

	// b is common to all sensing nodes, see (14) in Lin, Liao
	this->b = this->N * distribution->expectation() / this->K;

	// initialize node and packet arrays
	this->nodes = new Node[N];
	this->packets = new Packet[this->get_packets_size()];

	// create K sensing nodes
	for(int node_id=0; node_id<this->N; node_id++) {
		int degree = distribution->realization();

		// for pi formula, see (15) in Lin, Liao
		double pi = degree / ( this->N * distribution->expectation() );

		this->nodes[node_id] = Node(x(this->rng), y(this->rng), degree, pi);

		if (node_id < this->K) {
			// note that this way b is taken as floor(b)
			for (int j = 0; j < b; j++) {
				this->packets[node_id * b + j] = Packet(&this->nodes[node_id], j);
			}
		}
	}

	// once all nodes are created, create the neighbors network
	// for (Node node : this->nodes) {
	for (int i=0; i < N; i++) {
		for (int j=0; j < N; j++) {
			double distance = this->nodes[i].distance(this->nodes[j]);

			// distance == 0 is when other == node: case to avoid
			if (distance > 0 && distance <= max_distance) {
				this->nodes[i].add_neighbour(&this->nodes[j]);
			}
		}
	}

	cerr << "======> Network built \n";
}

Node* Network::get_nodes() {
	return this->nodes;
}

Packet* Network::get_packets() {
	return this->packets;
}

int Network::get_packets_size() {
	return this->K * this->b;
}

int Network::get_nodes_size() {
	return this->N;
}

double Network::spread_packets() {
	// save here average length of random walks
	double walks_length = 0;

	// perform a random walk for each packet
	for (int i = 0; i < this->get_packets_size(); i++) {
		cerr
			<< "Spreading pkt " << (i + 1)
			<< "/" << this->get_packets_size() << "\n";

		// add packet to sensing (source) node
		Packet* pkt = &this->packets[i];
		Node* node = pkt->get_origin();

		// randomly choose wheater to stop or proceed to one of the neighbours
		uniform_real_distribution<double> toss(0, 1);
		while (toss(this->rng) > node->get_pi()) {
			vector<Node*> neighbours = node->get_neighbours();

			// if we are proceeding, choose randomly a neighbour
			uniform_int_distribution<int> next(0, neighbours.size()-1);

			// set random neighbour as next node in the trip
			node = neighbours[next(this->rng)];

			// increment walks length
			walks_length++;
		}

		// add pkt to the final step of random walk
		node->add_packet(pkt);
	}
	cerr << "\n";

	// keep only "degree" packets for each node
	for (int i = 0; i < this->get_nodes_size(); i++) {
		Node* node = &this->nodes[i];
		node->filter_packets();
	}

	// return average length of random walk of each packet
	return walks_length / this->get_packets_size();
}

vector<vector<int>> Network::collector(int h) {
	/** Pick randomly h nodes from the network */
	vector<int> visits(this->N);
	for(int i = 0; i < this->N; i++) {
		visits[i] = i;
	}

	random_shuffle(visits.begin(), visits.end());

	/** Definition and building of the encoding matrix */
	vector<vector<int>> en_matrix(h, vector<int>(this->K, 0));

	// iter through visited nodes
	for (int i = 0; i < h && i < this->get_nodes_size(); i++) {
		Node* node = &this->nodes[visits[i]];

		// retrieve origin of each packet and its position in nodes array
		for (Packet* pkt: node->get_packets()) {
			Node* origin = pkt->get_origin();

			// find origin id in nodes array of pointers
			int origin_position = -1;
			for (int j = 0; j < this->get_nodes_size(); j++) {
				// NOTE that this checks ignores automatically duplicates from same origin
				if (origin == &this->nodes[j]) {
					origin_position = j;
				}
			}

			// mark packet reception in node i from node origin in matrix
			if (origin_position != -1) {
				en_matrix[i][origin_position] = 1;
			}
			else {
				cerr << "--->ERROR<--- Packet " << *pkt << "not in Network" << "\n";
			}
		}
	}
	return en_matrix;
}
