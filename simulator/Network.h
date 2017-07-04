#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <functional>

class Node {
	private:
		int d;
		int b;
		double pid;
		double coordx;
		double coordy;
		std::vector<Node*> neighList;
		std::vector<double> forwardingTable;
	public:
		Node();
		void setX(double x);
		void setY(double y);
		void setNeighList(std::vector<Node*> newNeighList);
		double getCoordX();
		double getCoordY();
		std::vector<Node*> getNeighList();
};

class Network {
	private:
		double neighThresh;
		int size;
		double lenx;
		double leny;
		std::vector<Node> nodeList;
	public:
		Network(int num, double x, double y);
		double getSize();
		double getArea();
		double getX();
		double getY();
		void findNeigh(double neighDiff);
		std::vector<Node> getNodeList();
		void describeNetwork();
};
