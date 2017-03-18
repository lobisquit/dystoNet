#include "catch.hpp"
#include <math.h>

#if defined(linux) || defined(__linux) || defined(__linux__)
	#include "../src/binomial.cpp"
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(_MSC_VER)
	#include "..\src\binomial.cpp"
#else
	#error OS not supported
#endif

TEST_CASE( "Test log_gamma values", "[ln_gamma]" ) {
	SECTION( "This gamma coincide with python scipy implementation" ) {
		int inputs[4] = {1, 10, 50, 100};
		double true_values[4] = {
			0.0000000000, 12.8018274801, 144.5657439463, 359.1342053696
		};

		for (int i = 0; i < 4; i++) {
			// require two number close enough
			REQUIRE( (ln_gamma(inputs[i]) - true_values[i]) <= 1e-9 );
		}
	}

	SECTION( "Test Gamma(1/2) = sqrt(pi)" ) {
		REQUIRE( (ln_gamma(0.5) - 0.5*log(M_PI) <= 1e-9) );
	}
}
