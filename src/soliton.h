#ifndef _SOLITON_H_
#define _SOLITON_H_

class IdealSoliton {
	public:
		/**
			Ideal Soliton parameter: distribution is defined then in [0, K]
		*/
		int K;

		/**
		* Build Ideal Soliton generator object
		* @param _K highest integer for which the distribution is defined
		*/
		IdealSoliton(int _K);

		/**
		* Default constructor, with K set to 1000
		*/
		IdealSoliton();

		/**
		* Compute probability distribution given parameters
		* @param degree node degree
		* @return probability of having given degree
		*/
		double get(int degree);
};

class RobustSoliton {
	private:
		/**
		* Related Ideal Soliton distribution, employed in Robust Soliton computation
		*/
		IdealSoliton rho;

		/**
		* Normalization factor for Robust Soliton distribution
		*/
		double beta = 0;

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
		* Build Robust Soliton generator object
		* @param _c constant
		* @param _delta maximal failure probability
		* @param _K highest integer for which the distribution is defined
		*/
		RobustSoliton(double _c, double _delta, int _K);

		/**
		* Compute probability distribution given parameters
		* @param degree node degree
		* @return probability of having given degree
		*/
		double get(int degree);
};

#endif
