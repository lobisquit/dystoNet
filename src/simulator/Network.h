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
		Node* nodes;
		Packet* packets;

		int N;

		int K;

		int b;

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
			Node* nodes = obj.get_nodes();
			strm << "<Network(nodes=[\n";
			for (int i = 0; i < obj.get_nodes_size(); i++) {
				strm << "\t" << nodes[i] << " {\n";
				for (Packet* pkt: nodes[i].get_packets()) {
					strm << "\t\t" << *pkt << "\n";
				}
				strm << "\t}\n";
			}
			return strm << "])>";
		}

		int get_packets_size();
		int get_nodes_size();

		Node* get_nodes();
		Packet* get_packets();

		void spread_packets();

		/** Visit to the network */
		vector<vector<int>> collector(int h);
};
