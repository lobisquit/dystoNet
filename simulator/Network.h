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
		std::vector<int> neighList;
		std::vector<double> forwardingTable;
	public:
		Node();
		setX(double x);
		setY(double y);
		setNeighList(std::vector<int> newNeighList);
		double getCoordX();
		double getCoordY();
		std::vector<int> getNeighList();
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
		findNeigh(double neighDiff);
		std::vector<Node> getNodeList();
		describeNetwork();
};
