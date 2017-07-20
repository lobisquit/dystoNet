#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#include "functionCSV.cpp"

int main () {
	vector<double> v1(10,10);
	string name("results/results.csv");

	writeCSV(v1, name);

	vector<double> v2 = readCSV("results/results.csv");

	for (int i =0; i<v2.size(); i++){
		cout << v2[i] << '\n';
	}

	return 0;
}
