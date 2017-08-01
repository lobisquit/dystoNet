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

	double b = this->N * distribution->expectation() / this->K;             // see (14) in Lin, Liao

	// create K sensing nodes
	for(int node_id=0; node_id<this->K; node_id++) {
		int degree = distribution->realization();
		double pi = degree / ( this->N * distribution->expectation() ); // see (15) in Lin, Liao

		this->nodes.push_back(Node(x(this->rng), y(this->rng), degree, pi));

		// note that this way b is taken as floor(b)
		for (int j=0; j<b; j++) {
			this->packets.push_back(Packet(node_id, j));
		}
	}

	// create N-K normal nodes
	for(int i=this->K; i<this->N; i++) {
		int degree = distribution->realization();
		double pi = degree / ( this->N * distribution->expectation() ); // see (15) in Lin, Liao

		this->nodes.push_back(Node(x(this->rng), y(this->rng), degree, pi));
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

	cout << "======> Network built \n";
}

vector<Node> Network::get_nodes() {
	return this->nodes;
}

vector<Packet> Network::get_packets() {
	return this->packets;
}

void Network::spread_packets() {
	// perform a random walk for each packet
	for (unsigned int i=0; i<this->packets.size(); i++) {
		cout << "Packet " << i << "/" << this->packets.size() << "\r";
		// add packet to sensing (source) node
		Node* node = &this->nodes[this->packets[i].get_origin_id()];
		node->add_packet(i);

		// randomly choose wheater to stop or proceed to one of the neighbours
		uniform_real_distribution<double> toss(0, 1);
		while (toss(this->rng) > node->get_pi()) {
			vector<int>* neighbours = node->get_neighbour_ids();

			// if we are proceeding, choose randomly a neighbour
			uniform_int_distribution<int> next(0, neighbours->size()-1);
			int chosen_neigh_id = neighbours->at(next(this->rng));

			// set it as next node in the trip and mark passage through it of pkt
			node = &this->nodes[chosen_neigh_id];
		}

		/** Check that packet has not been yet received from this node */
		// vector<int>* packets_ids = node->get_packets_ids();
		bool contains = false;
		// for(unsigned int j=0;j<packets_ids->size();j++){
		// 	if(this->packets[packets_ids->at(j)].get_origin_id() ==
		// 	this->packets[i].get_origin_id()){
		// 		contains = true;
		// 		break;
		// 	}
		// }
		if(!contains){
			node->add_packet(i);
		}
	}
}

void Network::collector(){
	vector<Node> nodes = this->get_nodes();

	vector<int> choices(this->N);
	for(int i=0;i<this->N;i++){
		choices[i] = i;
	}
	int h = 10;
	// for(int h = 1; h <= round(2*this->K); h++){
		random_shuffle(choices.begin(), choices.end());
		/** Definition and building of the encoding matrix */
		vector<vector<int>> en_matrix(h,vector<int>(this->K,0));
		/** Pick randomly h nodes from the network */
		for(int i=0;i<h;i++){
			/** Get packets of each visited node */
			vector<int>* packets_ids = this->nodes[choices[i]].get_packets_ids();
			/** Fill the encoding matrix */
			cout << "\n" << "Packets of node " << i << " : [";
			for(unsigned int j=0;j<packets_ids->size();j++){
				int original_packet = this->packets[packets_ids->at(j)].get_origin_id();
				cout << original_packet << " ";
				en_matrix[i][original_packet] = 1;
			}
			cout << "]";
		}
		// for(int i=0;i<h;i++){
		// 	cout << "\n";
		// 	for(unsigned int j=0;j<en_matrix[i].size();j++){
		// 		cout << en_matrix[i][j] << " ";
		// 	}
		// }
	// }
}
