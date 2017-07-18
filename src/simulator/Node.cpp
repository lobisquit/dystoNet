#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(double x, double y, int degree, double pi, double b) {
		this->degree = degree;
		this->pi_d = pi_d;
		this->x = x;
		this->y = y;
		this->b = b;
		vector<Packet*> packets;
		this->packets = packets;
}

double Node::get_x() {
	return this->x;
}

double Node::get_y() {
	return this->y;
}

double Node::get_b() {
	return this->b;
}

double Node::distance(Node other) {
	double delta_x = this->x - other.x;
	double delta_y = this->y - other.y;

	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

vector<Packet*> Node::get_packets() {
	return this->packets;
}

void Node::add_packet(Packet* pkt) {
	this->packets.push_back(pkt);
}

vector<Node*> Node::get_neighbours() {
	return this->neighbours;
}

void Node::add_neighbour(Node* other) {
	this->neighbours.push_back(other);
}

Packet::Packet(Node* origin, int id) {
	this->origin = origin;
	this->id = id;
}

Node* Packet::get_origin() {
	return this->origin;
}

int Packet::get_id() {
	return this->id;
}
