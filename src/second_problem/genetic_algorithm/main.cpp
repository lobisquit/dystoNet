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
#include "genetic_algorithm.h"
#include "functionCSV.h"

using namespace std;
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

int main() {
	int K = 1000;
	int N = 2000;

	RobustSoliton rs = RobustSoliton(
		/* c     */ 0.01,
		/* delta */ 0.05,
		            K,
		            2);

	GeneticAlgorithm GA = GeneticAlgorithm(
		/* K */ K,
		/* N */ N,
		/* robust_soliton */ &rs,
		/* max_failure_probability */ 0.05,
		/* num_generations */ 2.5e5,
		/** ATTENTION!!!! In this version, dim_population has to be integer multiple
		* of the inverse of the survival_rate */
		/* dim_population*/ 20,
		/* survival_rate */ 0.25
	);

	std::cout << GA << "\n";

	vector<double> best_redundancy = GA.run_search();

	// save result to CSV
	writeCSV(best_redundancy, "results/ADFC/GA-second.csv");
}
