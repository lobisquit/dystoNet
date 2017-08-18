#include <map>
#include <vector>
#include "Node.h"
#include <array>
#include <algorithm>

using namespace std;

Node::Node() {
	this->degree = 0;
	this->pi = 1;
	this->x = 0;
	this->y = 0;
}

Node::Node(double x, double y, int degree, double pi) {
	this->degree = degree;
	this->pi = pi;
	this->x = x;
	this->y = y;
}

double Node::get_x() {
	return this->x;
}

double Node::get_y() {
	return this->y;
}

double Node::get_pi() {
	return this->pi;
}

double Node::distance(Node other) {
	double delta_x = this->x - other.x;
	double delta_y = this->y - other.y;

	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

vector<Node*> Node::get_neighbours() {
	return this->neighbours;
}

vector<Packet*> Node::get_packets() {
	return this->packets;
}

void Node::add_neighbour(Node* other) {
	this->neighbours.push_back(other);
}

void Node::add_packet(Packet* new_pkt) {
	Node* origin = new_pkt->get_origin();

	// store only if packet (or any of its duplicates
	// from same source) was not in node
	bool contains_packet = false;
	for (Packet* p : this->packets) {
		if (origin == p->get_origin()) {
			contains_packet = true;
			break;
		}
	}
	if (!contains_packet) {
		this->packets.push_back(new_pkt);
	}
}

void Node::filter_packets() {
	// continue only if received packets are more than degree, otherwise keep all
	if ( (unsigned int) this->degree <= this->packets.size() ) {
		// uniformly pick packets among available
		vector<int> picks( this->packets.size() );
		for(unsigned int i = 0; i < this->packets.size(); i++) {
			picks[i] = i;
		}
		random_shuffle(picks.begin(), picks.end());

		// remove others from package vector
		vector<Packet*> packets_kept;
		for (int i = 0; i < degree; i++) {
			packets_kept.push_back(this->packets[picks[i]]);
		}
		this->packets = packets_kept;
	}
}

Packet::Packet() {
	this->origin = NULL;
	this->packet_id = 0;
}

Packet::Packet(Node* origin, int packet_id) {
	this->origin = origin;
	this->packet_id = packet_id;
}

Node* Packet::get_origin() {
	return this->origin;
}

int Packet::get_id() {
	return this->packet_id;
}
