#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <vector>
#include <random>
#include <string>

using namespace std;

class Packet;

class Node {
	private:
		/** Coordinates of the node */
		double x;
		double y;

		/** Degree */
		int degree;

		/** \f$ \pi_d \f$ */
		double pi_d;

		/** Packets received by the node */
		vector<Packet*> packets;

		/** Neighbours of current node */
		vector<Node*> neighbours;

		/** Number of random walks starting from the node (= if node is not sensing)*/
		double b;

	public:
		Node(double x, double y, int degree, double pi_d, double b);

		double get_x();
		double get_y();
		double get_b();

		vector<Packet*> get_packets();
		void add_packet(Packet* pkt);

		vector<Node*> get_neighbours();

		void add_neighbour(Node* other);

		/**
		* Compute distance from another node
		* @param other node to check
		* @return distance from nodes
		*/
		double distance(Node other);

		friend std::ostream& operator<<(std::ostream &strm, Node &obj) {
			strm << "<Node("
				"x="      << obj.x      << ", " <<
				"y="      << obj.y      << ", " <<
				"b="      << obj.b      << ", " <<
				"degree=" << obj.degree << ")>";
			return strm;
		}
};

class Packet {
	private:
		/** Source of the packet */
		Node* origin;

		/**
		* Unique id of the packet, since each source
		* generate more than 1 packet
		*/
		int id;

	public:
		Packet(Node* origin, int id);

		Node* get_origin();
		int get_id();

		friend std::ostream& operator<<(std::ostream &strm, Packet obj) {
			strm << "<Packet("
				"origin=" << *obj.origin << ", " <<
				"id="     <<  obj.id     << ")>";
			return strm;
		}
};

#endif
