#include "Network.h"

Node::Node(){
	d = 0;
	b = 0;
	pid = 0;
	coordx = 0;
	coordy = 0;
	std::vector<int> newNeighList(0);
	neighList = newNeighList;
	std::vector<double> newForwardingTable(0);
	forwardingTable = newForwardingTable;
}

Node::setNeighList(std::vector<int> newNeighList){
	neighList = newNeighList;
}

std::vector<int> Node::getNeighList(){
	return neighList;
}

Node::setX(double x){
	coordx = x;
}

Node::setY(double y){
	coordy = y;
}

double Node::getCoordX(){
	return coordx;
}

double Node::getCoordY(){
	return coordy;
}

Network::Network(int num, double x, double y) {
	neighThresh = sqrt(pow(x,2)+pow(y,2));
   	size=num;
   	lenx = x;
   	leny = y;
	std::vector<Node> newNodeList(size);
	std::default_random_engine rdx;
	std::default_random_engine rdy;
	std::uniform_real_distribution<> disx(0, lenx);
	std::uniform_real_distribution<> disy(0, leny);
	auto weightx = std::bind(disx, rdx);
	auto weighty = std::bind(disy, rdy);
	for(int i=0; i<size; i++){
		double newNodeX = weightx();
		double newNodeY = weightx();
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

Network::findNeigh(double neighDiff){
	neighThresh = neighDiff;
	for(int i=0; i<size; i++){
		Node thisNode = nodeList[i];
		double thisx = thisNode.getCoordX();
		double thisy = thisNode.getCoordY();
		std::vector<int> newNeighList(0);
		for (int j = 0; j < size; j++) {
			if (j!=i){
				double thatx = nodeList[j].getCoordX();
				double thaty = nodeList[j].getCoordY();
				double diffx = thisx-thatx;
				double diffy = thisy-thaty;
				double diff = sqrt(pow(diffx,2)+pow(diffy,2));
				if (diff<neighDiff){
					newNeighList.push_back(j);
				}
			}
		}
		nodeList[i].setNeighList(newNeighList);
	}
}

Network::describeNetwork(){
	std::cout << "Numero di nodi: " << nodeList.size() << '\n';
	std::cout << "Grandezza della rete: " << lenx << "*" << leny << '\n';
	std::cout << "Soglia di vicinanza: " << neighThresh << '\n';

	for(int i=0; i<size; i++){
		Node thisNode = nodeList[i];
		double thisx = thisNode.getCoordX();
		double thisy = thisNode.getCoordY();
		std::cout << "Coordinate del nodo num. "<< i <<": "<< '\n' << thisx << " "<< thisy << '\n';
		std::vector<int> myNeigh = thisNode.getNeighList();
		std::cout << "Numero dei vicini del nodo num. "<< i << ": " << myNeigh.size() <<'\n';
		std::cout << "Coordinate dei vicini: " << '\n';
		for (int h=0; h < myNeigh.size(); h++){
			double neighx = nodeList[myNeigh[h]].getCoordX();
			double neighy = nodeList[myNeigh[h]].getCoordY();
			std::cout << "Nodo n. "<< myNeigh[h]<< " " << neighx << " " << neighy << '\n';
		}
	}
}
