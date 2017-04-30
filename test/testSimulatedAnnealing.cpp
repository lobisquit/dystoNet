#include "catch.hpp"
#include <math.h>

#if defined(linux) || defined(__linux) || defined(__linux__)
	#include "../src/heuristicSearch.cpp"
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER)
	#include "..\src\heuristicSearch.cpp"
#else
	#error OS not supported
#endif

TEST_CASE( "Test annealing", "[SimulatedAnnealing]" ) {
	int K = 10;
	RobustSoliton rs = RobustSoliton(0.01, 0.05, K);
	SimulatedAnnealing SA = SimulatedAnnealing(
				/* K 						*/ K,
				/* N 						*/ 100,
				/* starting_temperature 	*/ 1500,
				/* cooling_rate 			*/ 0.99,
				/* max_iterations 			*/ 1000,
				/* robust_soliton 			*/ &rs,
				/* max_failure_probability 	*/ 0.05);

	SECTION( "Get initial value" ) {
		double x[K];
		// note that such point is automatically checked in function
		SA.get_initial_solution(x);
	}

	SECTION( "Neighbour is found without reaching recursion depth" ) {
		double x[K];
		SA.get_initial_solution(x);

		double new_x[K];
		// note that new point is automatically checked in function
		SA.get_neighbour(x, new_x);
	}
}
