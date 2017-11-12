#include "FC.cpp"
#include "catch.hpp"
#include <math.h>

using namespace std;

TEST_CASE( "Message passing algorithm is correct", "[FC]" ) {

	SECTION( "Simple decodable matrix is decoded successfully" ) {
		vector<vector<int>> en_matrix(5, vector<int>(3, 0));
		en_matrix[0][0] = 1;
		en_matrix[0][1] = 1;
		en_matrix[1][1] = 1;
		en_matrix[1][2] = 1;
		en_matrix[2][0] = 1;
		en_matrix[2][2] = 1;
		en_matrix[3][2] = 1;

		REQUIRE( message_passing(5, en_matrix) );
	}

	SECTION( "Matrix with no degree 1 rows is not decoded" ) {
		vector<vector<int>> en_matrix(5, vector<int>(3, 0));
		en_matrix[0][0] = 1;
		en_matrix[0][1] = 1;
		en_matrix[1][1] = 1;
		en_matrix[1][2] = 1;
		en_matrix[2][0] = 1;
		en_matrix[2][2] = 1;
		en_matrix[3][2] = 1;
		en_matrix[3][1] = 1;

		REQUIRE( !message_passing(5, en_matrix) );
	}

	SECTION( "Matrix with a all-zero column is not decoded" ) {
		vector<vector<int>> en_matrix(5, vector<int>(3, 0));
		en_matrix[0][0] = 1;
		en_matrix[1][2] = 1;
		en_matrix[2][0] = 1;
		en_matrix[2][2] = 1;
		en_matrix[3][2] = 1;

		REQUIRE( !message_passing(5, en_matrix) );
	}


	SECTION( "Matrix with a all-zero column is not decoded" ) {
		vector<vector<int>> en_matrix(5, vector<int>(3, 0));
		en_matrix[0][0] = 1;
		en_matrix[1][2] = 1;
		en_matrix[2][0] = 1;
		en_matrix[2][2] = 1;
		en_matrix[3][2] = 1;

		REQUIRE( !message_passing(5, en_matrix) );
	}

	SECTION( "Undecodable matrix is decoded with an additional row" ) {
		vector<vector<int>> en_matrix(5, vector<int>(3, 0));
		en_matrix[0][0] = 1;
		en_matrix[0][1] = 1;
		en_matrix[1][1] = 1;
		en_matrix[1][2] = 1;
		en_matrix[2][0] = 1;
		en_matrix[2][2] = 1;
		en_matrix[3][2] = 1;
		en_matrix[3][0] = 1;
		en_matrix[4][2] = 1;

		REQUIRE( !message_passing(4, en_matrix) );
		REQUIRE( message_passing(5, en_matrix) );
	}
}
