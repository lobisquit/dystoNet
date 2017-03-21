#include <iostream>
#include "EDFC.h"

/** \file */

/** \mainpage
* Online repository for this code can be found at https://github.com/lobisquit/dystoNet
*/

int main() {
	int K = 1000;
	double d = 10;
	double xd = 1.9;
	double violProb = getViolationProb(d,xd,K);
	std::cout << violProb;
}
