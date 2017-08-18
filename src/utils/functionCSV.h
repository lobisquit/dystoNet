#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#ifndef _FUNCTION_CSV_H_
#define _FUNCTION_CSV_H_

//Function writeCSV: it receives as input a vector of double and the path in which
// save the vector. It creates a file .csv in that path.

void writeCSV(vector<double> v_out, string path_out);

//Function readCSV: it receives as input the path of the file .csv that we want to read.
// It returns a vector of double which cointains the data previously save in the file.

vector<double> readCSV(string path_in);

#endif
