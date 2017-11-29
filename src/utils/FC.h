/*!
 * \file
 * Fountain codes utils
 */

#ifndef _FC_H_
#define _FC_H_

#include <vector>

using namespace std;

/**
 * Check if some encoded packets are decodable with message passing.
 * 1 in position (i, j) means that received i-th packet is obtained by
 * XOR operation of j-th source packet
 * @param h number of lines to consider of en_matrix
 * @param en_matrix encoding matrix
 * @return if packets can be decoded using first h lines
 */
bool message_passing(unsigned int h, vector<vector<int>> en_matrix);

void print_matrix(unsigned int h, vector<vector<int>> matrix);

void print_vector(vector<int> v);

void print_vector(vector<double> v);

#endif
