#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(double x, double y, int degree, double pi) {
		this->degree = degree;
		this->pi_d = pi_d;
		this->x = x;
		this->y = y;
		vector<Packet*> packets;
		this->packets = packets;
}

double Node::get_x(){
	return this->x;
}

double Node::get_y(){
	return this->y;
}

vector<Packet*> Node::getPackets(){
	return this->packets;
}

void Node::addPacket(Packet* pkt) {
	this->packets.push_back(pkt);
}

void Node::set_neighbours(vector<Node*> neighbours){
	/** Compute forwarding probability, according to Metropolis algorithm */
	double forwarding_prob = 1 / (double) neighbours.size();

	for (Node *node : neighbours) {
		this->forwardingTable.insert(
			pair<Node*, double>(node, forwarding_prob));
	}
}

vector<Node*> Node::get_neighbours(){
	vector<Node*> neighbours;
	for (auto const& entry : this->forwardingTable) {
		neighbours.push_back(entry.first);
	}
	return neighbours;
}


SensingNode::SensingNode(
	double x,
	double y,
	int degree,
	double pi,
	double b): Node::Node(x, y, degree, pi) {
		this->b = b;
	}

double SensingNode::get_b(){
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
