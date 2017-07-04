#define CATCH_CONFIG_MAIN
#include "binomial.cpp"
#include "catch.hpp"
#include <math.h>
//
// #if defined(linux) || defined(__linux) || defined(__linux__)
// 	#include "../src/binomial.cpp"
// #elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER)
// 	#include "..\src\binomial.cpp"
// #else
// 	#error OS not supported
// #endif

TEST_CASE( "Test log_gamma values", "[ln_gamma]" ) {
	SECTION( "This gamma coincide with python scipy implementation" ) {
		int inputs[4] = {1, 10, 50, 100};
		double true_values[4] = {
			0.0000000000, 12.8018274801, 144.5657439463, 359.1342053696
		};

		for (int i = 0; i < 4; i++) {
			// require two numbers close enough
			REQUIRE( (ln_gamma(inputs[i]) - true_values[i]) <= 1e-9 );
		}
	}

	SECTION( "Test Gamma(1/2) = sqrt(pi)" ) {
		REQUIRE( (ln_gamma(0.5) - 0.5*log(3.14159265358979323846) <= 1e-9) );
	}
}

TEST_CASE( "Test binomial_CDF values", "[binomial_CDF]" ) {
	SECTION( "This binomial_CDF coincides with python scipy implementation" ) {
		int n = 100;
		double p = 0.1;
		int inputs[7] = {0, 10, 20, 50, 60, 80, 100};

		double true_values[7] = {
			1.0000000000000024,
			0.5487098345579977,
			0.001978560865771237,
			5.832038785734366e-24,
			2.1909398776320784e-34,
			6.699796381281256e-61,
			1.0000000000000056e-100,
		};

		for (int i = 0; i < 7; i++) {
			// require two numbers close enough
			REQUIRE( (binomial_CDF(n, inputs[i], p) - true_values[i]) <= 1e-10 );
		}
	}

	SECTION( "Test CDF=0 when n<k" ) {
		REQUIRE( binomial_CDF(5, 10, 0.5) == 0 );
	}

	SECTION( "Test CDF=nan when n<=0: invalid input value" ) {
		REQUIRE( std::isnan(binomial_CDF(0, 10, 0.5)) );
		REQUIRE( std::isnan(binomial_CDF(-5, -2, 0.5)) );
	}

	SECTION( "Test std::runtime_error is thrown when p is not in [0, 1]" ) {
		REQUIRE_THROWS_AS( binomial_CDF(5, 10, 1.2), std::runtime_error );
		REQUIRE_THROWS_AS( binomial_CDF(5, 10, -0.5), std::runtime_error );
	}
}
