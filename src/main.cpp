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
#include <random>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include "soliton.h"
#include "binomial.h"
#include "heuristicSearch.h"
#include "firstProblem.h"
#include "jumping_ball.h"

/**
* Probability to accept the new solution calculated in the last step of the Simulated Annealing.
* ### Algorithm
* If the new solution \f$ S_{new} \f$ is better (less then) the current solution \f$ S_{current} \f$, accept the new solution w.p. 1,
* otherwise accept the new solution w.p. \f$ e^{(S_{new} - S_{current})/T} \f$.
* In this last case the \f$ S_{new} \f$ will be worse (greater then) or equal to \f$ S_{current} \f$
*/
// double acceptanceProbability(double F, double FNew, double T);
// double objectiveFunction(int K, double* x);
// double* getInitialSolution(int K);
// double* getNeighbor(double* xd, int K, int N);
// bool respectConstraints(double* candidateXd, int K, int N);
/**
* Algorithm to approximate the global minimum of the function \f$ L(x_d,\eta,\lambda) \f$.
* ### Step of the algorithm
*/
// double simulatedAnnealing(int K, int N);
//
// std::random_device rd;  //Will be used to obtain a seed for the random number engine
// std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

void print_array(double* x, int K) {
	std::cout << "x = [";
	for(int i=0; i<K-1; i++) {
		std::cout << x[i] << ", ";
	}
	std::cout << x[K-1] << "]\n";
}

int main() {
	int K = 1000;
	int N = 2000;
	RobustSoliton rs = RobustSoliton(
		/* c 		*/	0.01,
		/* delta 	*/	0.05,
						K);

	double x[K];
	// TB.run_search(x);
	// print_array(x, K);

	JumpingBall SA = JumpingBall(
				/* K 						*/ K,
				/* N 						*/ N,
				/* robust_soliton 			*/ &rs,
				/* max_failure_probability 	*/ 0.05,
				/* starting_temperature 	*/ 100.0,
				/* cooling_rate 			*/ 0.9,
				/* max_iterations 			*/ 100000,
				/* steps_coefficient 		*/ 5e4);

	// double new_x[K];
	double x_farlocco[K];
	for(int i=0; i<K; i++) {
		x_farlocco[i] = 1;
	}

	std::cout << SA << "\n";

	SA.get_initial_solution(x);
	std::cout << "Starting from score "
		<< (SA.objective_function(x)/SA.objective_function(x_farlocco)) << " with point ";
	print_array(x, 10);

	SA.run_search(x);
	std::cout << "Arriving to   score "
		<< (SA.objective_function(x)/SA.objective_function(x_farlocco)) << " with point ";
	print_array(x, 10);

}

//
// double acceptanceProbability(double F, double FNew, double T){
// 	double delta = F - FNew;
// 	if(delta <= 0){
// 		return 1.0;
// 	}
// 	return exp(-delta/T);
// }
//
// double objectiveFunction(int K, double* x){
// 	double c = 0.01;
// 	double delta = 0.05;
//
// 	RobustSoliton mu = RobustSoliton(c, delta, K);
// 	double objF = 0;
// 	for(int d = 1; d <= K; d++){
// 		objF += x[d] * d * mu.get(d);
// 	}
// 	return objF;
// }
//
// double* getInitialSolution(int K){
// 	double x[K];
// 	for(int d = 0; d < K; d++){
// 		x[d] = 10;
// 	}
//
// 	return x;
// }
//
// double* getNeighbor(double* xd, int K, int N) {
// 	// check base case: worrysome infinite recursion
// 	double newXd[K];
// 	std::uniform_real_distribution<> increment(-1, 1);
//
// 	for(int d=0; d<K; d++){
// 		newXd[K] = xd[K] + increment(gen);
// 	}
//
// 	if(respectConstraints(newXd, K, N)) {
// 		return newXd;
// 	}
// 	// repeat search if constraints are not mets
// 	return getNeighbor(xd, K, N);
// }
//
// bool respectConstraints(double* candidateXd, int K, int N) {
// 	double deltad = 0.05, failureProb;
// 	double E = objectiveFunction(K, candidateXd);
// 	for (int d=0; d++; d<K) {
// 		// check xd >= 1
// 		if(candidateXd[d] < 1) {
// 			return false;
// 		}
// 		// check violation probability > delta
// 		double p = 1 - pow((1 - candidateXd[d] * d / (N * E)),(N * E / K));
// 		failureProb = 1 - binomial_CDF(K, d, p); // check d (start from 0)
// 		if(failureProb>deltad) {
// 			return false;
// 		}
// 	}
// 	return true;
// }
//
//
// double simulatedAnnealing(int K, int N){
// 	std::uniform_real_distribution<> randU(0, 1);
//
// 	int TI = 1500;
// 	double *xd = getInitialSolution(K);
// 	double F = objectiveFunction(K, xd);
// 	double T = TI;
// 	int numIteration = 0;
// 	int maxIteration = 100000;
// 	double coolingRate = 0.99;
// 	double bestF = 10000000;
// 	int TL;
// 	double *xdNew, *bestx, FNew, deltaCost, q;
// 	while(numIteration <= maxIteration){
// 		TL = (int) 750000/T;
// 		for(int i = 0; i < TL; i++){
// 			xdNew = getNeighbor(xd, K, N);
// 			FNew = objectiveFunction(K, xdNew);
//
// 			if(FNew < bestF){
// 				bestx = xdNew;
// 				bestF = FNew;
// 			}
//
// 			if (randU(gen) < acceptanceProbability(F, FNew, T)){
// 				xd = xdNew;
// 			}
// 			numIteration++;
// 		}
// 		T = coolingRate*T;
// 	}
//
// 	return *bestx;
// }
