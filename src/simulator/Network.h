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

	public:
		vector<Packet> packets;
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

			for (Node node : obj.nodes) {
				strm << "\n" << node << ", ";
			}
			// \b\b moves cursor backwards 2 times, deleting last ", "
			strm << "], ";
			strm << "\npackages=[";

			for (Packet packet : obj.packets) {
				strm << "\n" << packet << ", ";
			}
			return strm << "])>";
		}

		vector<Node> get_nodes();
};
