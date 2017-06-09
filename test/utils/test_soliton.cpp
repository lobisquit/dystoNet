#include "catch.hpp"
#include "soliton.cpp"

TEST_CASE( "Our ideal soliton is a probability distribution", "[ideal_soliton]" ) {
	int K = 1000;
	IdealSoliton rho = IdealSoliton(K);

	SECTION( "Ideal soliton is strictly positive in interval [1, K]" ) {
		for (int degree = 1; degree <= K; degree++) {
			REQUIRE( rho.get(degree)>0 );
		}
	}

	SECTION( "Ideal soliton is zero outside [1, K]" ) {
		for (int degree = -10; degree <= 0; degree++) {
			REQUIRE( rho.get(degree) == 0 );
		}
		for (int degree = K+1; degree <= K+10; degree++) {
			REQUIRE( rho.get(degree) == 0 );
		}
	}

	SECTION( "Ideal soliton sums up to 1" ) {
		double cumulative_sum = 0;
		for (int degree = 1; degree <= K; degree++) {
			cumulative_sum += rho.get(degree);
		}
		// this has to be done because the sum is very close to 1, but not one
		// because of double precision
		REQUIRE( abs(1-cumulative_sum) < 1e-13 );
	}
}

TEST_CASE( "Our robust soliton is a probability distribution", "[robust_soliton]" ) {
	int K = 1000;
	double c = 0.01;
	double delta = 0.01;
	RobustSoliton mu = RobustSoliton(c, delta, K);

	SECTION( "Robust soliton is strictly positive in interval [1, K]" ) {
		for (int degree = 1; degree <= K; degree++) {
			REQUIRE( mu.get(degree)>0 );
		}
	}

	SECTION( "Robust soliton is zero outside [1, K]" ) {
		for (int degree = -10; degree <= 0; degree++) {
			REQUIRE( mu.get(degree) == 0 );
		}
		for (int degree = K+1; degree <= K+10; degree++) {
			REQUIRE( mu.get(degree) == 0 );
		}
	}

	SECTION( "Robust soliton sums up to 1" ) {
		double cumulative_sum = 0;
		for (int degree = 1; degree <= K; degree++) {
			cumulative_sum += mu.get(degree);
		}
		// this has to be done because the sum is very close to 1, but not one
		// because of double precision
		REQUIRE( abs(1-cumulative_sum) < 1e-13 );
	}
}
