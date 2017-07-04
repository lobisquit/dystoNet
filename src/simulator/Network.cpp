#include "Network.h"

Network::Network(int num, double x, double y) {
	neighThresh = sqrt(pow(x,2)+pow(y,2));
   	size=num;
   	lenx = x;
   	leny = y;
	std::vector<Node> newNodeList(size);

	// random seed is set to a default value, for reproducibility
	rng.seed(1);

	std::uniform_real_distribution<double> disx(0, lenx);
	std::uniform_real_distribution<double> disy(0, leny);

	for(int i=0; i<size; i++){
		double newNodeX = disx(rng);
		double newNodeY = disy(rng);
		Node newNode = Node();
		newNode.setX(newNodeX);
		newNode.setY(newNodeY);
		newNodeList[i]=newNode;
	}
	nodeList = newNodeList;
}

double Network::getSize(){
	return size;
}

double Network::getX(){
	return lenx;
}

double Network::getY(){
	return leny;
}

double Network::getArea(){
	double area = lenx*leny;
	return area;
}

std::vector<Node> Network::getNodeList(){
	return nodeList;
}

void Network::findNeigh(double neighDiff){
	neighThresh = neighDiff;
	for(int i=0; i<size; i++){
		Node thisNode = nodeList[i];
		double thisx = thisNode.getCoordX();
		double thisy = thisNode.getCoordY();
		std::vector<Node*> newNeighList(0);
		for (int j = 0; j < size; j++) {
			if (j!=i){
				double thatx = nodeList[j].getCoordX();
				double thaty = nodeList[j].getCoordY();
				double diffx = thisx-thatx;
				double diffy = thisy-thaty;
				double diff = sqrt(pow(diffx,2)+pow(diffy,2));
				if (diff<neighDiff){
					newNeighList.push_back(&nodeList[j]);
				}
			}
		}
		nodeList[i].setNeighList(newNeighList);
	}
}

void Network::describeNetwork(){
	std::cout << "Numero di nodi: " << nodeList.size() << '\n';
	std::cout << "Grandezza della rete: " << lenx << "*" << leny << '\n';
	std::cout << "Soglia di vicinanza: " << neighThresh << '\n';

	for(int i=0; i<size; i++){
		Node thisNode = nodeList[i];
		double thisx = thisNode.getCoordX();
		double thisy = thisNode.getCoordY();
		std::cout << "Coordinate del nodo num. "<< i <<": "<< '\n' << thisx << " "<< thisy << '\n';
		std::vector<Node*> myNeigh = thisNode.getNeighList();
		std::cout << "Numero dei vicini del nodo num. "<< i << ": " << myNeigh.size() <<'\n';
		std::cout << "Coordinate dei vicini: " << '\n';
		for (int h=0; h < myNeigh.size(); h++){
			double neighx = myNeigh[h]->getCoordX();
			double neighy = myNeigh[h]->getCoordY();
			std::cout << "Nodo "<< myNeigh[h]<< " " << neighx << " " << neighy << '\n';
		}
	}
}
