#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Node.h"
#include "soliton.h"

using namespace std;

class Network {
	private:
		/** List of nodes in the network */
		vector<Node> nodes;
		vector<Packet> packets;

		/** Random numbers generator */
		mt19937 rng;

	public:
		/**
		* Create a network of nodes
		* @param N number of nodes (of any type)
		* @param K number of sensing nodes
		* @param len_x length of rectangle
		* @param len_y height of rectangle
		* @param max_distance maximum distance between neighbours
		* @param distribution distribution of node degree
		* @param seed random seed for node generation
		*/
		Network(
			int N,
			int K,
			double len_x,
			double len_y,
			double max_distance,
			Distribution* distribution);

		friend std::ostream& operator<<(std::ostream &strm, Network &obj) {
			strm << "<Network(\nnodes=[";

			unsigned int MAX_LENGTH = 50;
			for (unsigned int i=0; i<obj.nodes.size(); i++) {
				if (i<MAX_LENGTH) {
					strm << "\n\t" << obj.nodes[i] << ", ";
				}
				else {
					strm << "...";
					break;
				}
			}
			strm << "], ";
			strm << "\npackages=[";
			for (unsigned int i=0; i<obj.packets.size(); i++) {
				if (i<MAX_LENGTH) {
					strm << "\n\t" << obj.packets[i] << ", ";
				}
				else {
					strm << "...";
					break;
				}
			}
			strm << "])>";

			return strm;
		}

		vector<Node> get_nodes();
		vector<Packet> get_packets();

		void spread_packets();
};
