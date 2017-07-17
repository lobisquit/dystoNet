#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(double x, double y, int degree, double pi) {
		this->degree = degree;
		this->pi_d = pi_d;
		this->x = x;
		this->y = y;
		vector<Packet> packets;
		this->packets = packets;
}

double Node::get_x() {
	return this->x;
}

double Node::get_y() {
	return this->y;
}

double Node::distance(Node other) {
	double delta_x = this->x - other.x;
	double delta_y = this->y - other.y;

	return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

vector<Packet> Node::getPackets() {
	return this->packets;
}

void Node::addPacket(Packet pkt) {
	this->packets.push_back(pkt);
}

vector<Node> Node::get_neighbours() {
	return this->neighbours;
}

SensingNode::SensingNode(
	double x,
	double y,
	int degree,
	double pi,
	double b): Node::Node(x, y, degree, pi) {
		this->b = b;
	}

double SensingNode::get_b() {
	return this->b;
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
