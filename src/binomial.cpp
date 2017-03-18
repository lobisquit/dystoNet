#include <math.h>
#include "binomial.h"

double ln_gamma(double x) {
	/**
	* ------------------------
	* Gamma integral is non-trivial, so it is approximated using this identity,
	* splitted into two pieces for convenience
	*
	* \f$ \Gamma(x) = A(x) B(x) \f$ for \f$ |x| \gt 0 \f$<br>
	* with \f$ ~ A(x) = (x-\frac 1 2+\gamma)^{x-\frac 1 2} e^{-(x-\frac 1 2+\gamma)} \f$ <br>
	* and \f$ ~ B(x) = \sqrt{2\pi} \left[ c_0 + \frac {c_1} {(x-1)+1} + ... + \frac {c_N} {(x-1)+N} + \epsilon \right] \f$ <br>
	* for certain constants \f$ \gamma, \epsilon, ~c_1, ~..., ~c_N \f$.
	*
	* Chosing \f$ N=6 \f$, we have that \f$ \gamma = 5 \f$: this choiche bounds the error
	* term to be \f$ |\epsilon| < 2 \cdot 10^{-10} \f$.
	*/

	/** - logarithm of A(x) is computed using \f$ \gamma = 5 \f$ */
	static double gamma = 5;
	double ln_first_term = (x - 0.5) * log(x + gamma - 0.5) - (x + gamma - 0.5);

	/** - B(x) is obtained using precomputed \f$ c_i \f$ values, optimal for \f$ N=6 \f$ */
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

	/** The two terms are then combined summing their logarithms.*/
	// square radix of double pi is precomputed for efficiency
	double sqrt2pi = 2.5066282746310005;
	return ln_first_term + log(sqrt2pi * second_term);
}
