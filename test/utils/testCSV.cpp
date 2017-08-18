#include "catch.hpp"
#include "functionCSV.cpp"
#include <stdio.h>

TEST_CASE("Functions writeCSV and readCSV are able to build and read a csv file containing a vector of double", "[CSV]") {
	SECTION("My input and output values are the same"){
		vector<double> v1(1000);
		for(unsigned int i=0; i<v1.size(); i++) {
			v1[i] = i;
		}
		string path("results.csv");
		writeCSV(v1, path);

		vector<double> v2 = readCSV(path);
		for (unsigned int i = 0; i < v2.size(); i++){
			REQUIRE(abs(v1[i] - v2[i]) <= 1e-9);
		}
	}

	// remove dummy CSV
	remove("results.csv");
}
