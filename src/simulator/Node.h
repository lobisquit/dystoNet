#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <map>

using namespace std;

#ifndef _NODE_H_
#define _NODE_H_

class Packet;

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
		vector<Packet*> packets;

		/** forwardingTable towards each neighbours */
		map<Node*, double> forwardingTable;

	public:
		Node(double coordx, double coordy, int degree, double pi_d);

		/** Set neighbours in the network of current node */
		double get_coordx();
		double get_coordy();

		vector<Packet*> getPackets();
		void addPacket(Packet* pkt);

		void set_neighbours(vector<Node*> neighbours);
		vector<Node*> get_neighbours();
};

class SensingNode : public Node {
	private:
		/** Number of random walks starting from the node */
		double b;

	public:
		SensingNode(double coordx, double coordy, int degree, double pi, double b);
		double get_b();
};

class Packet {
	private:
		/** Source of the packet */
		Node* origin;

		/** Unique id of the packet, since each source generate more than 1 packet */
		int id;

	public:
		Packet(Node* origin, int id);

		Node* get_origin();
		int get_id();
};

#endif
