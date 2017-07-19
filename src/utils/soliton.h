#ifndef SOLITONH
#define SOLITONH

#include <sstream>
#include <random>

using namespace std;

class Distribution {
	protected:
		/** Random number generator, needed for realizations */
		mt19937 rng;

		/**
		* Vector of length K, where element \f$ p(i) = probabilities[i-1] \f$:
		* probability is non-zero only for \f$ i \in [1, K] \f$
		*/
		vector<double> probabilities;

	public:
		Distribution(int seed);

		Distribution(vector<double> probabilities, int seed);

		/**
		* Get a realization of the random variable described by
		* current Distribution
		* @return realization
		*/
		int realization();

		/**
		* Get expected value of random variable described by Distribution
		* @return expected value
		*/
		virtual double exp();

		/**
		* Compute probability Distribution (given parameters)
		* @param degree node degree
		* @return probability of having given degree
		*/
		double get(int degree);

		/**
		* @return retrieve rng for use outside of distribution
		*/
		mt19937 get_rng() {
			return this->rng;
		}
};

class IdealSoliton : public Distribution {
	public:
		/**
		* Build Ideal Soliton generator object
		* @param K highest integer for which the Distribution is defined
		*/
		IdealSoliton(int K, int seed);

		using Distribution::get;
		using Distribution::realization;
		using Distribution::exp;
};

class RobustSoliton : public Distribution {
	private:
		/**
		* Compute function tau
		* @param degree node degree
		* @return tau function computed in given input integer
		*/
		double get_tau(int degree, double R);

	public:
		/** Robust Soliton constant */
		double c;

		/** Robust Soliton maximal failure probability */
		double delta;

		/** Ideal Soliton parameter: Distribution is defined then in [0, K] */
		int K;

		/**
		* Make RobustSoliton objects printable, for debug
		*/
		friend std::ostream& operator<<(std::ostream &strm, RobustSoliton &obj) {
			strm << "<RobustSoliton("
				"c="      << obj.c      << ", " <<
				"delta="  << obj.delta  << ", " <<
				"K="      << obj.K      << ")>";
			return strm;
		}

		/**
		* Build Robust Soliton generator object
		* @param c constant
		* @param delta maximal failure probability
		* @param K highest integer for which the Distribution is defined
		*/
		RobustSoliton(double c, double delta, int K, int seed);

		using Distribution::get;
		using Distribution::realization;
		using Distribution::exp;
};

class OverheadRobustSoliton : public RobustSoliton {
	private:
		/**
		* Overhead coefficients
		*/
		vector<double> x;

	public:
		/**
		* Make RobustSoliton objects printable, for debug
		*/
		friend std::ostream& operator<<(std::ostream &strm, OverheadRobustSoliton &obj) {
			strm << "<OverheadRobustSoliton("
				"c="      << obj.c      << ", " <<
				"delta="  << obj.delta  << ", " <<
				"K="      << obj.K      << ")>";
			return strm;
		}

		/**
		* Build Robust Soliton generator object
		* @param c constant
		* @param delta maximal failure probability
		* @param K highest integer for which the Distribution is defined
		*/
		OverheadRobustSoliton(
			vector<double> x,
			double c,
			double delta,
			int K,
			int seed);

		int realization();
		double get(int degree);
		double exp();
};


#endif
