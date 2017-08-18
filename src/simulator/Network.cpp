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
	cout << "\n";

	// keep only "degree" packets for each node
	for (int i = 0; i < this->get_nodes_size(); i++) {
		Node* node = &this->nodes[i];

		node->filter_packets();
	}
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

bool Network::message_passing(int h, vector<vector<int>> en_matrix){
	vector<int> degrees(h);
	bool decoded, fail = false;

	while(fail == false){
		decoded = true;
		/** Compute degree of each packet in the matrix */
		for (int i=0; i<h; i++) {
			vector<int> row = en_matrix[i];
			int degree = 0;
			for (int j: row) {
				degree += j;
			}
			if(degree > 0){
				decoded = false;
			}
			degrees.at(i) = degree;
		}
		if(decoded == true){
			return true;
		}
		/** Simplify one-degree nodes */
		fail = true;
		for(int i=0;i<h;i++){
			if(degrees.at(i)==1){
				// cout << "\n One-degree at row " << i;
				fail = false;
				/** Simplify the packet of the one-degree node */
				vector<int> one_degree = en_matrix[i];
				for(unsigned int j=0;j<one_degree.size();j++){
					if(one_degree.at(j) == 1){
						// cout << " and column " << j;
						for(unsigned int row = 0; row < en_matrix.size(); row++){
								en_matrix[row][j] = 0;
						}
					}
				}
			}
		}
		// cout << "\n Encoding Matrix: \n";
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
	return false;
}
