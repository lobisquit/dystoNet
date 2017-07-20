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
#include "simulated_annealing.h"

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
// double GeneticAlgorithm(int K, int N);
//
// std::random_device rd;  //Will be used to obtain a seed for the random number engine
// std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
//
// void print_array(double x, int K) {
// 	std::cout << "x = [";
// 	for(int i=0; i<K-1; i++) {
// 		std::cout << x[i] << ", ";
// 	}
// 	std::cout << x[K-1] << "]\n";
// }

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K, 1);

	SimulatedAnnealing SA = SimulatedAnnealing(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* starting_temperature */ 150.0,
		/* cooling_rate */ 0.9,
		/* max_iterations */ 500000,
		/* steps_coefficient */ 5e4,
		/* acceptance_coefficient */ 200
	);

	vector<double> no_redundancy(K, 1);

	std::cout << SA << "\n";

	vector<double> best_redundancy = SA.run_search();
		std::cout << "Arriving to   score "
			<< (
				SA.objective_function(best_redundancy) / SA.objective_function(no_redundancy)
			) << "\n";

	// std::cout << GA << "\n";
	// GA.run_search(x);

	// std::cout << "Starting from score "
	// 	<< (GA.objective_function(x)/GA.objective_function(x_farlocco)) << " with point ";
	// print_array(x, 10);

	// GA.run_search(x);
	// std::cout << "Arriving to   score "
	// 	<< (GA.objective_function(x)/GA.objective_function(x_farlocco)) << " with point ";
	// print_array(x, 10);

}
