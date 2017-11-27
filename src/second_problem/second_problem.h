#include <sstream>

#include "soliton.h"
#include "heuristicSearch.h"

#ifndef _SECOND_PROBLEM_H_
#define _SECOND_PROBLEM_H_

struct individual{
	vector<double> values;
	vector<double> v_prime;
	double obj_function = 0;
};

/*!
* Provide solution methods for Lin's paper problem in equation 18
* (second problem in our study)
* \authors Enrico Lovisotto, Davide Peron, Federico Mason
*/

class SecondProblem : public HeuristicSearch {
	public:
		/** Random numbers generator */
		mt19937 rng;

		/** Number of sensor nodes */
		int K;

		/** Total number of nodes */
		int N;

		/** Maximal probability that a node receive less packets than we wanted */
		double max_failure_probability;

		/** Robust Soliton reference distribution */
		RobustSoliton* robust_soliton;

		SecondProblem(
			int K,
			int N,
			RobustSoliton* robust_soliton,
			double max_failure_probability);

		/*!
		* Custom overload of << operator, to print debug info
		*/
		friend std::ostream& operator<<(std::ostream &strm, SecondProblem &obj) {
			strm << "<SecondProblem("
				"K="                        << obj.K                       << ", " <<
				"N="                        << obj.N                       << ", " <<
				"max_failure_probability="  << obj.max_failure_probability << ", " <<
				"robust_soliton="           << *obj.robust_soliton         <<
				")>";
			return strm;
		}

		/*!
		* Computes in a non-optimized way the objective function for second problem.
		* This method is really slow!
		* \param v Vector on which the objective function is computed.
		*/
		double objective_function(vector<double> v);
		/*!
		* Computes in an approximate way the objective function for second problem.
		* This method is faster than \a objective_function but gives an approximate solution.
		* \param v Vector on which the approximate objective function is computed.
		*/
		individual approximate_objective_function(vector<double> v);
		/*!
		* Computes the objective function for second problem in a very optimized way.
		* It exploit the last objective function computed updating only the terms of the sum
		* that change changing two component of the original vector.
		* This method is \a O(K), very faster than \a objective_function but it supposes a pre-existent
		* objective_function.
		* \param old_individual Original vector with its objective function pre-computed
		* \param v Original vector with 2 perturbations, vector on which the new objective function is computed
		* \param first_d First component perturbated
		* \param second_d Second component perturbated
		* \return Individual containing the perturbated vector and the updated objective function.
		*/
		individual update_objective_function(individual old_individual, vector<double> v, int first_d, int second_d);

		/*!
		* Create a random initial solution to start the searching algorithms.
		*	\return Initial random solution
		*/
		vector<double> get_initial_solution();

		/*!
		* Check if the input vector respects the constraints required in Equation 18 (Lin's Paper)
		*/
		bool respect_constraints(vector<double> candidate_v);

		/**
		* Normalizes the input vector
		*	\return Normalized Vector
		*/
		vector<double> normalize(vector<double> v);

		/**
		* Search for a neighbour solution, possible with a better objective function.
		* This function is implemented in lower classes
		* \return Neighbour solution
		*/
		vector<double> get_neighbour(vector<double> v);
		/**
		* Computes the acceptance probability used to choose a neighbour function
		* with a worst objective function.
		* This function is implemented in lower classes
		* \return Acceptance probability
		*/
		double acceptance_probability(vector<double> old_v, vector<double> new_v);
		/**
		* Search an optimal solution for the problem starting from an initial random
		* solution and using an heuristic algorithm.
		* This function is completely different for each heuristic algorithm, so is
		* implemented in the correspondent algorithm's class.
		* \return Sub-optimal solution and its objective function
		*/
		vector<double> run_search(string progress_file_name);
};

#endif
