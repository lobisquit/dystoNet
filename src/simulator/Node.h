#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <map>

#ifndef _NODE_H_
#define _NODE_H_

class Node {
	private:
		/** Coordinates of the node */
		double coordx;
		double coordy;

		/** Degree */
		int degree;

		/** \f$ \pi_d \f$ */
		double pi_d;

		/** Packets received by the node */
		std::vector<Packet*> packets;

		/** forwardingTable towards each neighbours */
		std::map<Node*, double> forwardingTable;

	public:
		Node(
			double coordx,
			double coordy,
			std::map<Node*, double> forwardingTable,
			int degree,
			double pi_d);

		double get_coordx();
		double get_coordy();
		std::vector<Node*> getNeighbours();
		std::vector<Packet*> getPackets();

};

class SensingNode : public Node{
	private:
		/** Number of random walks starting from the node */
		int b;

	public:
		SensingNode(
			double coordx,
			double coordy,
			std::vector<Node*> neighbours,
			int degree,
			double pi):
		Node::Node(
			coordx,
			coordy,
			neighbours,
			degree,
			pi);

		int get_b();
};

class Packet {
	private:
		/** Source of the packet */
		Node* origin;

		/** Unique id of the packet, since each source generate more than 1 packet */
		int id;

	public:

		Packet(Node* origin, int id);
		/** Getters */
		Node* getOrigin();
		int getId();
};

#endif
