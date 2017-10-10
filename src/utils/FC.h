#ifndef _FC_H_
#define _FC_H_

#include <vector>

using namespace std;

bool message_passing(unsigned int h, vector<vector<int>> en_matrix);

void print_matrix(unsigned int h, vector<vector<int>> matrix);

void print_vector(vector<int> v);

void print_vector(vector<double> v);

#endif
