/** \file
* Resolve an optimization problem through the minimization of a non-convex Lagrangian function.
* This is be possible thanks to the Simulated Annealing Algorithm.
* The problem in question is
* \f{eqnarray}{
* 	minimize \quad \sum\limits_{d=1}^{K} x_dd\mu(d) \\
* 	subject \; to \quad Pr(Y < d | X = d) \le \delta_d \\
* 	x_d \ge 1 \\
* 	for \; d = 1,...,K/R
* \f}
* where \f$ \delta_d \f$ is a small constant and \f$ Pr(Y < d | X = d) \f$ is given in <a
* href="EDFC_8h.html">EDFC Library</a>.
* To resolve this problem we have to minimize the following Lagrangian function:
* \f{eqnarray}{
* L(x_d,\eta,\lambda) = \sum\limits_{d=1}^{K} x_dd\mu(d) + \eta(1-x_d) + \lambda(\delta_d - Pr(Y < d | X = d))
* \f}
* So we have to resolve \f$ L(x_d,\eta,\lambda) = 0 \f$
*/

/** \mainpage
* Online repository for this code can be found at https://github.com/lobisquit/dystoNet
*/
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "EDFC.h"
#include "soliton.h"

/**
* Probability to accept the new solution calculated in the last step of the Simulated Annealing.
* ### Algorithm
* If the new solution \f$ S_{new} \f$ is better (less then) the current solution \f$ S_{current} \f$, accept the new solution w.p. 1,
* otherwise accept the new solution w.p. \f$ e^{(S_{new} - S_{current})/T} \f$.
* In this last case the \f$ S_{new} \f$ will be worse (greater then) or equal to \f$ S_{current} \f$
*/
double acceptanceProbability(int currentS, int newS, double temp);
/**
* Lagrangian function just described.
* We have used as parameters of the Lagrangian function \f$ \lambda = 1 \f$ and \f$ \delta_d = 0.05 \f$,
* while, as parameters of the Robust Soliton Distribution \f$ c = 0.01 \f$ and \f$ \delta = 0.05 \f$.
*
* @param K number of sensing nodes
* @param d degree chosen from Robust Soliton Distribution
* @return L Lagrangian function computed for K and d given.
*/
double getL(int K, int d);
/**
* Algorithm to approximate the global minimum of the function \f$ L(x_d,\eta,\lambda) \f$.
* ### Step of the algorithm
*/
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

double acceptanceProbability(int currentS, int newS, double temp){
	if(newS < currentS){
		return 1.0;
	}
	return exp((currentS-newS)/temp);
}

double getL(int K, int d, double x){
	double lambda = 1;
	double eta = 1;
	double c = 0.01;
	double delta = 0.05;
	double deltad = 0.05;

	RobustSoliton mu = RobustSoliton(c, delta, K);
	double first_term = 0;
	for(int i = 1; i<=K; i++){
		first_term += x*i*mu.get(i);
	}
	double newL = first_term + eta*(1-x) + lambda*(deltad - getViolationProb(d,x,K));

	return newL;
}

double simulatedAnnealing(int K, int d){
	double Tmax = 10000;
	double Tmin = 0.01;
	double coolingRate = 0.003;
	/** -# Set the best solution for L to an high value. */
	double bestL = 10000;
	double bestx = 0;
	double T;
	double currentx = 0;
	double currentL = 0;
	srand (time(NULL));
	for(int i = 0; i < 20; i++){
		/** -# Set temperature T to an initial value \f$ T_{max} \f$ and pick randomly a value for \f$ x_d \f$ */
		T = Tmax;
		currentx = (rand() % 5000)/1000.0;
		/** -# Compute the value of \f$ L(x_d,\eta,\lambda) \f$. */
		currentL = getL(K, d, currentx);
		while(T>Tmin){
			/** -# Add to the current \f$ x_d \f$ a randomly (but little enough) number and compute \f$ L(x_d,\eta,\lambda) \f$ for the new \f$ x_d \f$.*/
			double newx = currentx +(rand() % 10000)/10000.0;
			double newL = getL(K, d, newx);
			/** -# Decide if the new solution is valid through the function acceptanceProbability */
			/** -# If new solution is accepted then update the current solution, otherwise discard the new solution */
			if (acceptanceProbability(currentL, newL, T) > rand()){
				currentL = newL;
				currentx = newx;
			}
			/** -# Decrement T of a little value coolingRate. */
			T *= 1 - coolingRate;
			/** -# Repeat this process from 4) until T is close enough to 0 */
		}
		/** -# If the final \f$ L(x_d,\eta,\lambda) \f$ is better (less then) the best one, update the best solution */
		if(currentL < bestL){
			bestL = currentL;
			bestx = currentx;
		}
		/** -# Repeat the process from 2) for a fixed number of iteration */
	}
	return bestx;
}
