#include <iostream>
#include "soliton.h"

/** \file */

/** \mainpage
* Online repository for this code can be found at https://github.com/lobisquit/dystoNet
*/

int main() {
	// setting c = 0.01, deltad = 0.05, K = 1000 in the constructor
	RobustSoliton s = RobustSoliton(0.01, 0.05, 1000);
	double cumulative_sum = 0;
	for (int d=0; d<=120; d++) {
		std::cout << "mu(" << d << ")   = " << s.get(d) << "\n";
		cumulative_sum += s.get(d);
	}
	std::cout << "Sum = " << cumulative_sum << "\n";
	return 0;
}
