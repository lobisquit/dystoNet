#include "Network.h"
#include "soliton.h"

using namespace std;

Network::Network(
	int N,
	int K,
	double len_x,
	double len_y,
	double max_distance,
	Distribution* distribution) {

	// setup random number generation
	mt19937 rng = distribution->get_rng();

	uniform_real_distribution<double> x(0, len_x);
	uniform_real_distribution<double> y(0, len_y);


	// create K sensing nodes
	for(int i=0; i<K; i++) {
		std::cout << "sensing: " << i << "\n";
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao
		double b = N * distribution->exp() / K; // see (14) in Lin, Liao

		SensingNode node = SensingNode(x(rng), y(rng), degree, pi, b);
		this->nodes.push_back( (Node) node );
	}

	// create N-K normal nodes
	for(int i=K; i<N; i++) {
		std::cout << "normal: " << i << "\n";
		int degree = distribution->realization();
		double pi = degree / ( N * distribution->exp() ); // see (15) in Lin, Liao

		Node node = Node(x(rng), y(rng), degree, pi);
		this->nodes.push_back(node);
	}

	/** Once all nodes are created, create the neighbors network */
	for (Node node : this->nodes) {
		for (Node other : node.get_neighbours()) {
			double distance = node.distance(other);

			// distance == 0 is when other == node: case to avoid
			if (distance > 0 && distance <= max_distance) {
				node.get_neighbours().push_back(other);
			}
		}
	}
}
