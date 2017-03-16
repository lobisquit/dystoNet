#include <stdio.h>
#include "soliton.h"

/** \file */

/** \mainpage
* Online repository for this code can be found at https://github.com/lobisquit/dystoNet
*/

const int N = 2000; /** Number of nodes */
const int K = 1000; /** Number of sensing nodes */
const double deltad = 0.05; /** Violation probability */
const double R = c * log(K / delta) * sqrt(K);

main() {
	int d = 1; // Node-degree
	double mu = getmu(K, R, d);
	printf("%f\n", mu);
	return 0;
}
