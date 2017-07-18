#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Node.cpp"

TEST_CASE( "Test theoretic bound", "[Node]" ) {
	double x = 0.5;
	double y = 0.5;
	int degree = 3;
	double pi_d = 0.3;
	double b = 0;

	Node node = Node(x, y, degree, pi_d, b);

	SECTION( "Test add of Packets to Node (pointers consinstency)" ) {

		// packets should be kept in Network object
		// otherwise pointers could be invalidated
		vector<Packet> packets;
		int n_packets = 100;
		// this is VERY important: otherwise vector is moved and
		// pointers to packets stored in node becomes invalid
		packets.reserve(n_packets);

		for (int i=0; i<n_packets; i++) {
			Packet p = Packet(&node, i);
			packets.push_back(p);

			node.add_packet( &packets.back() );
			INFO( "1) " << packets.back() << " @ " << &packets.back() );
		}

		int i = 0;
		for (Packet* p: node.get_packets()) {
			INFO( "2) " << *p << " @ " << p );
			// check id: fails if memory of Packet is overwritten
			REQUIRE( p->get_id() == i);
			i++;
		}
	}
}
