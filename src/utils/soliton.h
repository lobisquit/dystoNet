#ifndef SOLITONH
#define SOLITONH

#include <sstream>

class Distribution {
	public:
		Distribution() {};

		/**
		* Compute probability distribution given parameters
		* @param degree node degree
		* @return probability of having given degree
		*/
		virtual double get(int degree) = 0;
};

class IdealSoliton : public Distribution {
	public:
		/**
		* Ideal Soliton parameter: distribution is defined then in [0, K]
		*/
		int K;

		/**
		* Build Ideal Soliton generator object
		* @param K highest integer for which the distribution is defined
		*/
		IdealSoliton(int K);

		/**
		* Default constructor, with K set to 1000
		*/
		IdealSoliton();

		double get(int degree);
};

class RobustSoliton : public Distribution {
	private:
		/**
		* Related Ideal Soliton distribution, employed in Robust Soliton computation
		*/
		IdealSoliton rho;

		/**
		* Normalization factor for Robust Soliton distribution
		*/
		double beta;

		/**
		* Parameter precomputed and stored at creation given c, delta, K
		*/
		double R;

		/**
		* Compute function tau
		* @param degree node degree
		* @return tau function computed in given input integer
		*/
		double get_tau(int degree);

	public:
		/** Robust Soliton constant */
		double c;

		/** Robust Soliton maximal failure probability */
		double delta;

		/** Ideal Soliton parameter: distribution is defined then in [0, K] */
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
		* @param K highest integer for which the distribution is defined
		*/
		RobustSoliton(double c, double delta, int K);

		double get(int degree);
};

#endif
