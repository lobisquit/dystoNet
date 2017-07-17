#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(double coordx, double coordy, int degree, double pi) {
		this->degree = degree;
		this->pi_d = pi_d;
		this->coordx = coordx;
		this->coordy = coordy;
		vector<Packet*> packets;
		this->packets = packets;
}

double Node::get_coordx(){
	return this->coordx;
}

double Node::get_coordy(){
	return this->coordy;
}

vector<Packet*> Node::getPackets(){
	return this->packets;
}

void addPacket(Packet* pkt) {
	this->packets.push(pkt);
}

void Node::set_neighbours(vector<Node*> neighbours){
	/** Compute forwarding probability, according to Metropolis algorithm */
	double forwarding_prob = 1 / (double) neighbours.size();

	for (Node &node : neighbours) {
		this->forwardingTable.insert(
			pair<Node*, double>(node, forwarding_prob));
	}
}

vector<Node*> Node::get_neighbours(){
	vector<Node*> neighbours;
	for (Node const& element : this->forwardingTable) {
		neighbours.push_back(element.first);
	}
	return neighbours;
}


SensingNode::SensingNode(
	double coordx,
	double coordy,
	int degree,
	double pi,
	double b): Node::Node(coordx, coordy, degree, pi) {
		this->b = b;
	}

int SensingNode::get_b(){
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
