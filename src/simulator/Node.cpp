#include <map>
#include <vector>
#include "Node.h"
#include <array>

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
