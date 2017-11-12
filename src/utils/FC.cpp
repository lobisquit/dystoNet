#include "soliton.h"
#include "FC.h"
#include <iostream>

using namespace std;

bool message_passing(unsigned int h, vector<vector<int>> en_matrix) {
	if (h > en_matrix.size()) {
		throw invalid_argument("Number of rows to consider exceeds actual number of rows");
	}

	// create a 0/1 vector to mark which packets have been solved
	vector<int> solved_packets(en_matrix[0].size(), 0);

	// initialize proper termination variables
	unsigned int number_of_solved_packets = 0;
	unsigned int new_rows_of_degree_one = 0;

	do {
		/** Find all rows of degree 1: mark corresponding source packet as solved */
		for(unsigned int i=0; i<h; i++) {
			int degree = 0;
			int source_packet_index = -1;

			for (unsigned int j=0; j<en_matrix[0].size(); j++) {
				if (en_matrix[i][j] == 1) {
					degree++;
					source_packet_index = j;
				}
			}
			if (degree == 1) {
				solved_packets[source_packet_index] = 1;
			}
		}

		/**
			 Gaussian simplificate decoding matrix
			 Note that in our test algorithm, each degree 1 packet is used to
			 simplify also it rows, leading to all zeros the corresponding
			 column.

			 Algorithm is still correct, since decoded source packets
			 information is kept in solved_packets vector.
		 */
		for (unsigned int i=0; i<h; i++) {
			for (unsigned int j=0; j<solved_packets.size(); j++) {
				if (solved_packets[j] == 1) {
					if (en_matrix[i][j] == 1) {
						en_matrix[i][j] = 0;
					}
				}
			}
		}

		/** Count new rows of degree one */
		new_rows_of_degree_one = 0;
		for (unsigned int i=0; i<h; i++) {
			int degree = 0;
			for(unsigned int j=0; j<en_matrix[0].size(); j++) {
				if (en_matrix[i][j] == 1) {
					degree++;
				}
			}
			if (degree == 1) {
				new_rows_of_degree_one++;
			}
		}

		/** Count solved packets */
		number_of_solved_packets = 0;
		for (int element: solved_packets) {
			if (element == 1) {
				number_of_solved_packets++;
			}
		}

		/** Algorithm stops either if all source packets have been decoded, or if no new packet of degree 1 is found */
	} while(number_of_solved_packets != solved_packets.size() && new_rows_of_degree_one !=0);

	return number_of_solved_packets == solved_packets.size();
}

void print_matrix(unsigned int h, vector<vector<int>> matrix) {
	cout << "-------------------\n";
	for (unsigned int i=0; i<h || i<matrix.size(); i++) {
		vector<int> row = matrix[i];
		for (int element: row) {
			cout << element << "\t";
		}
		cout << "\n";
	}
	cout << "-------------------\n";
}

void print_vector(vector<int> v) {
	cout << "---------------------------\n";
	for (int element: v) {
		cout << element << "\t";
	}
	cout << "\n";
	cout << "---------------------------\n";
}

void print_vector(vector<double> v) {
	cout << "---------------------------\n";
	for (double element: v) {
		cout << element << "\t";
	}
	cout << "\n";
	cout << "---------------------------\n";
}
