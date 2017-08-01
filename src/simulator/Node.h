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

		/** \f$ \pi_d \f$ = probability that a random walk stops at current node */
		double pi;

		/** neighbour_ids id of current node */
		vector<Node*> neighbours;

		/** packets ids received by current node */
		vector<Packet*> packets;

	public:
		Node();

		Node(double x, double y, int degree, double pi);

		double get_x();
		double get_y();
		double get_pi();

		vector<Node*> get_neighbours();
		vector<Packet*> get_packets();

		void add_neighbour(Node* other);
		void add_packet(Packet* new_pkt);

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
				"degree=" << obj.degree << ", ";

			unsigned int MAX_LENGTH = 2;
			strm << "packets=[";
			for (unsigned int i=0; i<obj.packets.size(); i++) {
				if (i<MAX_LENGTH) {
					strm << obj.packets[i] << ", ";
				}
				else {
					strm << "...";
					break;
				}
			}
			return strm << "])>";
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
		int packet_id;

	public:
		Packet();

		Packet(Node* origin, int id);

		Node* get_origin();
		int get_id();

		friend std::ostream& operator<<(std::ostream &strm, Packet obj) {
			strm << "<Packet("
				"origin="    << *obj.origin    << ", " <<
				"packet_id=" <<  obj.packet_id << ")>";
			return strm;
		}
};

#endif
