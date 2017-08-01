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
	cout << "======> Building network\n";

	// setup random number generation
	this->rng = distribution->get_rng();

	this->N = N;
	this->K = K;

	uniform_real_distribution<double> x(0, len_x);
	uniform_real_distribution<double> y(0, len_y);

	// b is common to all sensing nodes
	this->b = this->N * distribution->expectation() / this->K;             // see (14) in Lin, Liao

	// initialize node and packet arrays
	this->nodes = new Node[N];
	this->packets = new Packet[this->get_packets_size()];

	// create K sensing nodes
	for(int node_id=0; node_id<this->N; node_id++) {
		int degree = distribution->realization();
		double pi = degree / ( this->N * distribution->expectation() ); // see (15) in Lin, Liao

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

	cout << "======> Network built \n";
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

void Network::spread_packets() {
	// perform a random walk for each packet
	for (int i = 0; i < this->get_packets_size(); i++) {
		cout
			<< "Spreading pkt " << (i + 1)
			<< "/" << this->get_packets_size() << "\r";

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
		}

		// add pkt to the final step of random walk
		node->add_packet(pkt);
	}
}
//
// void Network::collector(){
// 	vector<Node> nodes = this->get_nodes();
//
// 	vector<int> choices(this->N);
// 	for(int i=0;i<this->N;i++){
// 		choices[i] = i;
// 	}
// 	int h = 10;
// 	// for(int h = 1; h <= round(2*this->K); h++){
// 		random_shuffle(choices.begin(), choices.end());
// 		/** Definition and building of the encoding matrix */
// 		vector<vector<int>> en_matrix(h,vector<int>(this->K,0));
// 		/** Pick randomly h nodes from the network */
// 		for(int i=0;i<h;i++){
// 			/** Get packets of each visited node */
// 			vector<int>* packets_ids = this->nodes[choices[i]].get_packets_ids();
// 			/** Fill the encoding matrix */
// 			cout << "\n" << "Packets of node " << i << " : [";
// 			for(unsigned int j=0;j<packets_ids->size();j++){
// 				int original_packet = this->packets[packets_ids->at(j)].get_origin_id();
// 				cout << original_packet << " ";
// 				en_matrix[i][original_packet] = 1;
// 			}
// 			cout << "]";
// 		}
// 		// for(int i=0;i<h;i++){
// 		// 	cout << "\n";
// 		// 	for(unsigned int j=0;j<en_matrix[i].size();j++){
// 		// 		cout << en_matrix[i][j] << " ";
// 		// 	}
// 		// }
// 	// }
// }
