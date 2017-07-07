#include "Network.h"
#include <iostream>

int main() {
	int num = 10;
	double lenx = 10;
	double leny = 10;
	double neighThresh = 5;
	Network net = Network(num, lenx, leny, neighThresh);
	net.describeNetwork();
}
