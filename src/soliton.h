#include <math.h>
#ifndef _SOLITON_H_
#define _SOLITON_H_

/**
* Constants for Robust Soliton Distribution
*/
const double c = 0.01;
const double delta = 0.05;

/**
* Get Ideal Soliton distribution
* @param d node degree
* @return Robust Soliton probabilit
*/
double getrho(int K, int d);

/**
* Tau weighting function for Robust Soliton
* @param R Robust soliton parameter
* @param d node degree
* @return Robust Soliton probability for d
*/
double gettau(int K, double R, int d);

/**
* Compute Robust Soliton Distribution for a node-degree d
* @param R Robust soliton parameter
* @param d node degree
* @return Robust Soliton probability for d
*/
double getmu(int K, double R, int d);

#endif
