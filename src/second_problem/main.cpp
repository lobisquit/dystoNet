#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "second_problem.h"
#include <chrono>
#include "functionCSV.h"

using namespace std::chrono;

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 1);

	// IdealSoliton is = IdealSoliton(K, 1);

	SecondProblem SP = SecondProblem(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05
	);

	vector<double> v = rs.get_probabilities();
	high_resolution_clock::time_point begin;
	duration<double> time_span;

	cout << (SP.respect_constraints(v) ? "True" : "False") << "\n";
	cout << SP.objective_function(v) << "\n";
	for (int i=0; i<10; i++) {
		v = SP.get_initial_solution();
		begin = high_resolution_clock::now();
		cout << "f=" << SP.objective_function(v);
		time_span = high_resolution_clock::now() - begin;
		cout << " with time " << time_span.count() << "s\n";

		begin = high_resolution_clock::now();
		cout << "g=" << SP.approximate_objective_function(v);
		time_span = high_resolution_clock::now() - begin;
		cout << " with time " << time_span.count() << "s\n";
		cout << "\n";
	}

	// vector<double> no_redundancy(K, 1 / (double) K);
	//

}
