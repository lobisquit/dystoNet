#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "second_problem.h"
#include <chrono>

using namespace std::chrono;

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 1);

	SecondProblem SP = SecondProblem(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05
	);

	vector<double> no_redundancy(K, 1);

	high_resolution_clock::time_point begin = high_resolution_clock::now();
	cout << "f = " << SP.objective_function(no_redundancy);
	duration<double> time_span = high_resolution_clock::now() - begin;
	cout << " with time " << time_span.count() << "s\n";
}
