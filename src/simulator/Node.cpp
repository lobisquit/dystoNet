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

vector<int> Node::get_neighbour_ids() {
	return this->neighbour_ids;
}

void Node::add_neighbour(int other) {
	this->neighbour_ids.push_back(other);
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
