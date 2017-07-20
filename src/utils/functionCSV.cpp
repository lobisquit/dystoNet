#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//Function writeCSV: it receives as input a vector of double and the path in which
// save the vector. It creates a file .csv in that path.

void writeCSV(vector<double> v_out, string path_out, string title_out){
	int l = v_out.size();
	ofstream outfile (path_out, ofstream::out);
	outfile << title_out;
	for(int i=1; i<l+1; i++) {
		outfile << "\n";
		outfile << v_out[i-1];
	}
    outfile.close();
}

//Function readCSV: it receives as input the path of the file .csv that we want to read.
// It returns a vector of double which cointains the data previously save in the file.

vector<double> readCSV(string path_in){
	ifstream infile (path_in, ifstream::in );
	vector<double> v_in;
	string value;
	string::size_type sz;
	getline ( infile, value, '\n' );
	while ( infile.good() ){
  		getline ( infile, value, '\n' );
		v_in.push_back(atof(value.c_str()));
	}
	return v_in;
}
