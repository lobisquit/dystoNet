#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <vector>
#include <random>
#include <string>

using namespace std;

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
		vector<int> neighbour_ids;

		/** packets ids received by current node */
		vector<int> packets_ids;

	public:
		Node(double x, double y, int degree, double pi);

		double get_x();
		double get_y();
		double get_pi();

		vector<int>* get_neighbour_ids();
		vector<int>* get_packets_ids();

		void add_neighbour(int other);
		void add_packet(int packet_id);

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

			strm << "neighbours=[";

			// print no more than MAX_LENGTH entries, otherwise ...
			unsigned int MAX_LENGTH = 15;
			for (unsigned int i=0; i<obj.neighbour_ids.size(); i++) {
				if (i<MAX_LENGTH) {
					strm << obj.neighbour_ids[i] << ", ";
				}
				else {
					strm << "...";
					break;
				}
			}
			strm << "], ";

			strm << "packets=[";

			// print no more than MAX_LENGTH entries, otherwise ...
			for (unsigned int i=0; i<obj.packets_ids.size(); i++) {
				if (i<MAX_LENGTH) {
					strm << obj.packets_ids[i] << ", ";
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
		int origin_id;

		/**
		* Unique id of the packet, since each source
		* generate more than 1 packet
		*/
		int packet_id;

	public:
		Packet(int origin_id, int id);

		int get_origin_id();
		int get_id();

		friend std::ostream& operator<<(std::ostream &strm, Packet obj) {
			strm << "<Packet("
				"origin_id=" << obj.origin_id << ", " <<
				"packet_id=" <<  obj.packet_id << ")>";
			return strm;
		}
};

#endif
