#include <vector>

using namespace std;

vector<double> linspace(double start, double stop, int num) {
	vector<double> out;
	double step = (stop - start) / (num - 1);

	for (int i = 0; i < num; i++) {
		out.push_back(start + i*step);
	}
	return out;
}
