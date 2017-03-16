#include<stdio.h>
#include "soliton.cpp"

/** \file */

/** \mainpage
 * Online repository for this code can be found at https://github.com/lobisquit/dystoNet
 */

main() {
	int d = 1; // Node-degree
	double mu = getmu(R, d);
	printf("%f\n", mu);
	return 0;
}
