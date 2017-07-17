#include <iostream>
#include <vector>
#include <random>
#include <string>

using namespace std;

#ifndef _NODE_H_
#define _NODE_H_

class Packet;

class Node {
	protected:
		/** Coordinates of the node */
		double x;
		double y;

		/** Degree */
		int degree;

		/** \f$ \pi_d \f$ */
		double pi_d;

		/** Packets received by the node */
		vector<Packet> packets;

		/** forwardingTable towards each neighbours */
		vector<Node> neighbours;

	public:
		Node(double x, double y, int degree, double pi_d);

		/** Set neighbours in the network of current node */
		double get_x();
		double get_y();

		vector<Packet> getPackets();
		void addPacket(Packet pkt);

		vector<Node> get_neighbours();

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
				"degree=" << obj.degree << ")>";
			return strm;
		}
};

class SensingNode : public Node {
	private:
		/** Number of random walks starting from the node */
		double b;

	public:
		SensingNode(double x, double y, int degree, double pi, double b);
		double get_b();

		friend std::ostream& operator<<(std::ostream &strm, SensingNode &obj) {
			strm << "<SensingNode("
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

		friend std::ostream& operator<<(std::ostream &strm, Packet &obj) {
			strm << "<Packet("
				"origin=" << obj.origin << ", " <<
				"id="     << obj.id     << ")>";
			return strm;
		}
};

#endif
