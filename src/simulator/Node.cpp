#include <map>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(
	double coordx,
	double coordy,
	int degree,
	double pi) {
		this->degree = degree;
		this->pi_d = pi_d;
		this->coordx = coordx;
		this->coordy = coordy;
}

void Node::set_neighbours(std::vector<Node*> neighbours){
	/**------------------- TO DO ------------------
	* Compute forwarding probability
	*/
	double forwarding_prob = 1/neighbours.size();
	for (int i = 0; i < neighbours.size(); i++){
		this->forwardingTable.insert(std::pair<Node*,double>(neighbours[i],forwarding_prob));
	}
}

// vector<Node*> Node::get_neighbours() {
// 	vector<Node*> nodes;
//
// 	// iterate over map entries
// 	map<Node*, double>::iterator iter = this->forwardingTable.begin();
// 	while(it != this->forwardingTable.end()) {
// 		// push key into vector
// 		nodes.push_back(iter->first);
//
// 		// jump to next one
// 		iter++;
// 	}
// 	return nodes;
// }

double Node::get_coordx(){
	return this->coordx;
}

double Node::get_coordy(){
	return this->coordy;
}

std::vector<Node*> Node::get_neighbours(){
	std::vector<Node*> neighbours;
	for (auto const& element : this->forwardingTable) {
		neighbours.push_back(element.first);
	}
	return neighbours;
}

// vector<Packet*> Node::getPackets(){
// 	return this->packets;
// }

SensingNode::SensingNode(
	double coordx,
	double coordy,
	int degree,
	double pi):
Node::Node(
		coordx,
		coordy,
		degree,
		pi) {
	/** --------------- COMPUTE B ---------------- */
	}

int SensingNode::get_b(){
	return this->b;
}

Packet::Packet(Node* origin, int id){
	this->origin = origin;
	this->id = id;
}

Node* Packet::get_origin(){
	return this->origin;
}

int Packet::get_id(){
	return this->id;
}
