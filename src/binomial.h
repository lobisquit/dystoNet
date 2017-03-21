/*!
* \file
* The algorithms applied here to compute all the special function employed
* are taken from the book <i>Numerical Recipes in C</i>, expecially section 5 and 6. <br>
* Source code in the book is taken as reference for our specific C++ implementation.
*
* William H. Press, Saul A. Teukolsky, William T. Vetterling, and Brian P. Flannery. 1992.
* Numerical Recipes in C (2nd Ed.): The Art of Scientific Computing.
* Cambridge University Press, New York, NY, USA.
*/

#include <math.h>

#ifndef _BINOMIAL_H_
#define _BINOMIAL_H_

/**
* Logarithm in base \f$ e \f$ of Euler gamma function, defined as
* \f$ ~ \Gamma(x) = \int_0^{+\infty} t^{x-1} e^{-t} dt\f$
*
* Since \f$ \Gamma(n+1) = n! ~\f$ for \f$~ n \in \mathbb{N} \f$, logatithm is taken
* not to overflow real number representation.
* @param x input value
* @return \f$ log_e \Gamma(x) \f$
*/
double ln_gamma(double x);

/**
* \f$ D(a, b, x) \f$, second term of our formulation of \f$ I_x(a, b) \f$,
* can be expressed as a continued fraction.
*
* \f$ D(a, b, x) = \frac {1} {1+} \frac {d_1} {1+} \frac {d_2} {1+} ... \f$
* @see betai(double a, double b, double x)
*
* @param x real number \f$\in [0, 1]\f$
* @param a generic real number
* @param b generic real number
* @return \f$ D(a, b, x) \f$
*/
double betacf(double a, double b, double x);

/**
* Regularized incomplete beta function is defined as
*
* \f$ I_x(a, b) =  \frac {B_x(a, b)} {B(a, b)} \f$ for \f$ x \in [0, 1]\f$
*
* with \f$ B_x(a, b) = \int_0^x t^{a-1} (1-t)^{b-1} dt ~\f$ and
* \f$~ B(a, b) = \frac {\Gamma(a) ~ \Gamma(b)} {\Gamma(a+b)} = \int_0^1 t^{a-1} (1-t)^{b-1} dt\f$
*
* @param x real number \f$\in [0, 1]\f$
* @param a generic real number
* @param b generic real number
* @return \f$ I_x(a, b)\f$
* @throws std::runtime_error if \f$ x \notin [0, 1] \f$
*/
double betai(double a, double b, double x);

/**
* Comulative distribution function for a binomial random variable, evaluated in integer \f$ k \f$.
*
* To perform the actual computation, the incomplete beta function is employed, given that, <br>
* for \f$ X \sim Bin(n, p), ~ CDF_X(k) = P[X \ge k] = \sum_{j=k}^n {n \choose j} p^j (1-p)^{n-j} = I_x(a, b) \f$
*
* @param n total number of trials
* @param k number of successful trials
* @param p success probability of single trial
* @return \f$ CDF_X(k) = P[X \ge k] \f$ if \f$ n \gt 0 \f$
* @return <i>NaN</i> if \f$ n \le 0 \f$
*/
double binomial_CDF(int n, int k, double p);

#endif
