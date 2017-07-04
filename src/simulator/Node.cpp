#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(
	double coordx,
	double coordy,
	map<Node*, double> forwardingTable,
	int degree,
	double pi) {
		this.degree = degree;
		this.pi_d = pi_d;
		this.coordx = coordx;
		this.coordy = coordy;
		this.forwardingTable = forwardingTable;

}

vector<<Node*> getNeighbours() {
	vector<Node*> nodes;

	// iterate over map entries
	map<Node*, double>::iterator iter = this.forwardingTable.begin();
	while(it != this.forwardingTable.end()) {
		// push key into vector
		nodes.push_back(iter->first);

		// jump to next one
		iter++;
	}
	return nodes;
}

double Node::get_coordx(){
	return this.coordx;
}

double Node::get_coordy(){
	return this.coordy;
}

std::vector<Node*> Node::getNeighbours(){
	std::vector<Node*> neighbours;
	for (auto const& element : this.forwardingTable) {
		neighbours.push_back(element.first);
	}
	return neighbours;
}

SensingNode::SensingNode(
	double coordx,
	double coordy,
	map<Node*, double> forwardingTable,
	int degree,
	double pi):
Node::Node(
		coordx,
		coordy,
		neighbours,
		degree,
		pi) {
}

vector<Packet*> Node::getPackets(){
	return this.packets;
}

Packet::Packet(Node* origin, int id){
	this.origin = origin;
	this.id = id;
}

Node* get_origin(){
	return this.origin;
}

int get_id(){
	return this.id;
}
