#include <map>
#include <vector>
#include "Node.h"

using namespace std;

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

vector<int>* Node::get_neighbour_ids() {
	return &this->neighbour_ids;
}

vector<int>* Node::get_packets_ids() {
	return &this->packets_ids;
}

void Node::add_neighbour(int other) {
	this->neighbour_ids.push_back(other);
}

void Node::add_packet(int packet_id) {
	// store only if packet was not in node
	bool contains_packet = false;
	for (int i : this->packets_ids) {
		if (packet_id == i) {
			contains_packet = true;
			break;
		}
	}
	if (!contains_packet) {
		this->packets_ids.push_back(packet_id);
	}
}

Packet::Packet(int origin_id, int packet_id) {
	this->origin_id = origin_id;
	this->packet_id = packet_id;
}

int Packet::get_origin_id() {
	return this->origin_id;
}

int Packet::get_id() {
	return this->packet_id;
}
