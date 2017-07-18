#include "Network.h"
#include "Node.h"
#include <iostream>

int main() {
	int N = 1000;
	int K = 5;
	double len_x = 10;
	double len_y = 10;
	double neighThresh = 2;
	Distribution d = IdealSoliton(K, 10);

	Network net = Network(N, K, len_x, len_y, neighThresh, &d);

	// for (Node node : net.get_nodes()) {
	// 	std::cout << node << "\n";
	// }
	//
	// std::cout << net.get_packets().size() << '\n';
	std::cout << net << "\n";
}

// int main() {
// 	double x = 0.5;
// 	double y = 0.5;
// 	int degree = 3;
// 	double pi_d = 0.3;
// 	double b = 0;
//
// 	Node node = Node(x, y, degree, pi_d, b);
//
// 	vector<Packet> packets;
// 	packets.reserve(10);
//
// 	for (int i=0; i<10; i++) {
// 		std::cout << "asdasd" << '\n';
// 		Packet p = Packet(&node, i);
// 		packets.push_back(p);
//
// 		node.add_packet( &packets.back() );
// 		std::cout << packets.back() << " @ " << &packets.back() << "\n";
// 	}
// 	std::cout << '\n';
// 	// for (Packet p: packets) {
// 	// 	std::cout << p << "\n";
// 	// 	std::cout << &p << "\n";
// 	// }
//
// 	for (Packet* p: node.get_packets()) {
// 		std::cout << *p << " @ " << p << "\n";
// 	}
// }
