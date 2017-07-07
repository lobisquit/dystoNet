#include "Network.h"

Network::Network(int num, double lenx, double leny, double neighThresh) {

	this->neighThresh = neighThresh;

	// random seed is set to a default value, for reproducibility
	rng.seed(1);

	std::uniform_real_distribution<double> disx(0, lenx);
	std::uniform_real_distribution<double> disy(0, leny);

	for(int i=0; i<num; i++){
		double newNodeX = disx(this->rng);
		double newNodeY = disy(this->rng);
		/**----------------- TO DO: compute degree and pi ----------------- */
		Node newNode = Node(newNodeX, newNodeY, 2, 1);
		this->nodeList.push_back(newNode);
	}

	/** Once all nodes are created, create the neighbors map */
	this->find_neigh();
}

std::vector<Node> Network::getNodeList(){
	return nodeList;
}

void Network::find_neigh(){
	for(int i=0; i<nodeList.size(); i++){
		Node thisNode = this->nodeList[i];
		double thisx = thisNode.get_coordx();
		double thisy = thisNode.get_coordy();
		std::vector<Node*> newNeighList(0);
		for (int j = 0; j < nodeList.size(); j++) {
			if (j!=i){
				double thatx = this->nodeList[j].get_coordx();
				double thaty = this->nodeList[j].get_coordy();
				double diffx = thisx-thatx;
				double diffy = thisy-thaty;
				double diff = sqrt(pow(diffx,2)+pow(diffy,2));
				if (diff<this->neighThresh){
					newNeighList.push_back(&this->nodeList[j]);
				}
			}
		}
		this->nodeList[i].set_neighbours(newNeighList);
	}
}

void Network::describeNetwork(){
	std::cout << "Numero di nodi: " << nodeList.size() << '\n';
	// std::cout << "Grandezza della rete: " << lenx << "*" << leny << '\n';
	std::cout << "Soglia di vicinanza: " << this->neighThresh << '\n';

	for(int i=0; i<nodeList.size(); i++){
		Node thisNode = nodeList[i];
		double thisx = thisNode.get_coordx();
		double thisy = thisNode.get_coordy();
		std::cout << "Coordinate del nodo num. "<< i <<": "<< '\n' << thisx << " "<< thisy << '\n';
		std::vector<Node*> myNeigh = thisNode.get_neighbours();
		std::cout << "Numero dei vicini del nodo num. "<< i << ": " << myNeigh.size() <<'\n';
		std::cout << "Coordinate dei vicini: " << '\n';
		for (int h=0; h < myNeigh.size(); h++){
			double neighx = myNeigh[h]->get_coordx();
			double neighy = myNeigh[h]->get_coordy();
			std::cout << "Nodo "<< myNeigh[h]<< " " << neighx << " " << neighy << '\n';
		}
	}
}
