#include <stdexcept>
#include <math.h>
#include "binomial.h"

double ln_gamma(double x) {
	/**
	* ------------------------
	* ### Algorithm
	* Gamma integral is non-trivial, so it is approximated using this serie,
	* splitted into two pieces for convenience
	*
	* \f$ \Gamma(x) = A(x) ~ B(x) \f$ for \f$ |x| \gt 0 \f$<br>
	* &ensp; with \f$ ~ A(x) = (x-\frac 1 2+\gamma)^{x-\frac 1 2} e^{-(x-\frac 1 2+\gamma)} \f$ <br>
	* &ensp; and \f$ ~ B(x) = \sqrt{2\pi} \left[ c_0 + \frac {c_1} {(x-1)+1} + ... +
	* \frac {c_N} {(x-1)+N} + \epsilon \right] \f$ <br>
	* for certain constants \f$ \gamma, \epsilon, ~c_1, ~..., ~c_N \f$.
	*
	* Choosing \f$ N=6 \f$ and optimal values of the \f$ c_i \f$ and \f$ \gamma \f$,
	* the error term is bounded \f$ |\epsilon| < 2 \cdot 10^{-10} ~\forall x > \mathbb{R}^+ \f$.
	*
	* ### Steps
	*/

	/** - Logarithm of A(x) is computed using \f$ \gamma = 5 \f$ */
	static double gamma = 5;
	double ln_first_term = (x - 0.5) * log(x + gamma - 0.5) - (x + gamma - 0.5);

	/** - \f$ B(x)\f$ is obtained using precomputed \f$ c_i \f$ values, optimal for \f$ N=6 \f$ */
	static double c[7] = {
		1.000000000190015,
		76.18009172947146,
		-86.50532032941677,
		24.01409824083091,
		-1.231739572450155,
		0.1208650973866179e-2,
		-0.5395239384953e-5
	};

	// increment B(x) using serie described above
	double second_term = c[0];
	for (int j = 1; j < 7; j++) {
		second_term += c[j] / (x + j - 1);
	}

	/** - The two terms are then combined summing their logarithms.*/
	// square radix of double pi is precomputed for efficiency
	double sqrt2pi = 2.5066282746310005;
	return ln_first_term + log(sqrt2pi * second_term);
}

double betai(double a, double b, double x) {
	double betacf(double a, double b, double x);
	double ln_gamma(double x);
	double first_term;

	// values of x outside [0, 1] lead to a function error
	if (x < 0.0 || x > 1.0) {
		throw std::runtime_error("x parameter = " + std::to_string(x) +
			" (p parameter of binomial) is out of [0, 1].");
	}
	/**
	* --------
	* ### Algorithm
	* Incomplete beta function is computed using this expression,
	* whose second term \f$ D(a, b, x) \f$ is approximated in the <i>betacf</i> routine.
	*
	* \f$ I_x(a, b) = \frac {x^a (1-x)^b} {a ~ B(a, b)} \cdot D(a, b, x) \f$ where
	* \f$ B(a, b) = \frac {\Gamma(a) \Gamma(b)} {\Gamma(a+b)} \f$
	*
	* ### Steps
	* - First term computation is straighforward from its definition.
	*/
	if (x == 0.0 || x == 1.0) {
		first_term = 0.0;
	}
	else {
		double log_complete_beta = ln_gamma(a) + ln_gamma(b) - ln_gamma(a + b);
		first_term = exp( a * log(x) + b * log(1.0 - x) - log_complete_beta);
	}
	/**
	* - Second term is multiplied then. To allow a fast computation of the
	* continued fraction approximation, \f$ x \f$ is kept below a certain threshold
	* \f$ \mu = \frac {a + 1} {a + b + 2}\f$. <br>
	* Terms for \f$ x \ge \mu \f$ are obtained using incomplete beta simmetry below.
	* Note that roles of \f$ a \f$ and \f$ b \f$ are swapped in this case.
	*
	* \f$ I_x(a, b) = I_{1-x}(b, a) \f$
	*/
	if (x < (a + 1.0) / (a + b + 2.0)) {
		return first_term * betacf(a, b, x) / a;
	}
	else {
		return 1.0 - first_term * betacf(b, a , 1.0 - x) / b;
	}
	/**
	* @see betacf(double a, double b, double x)
	* @see ln_gamma(double x)
	*/
}

/**
* Maximum number of iterations to compute continued fraction.
* @see betacf(double a, double b, double x)
*/
#define MAXIT 100

