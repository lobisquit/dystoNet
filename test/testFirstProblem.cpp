#include <iostream>
#include "catch.hpp"
#include <math.h>

#if defined(linux) || defined(__linux) || defined(__linux__)
	#include "../src/firstProblem.cpp"
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER)
	#include "..\src\firstProblem.cpp"
#else
	#error OS not supported
#endif

TEST_CASE( "Test theoretic bound", "[TheoreticBound]" ) {
	int K = 10;
	RobustSoliton rs = RobustSoliton(0.01, 0.05, K);
	TheoreticBound TB = TheoreticBound(
				/* K 						*/ K,
				/* N 						*/ 100,
				/* robust_soliton 			*/ &rs,
				/* max_failure_probability 	*/ 0.05);

	SECTION( "Solution obtained with the bound is a valid one" ) {
		double x[K];
		// run_search throws std::logic_error if x is not valid after research
		REQUIRE_NOTHROW( TB.run_search(x) );
	}
}

TEST_CASE( "Test simulated annealing", "[SimulatedAnnealing]" ) {
	int K = 10;
	RobustSoliton rs = RobustSoliton(0.01, 0.05, K);
	SimulatedAnnealing SA = SimulatedAnnealing(
				/* K 						*/ K,
				/* N 						*/ 100,
				/* robust_soliton 			*/ &rs,
				/* max_failure_probability 	*/ 0.05,
				/* starting_temperature 	*/ 1500.0,
				/* cooling_rate 			*/ 0.99,
				/* max_iterations 			*/ 5,
				/* acceptance_threshold		*/ 0.9999,
				/* steps_coefficient 		*/ 5e5);

	SECTION( "Test starting point" ) {
		double x[K];
		// run_search throws std::logic_error if x is not valid after research
		REQUIRE_NOTHROW( SA.get_initial_solution(x) );
	}

	SECTION( "First neighbour can be found starting from initial point" ) {
		double x[K];
		SA.get_initial_solution(x);

		double new_x[K];
		// get_neighbour throws std::logic_error if x is not valid
		REQUIRE_NOTHROW( SA.get_neighbour(x, new_x) );
	}

	SECTION( "Test acceptance_probability is valid" ) {
		double x[K];
		SA.get_initial_solution(x);

		double new_x[K];
		SA.get_neighbour(x, new_x);

		double p = SA.acceptance_probability(x, new_x);
		REQUIRE( p <= 1 );
		REQUIRE( p >= 0 );
	}

	SECTION( "Solution obtained with the bound is a valid one" ) {
		double x[K];
		// run_search throws std::logic_error if x is not valid after research
		REQUIRE_NOTHROW( SA.run_search(x) );
	}
}
