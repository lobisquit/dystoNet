#include "Network.h"
#include <iostream>
#include "soliton.h"
#include <algorithm>

using namespace std;

vector<double> linspace(double start, double stop, int num) {
	vector<double> out;
	double step = (stop - start) / (num - 1);

	for (int i = 0; i < num; i++) {
		out.push_back(start + i*step);
	}
	return out;
}

double max(vector<double> v) {
	double max_value = -1 * numeric_limits<double>::infinity();
	for (double element: v) {
		if (element > max_value) {
			max_value = element;
		}
	}
	return max_value;
}

int main(int argc, char* argv[]) {
	int K;
	int N;
	double delta;
	int seed;

	istringstream sK(argv[1]);
	if (!(sK >> K)) { cerr << "Invalid K " << argv[1] << '\n'; }

	istringstream sN(argv[2]);
	if (!(sN >> N)) { cerr << "Invalid N " << argv[2] << '\n'; }

	istringstream sdelta(argv[4]);
	if (!(sdelta >> delta)) { cerr << "Invalid delta " << argv[4] << '\n'; }

	istringstream sseed(argv[5]);
	if (!(sseed >> seed)) { cerr << "Invalid seed " << argv[5] << '\n'; }

	// test a range of values for c, given K and delta, in interval (0, 1)
	// w.r.t. the successful decoding probability

	int number_of_cs = 10;
	vector<double> cs = linspace(0, 1, number_of_cs + 2);
	// exclude invalid values 0 and 1, first and last element
	cs.erase(cs.begin());
	cs.pop_back();

	int number_of_trials = 1000;

	int number_of_decoding_ratios = 10;
	vector<double> etas = linspace(1, 2.5, number_of_decoding_ratios);

	int max_rows = (int) (K * max(etas));

	// indeces to random select degree elements among K
	vector<int> indeces;
	for (int i = 0; i < K; i++) {
		indeces.push_back(i);
	}

	// store successful decoding probability of each (c, eta) couple
	vector<vector<double>> dec_probabilities
		(number_of_cs, vector<double>(number_of_decoding_ratios, 0));

	for (unsigned int c_index = 0; c_index < cs.size(); c_index++) {
		double c = cs[c_index];
		cout << "c index = " << c_index << "/" << cs.size() << "\n";

		for (unsigned int eta_index = 0; eta_index < etas.size(); eta_index++) {
			cout << "eta index = " << eta_index << "/" << etas.size() << "\n";
			double eta = etas[eta_index];

			double mean_dec_probability = 0;
			for (int trial = 0; trial < number_of_trials; trial++) {
				cout << "trial " << trial << "\r";

				RobustSoliton rs = RobustSoliton(c, delta, K, seed);

				// populate coding matrix
				vector<vector<int>> coding_matrix(max_rows, vector<int>(K, 0));

				for (int row = 0; row < max_rows; row++) {
					random_shuffle(indeces.begin(), indeces.end());

					int degree = rs.realization();
					for (int i = 0; i < degree; i++) {
						// put 1 in the i-th random index of current row
						coding_matrix[row][indeces[i]] = 1;
					}
				}
				int h = (int) (eta * K);
				mean_dec_probability += message_passing(h, coding_matrix) ? 1 : 0;
			}
			mean_dec_probability /= number_of_trials;

			dec_probabilities[c_index][eta_index] = mean_dec_probability;
		}
	}

	for (vector<double> row: dec_probabilities) {
		for (double p: row) {
			cout << p << ", ";
		}
		cout << "\n";
	}
}