/**
* Smallest increment allowed in Lentz approximation iterations.
* @see betacf(double a, double b, double x)
*/
#define EPS 3.0e-7

/**
* Smallest non-zero positive float, employed in Thompson and Barnett modification
* of Lentz approximation method.
* @see betacf(double a, double b, double x)
*/
#define TINY 1.0e-30

double betacf(double a, double b, double x) {
	/**
	* ----------------------
	* ### Algorithm
	* Modified recursive Lentz method is employed to compute such fraction in an efficient way. <br>
	* It is based on the fact that, given \f$ f_j \f$ the fraction computed until the \f$ j \f$-th term,
	*
	* \f$ f_j = f_{j−1} C_j D_j \f$, <br>
	* &ensp; with \f$ C_j = \frac {A_j} {A_{j-1}} \f$, \f$ D_j = \frac {B_j} {B_{j-1}} \f$, <br>
	* &ensp; and \f$ D_j = \frac {1} {1 + d_j D_{j−1}} \f$, \f$ C_j = 1 + \frac {d_j} {C_{j−1}} \f$
	*/
	// For performance, some relevant constant quantities are precomputed here
	double a_plus_b = a + b;
	double a_plus_one = a + 1.0;
	double a_minus_one = a - 1.0;

	/**
	* ### Steps
	*
	* First step of Lentz’s method sets initial values \f$ C_0=1\f$ and \f$ D_0=0 \f$.
	* For this fraction, it can be seen that \f$ f_0=d_0=1 \f$.
	* Using this conditions, values of recurrence are initialized for \f$ j=1 \f$.
	*/
	double dj = - a_plus_b * x / a_plus_one;
	double Dj = 1.0 + dj;
	if (fabs(Dj) < TINY) {
		// avoid case Dj=0
		Dj = TINY;
	}
	Dj = 1.0 / Dj;

	double Cj = 1.0;

	// f0 term
	double fj = Dj;

	/**
	* The loop starts for \f$ j=2 \f$. For this specific fraction,
	* terms \f$ d_1, d_2, ... \f$ can be expressed this way
	*
	* \f$ d_j = \left\{\begin{matrix}
	* 		-\frac {(a+m)(a+b+m)x} {(a+2m)(a+2m+1)} & \text{ for } j=2m+1\\
	*		-\frac {m(b−m)x} {(a+2m−1)(a+2m)} & \text{ for } j=2m
	*		\end{matrix}\right. \f$
	*/
	int m, m_mult_2;
	double deltdj;

	for (m = 1; m <= MAXIT; m++) {
		// Even step of the loop: case j = 2m
		m_mult_2 = 2 * m;

		dj = m * (b - m) * x / ((a_minus_one + m_mult_2) * (a + m_mult_2));

		// compute Dj according to its definition
		Dj = 1.0 + dj * Dj;
		/**
		* Since \f$ D_j \f$ and \f$ C_j \f$ values can reach zero, leading to
		* inconsistent results, we employ Thompson and Barnett modification that
		* convert such zeros to the smallest number representable in the machine
		* at every approximation step.
		*/
		if (fabs(Dj) < TINY) { Dj = TINY; }
		Dj = 1.0 / Dj;

		// compute Cj according to its definition
		Cj = 1.0 + dj / Cj;
		if (fabs(Cj) < TINY) { Cj = TINY; }

		fj *= Dj * Cj;

		// Next step of the recurrence: case j = 2m + 1
		dj = -(a + m) * (a_plus_b + m) * x / ((a + m_mult_2) * (a_plus_one + m_mult_2));

		// compute Dj according to its definition
		Dj = 1.0 + dj * Dj;
		if (fabs(Dj) < TINY) { Dj = TINY; }
		Dj = 1.0 / Dj;

		// compute Cj according to its definition
		Cj = 1.0 + dj / Cj;
		if (fabs(Cj) < TINY) { Cj = TINY; }

		/**
		* At the end of each iteration, \f$ \Delta_j = \frac {f_j} {f_{j-1}} \f$
		* is computed and program exits the loop if the increment is smaller than EPS
		* threshold.
		*/
		deltdj = Dj * Cj;
		fj *= deltdj;

		if (fabs(deltdj - 1.0) < EPS) { break; }
	}
	return fj;
}

#undef MAXIT
#undef EPS
#undef TINY

double binomial_CDF(int n, int k, double p) {
	return betai(k, n-k + 1, p);
}
