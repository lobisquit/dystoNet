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

double acceptanceProbability(double F, double FNew, double T){
	double delta = F - Fnew;
	if(delta <= 0){
		return 1.0;
	}
	return exp(-delta/T);
}

double objectiveFunction(int K, double[] x){
	double c = 0.01;
	double delta = 0.05;

	RobustSoliton mu = RobustSoliton(c, delta, K);
	double objF = 0;
	for(int d = 1; d <= K; d++){
		objF += x[d] * d * mu.get(d);
	}
	return objF;
}

double[] getInitialSolution();

double[] getNeighbor(double[] xd, double T, int K) {
	// check base case: worrysome infinite recursion
	double newXd[K];

	for(int d=0, d<K; d++){
		newXd[K] = xd[K] + rand(-1, 1); // put real function, not pseudocode
	}
	if(respectConstraints(newXd)) {
		return newXd;
	}
	// repeat search if constraints are not met
	return respectConstraints(newXd);
}

boolean respectConstraints(double[] candidateXd, int K, int N, double deltad) {
	double E = objectiveFunction(candidateXd);
	for (int d=0; d++; d<K) {
		// check xd >= 1
		if(candidateXd[d] < 1) {
			return false;
		}
		// check violation probability > delta
		double p = 1 - (1 - candidateXd[d] * d / (N * E))^(N * E / K);
		failureProb = 1 - binomial_CDF(K, d, p); // check d (start from 0)
		if(failureProb>deltad) {
			return false;
		}
	}
	return true;
}


double simulatedAnnealing(int K, int N, int d){
	srand (time(NULL));
	int TI = 1500;
	double xd[] = getInitialSolution();
	double F = objectiveFunction(xd);
	double T = TI;
	int numIteration = 0;
	int maxIteration = 100000;
	double coolingRate = 0.99;
	double bestx = 0;
	double bestF = 10000000;
	int TL;
	double xdNew[K], FNew, deltaCost, q;
	while(numIteration < maxIteration){
		TL = (int) 750000/T;
		for(int i = 0; i < TL; i++){
			xdNew = getNeighbor(xd, T);
			FNew = objectiveFunction(xDNew);

			if(FNew < bestF){
				bestx = xdNew;
				bestF = FNew;
			}

			if (acceptanceProbability(F, FNew, T) >= (rand() % 1000)/1000.0){
				xd = xdNew;
			}
			numIteration++;
		}
		T = coolingRate*T;
	}

	return bestx;
}
