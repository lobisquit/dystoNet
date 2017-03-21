#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "EDFC.h"
#include "soliton.h"

/** \file */

/** \mainpage
* Online repository for this code can be found at https://github.com/lobisquit/dystoNet
*/

double acceptanceProbability(int L, int newL, double temp);
double getL(int K, int d);
double simulatedAnnealing(int K, int d);

int main() {
	int K = 1000;
	double d = 10;
	double xd = 1.9;
	srand (time(NULL));
	double violProb = getViolationProb(d,xd,K);
	double bestx = simulatedAnnealing(K,d);
	std::cout << bestx << "\n";
}

double acceptanceProbability(int currentL, int newL, double temp){
	if(newL < currentL){
		return 1.0;
	}
	return exp((currentL-newL)/temp);
}

double getL(int K, int d, double x){
	double lambda = 1;
	double c = 0.01;
	double delta = 0.05;
	double deltad = 0.05;

	RobustSoliton mu = RobustSoliton(c, delta, K);
	double first_term = 0;
	for(int i = 1; i<=K; i++){
		first_term += x*i*mu.get(i);
	}
	double newL = first_term + lambda*(deltad - getViolationProb(d,x,K));

	return newL;
}

double simulatedAnnealing(int K, int d){
	double Tmax = 10000;
	double Tmin = 0.01;
	double coolingRate = 0.003;
	double bestL = 10000;
	double bestx = 0;
	double T = Tmax;
	double currentx = 0;
	double currentL = 0;
	/* initialize random seed: */
	srand (time(NULL));
	for(int i = 0; i < 20; i++){
		currentx = (rand() % 5000)/1000.0;
		currentL = getL(K, d, currentx);
		while(T>Tmin){
			double newx = currentx +(rand() % 10000)/10000.0;
			double newL = getL(K, d, newx);
			if (acceptanceProbability(currentL, newL, T) > rand()){
				currentL = newL;
				currentx = newx;
			}

			T *= 1 - coolingRate;
		}
		if(currentL < bestL){
			bestL = currentL;
			bestx = currentx;
		}
	}
	return bestx;
}
