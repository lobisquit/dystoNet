/*!
* \file
* Author: Davide Peron
*
* Provides a set of function useful to implement Exact Decentralized Fountain Codes
*/

#include <cmath>

#ifndef _EDFC_H_
#define _EDFC_H_

/**
* Probability that at least one source block is received from the \f$ i_{th} \f$ sensing node given that
* the chosen degree for this node is \f$ d\f$.
* More formally, \f$ p = Pr(Y_i = 1 | X = d) \f$.
* This is calculated \f$ N \rightarrow \infty\f$ where N is the total number of nodes
* in the network.
* We have that \f$ Pr(Y_i = 1 | X = d) = 1 - e^{-x_dd/K} \f$.
*
* @param d chosen degree for node \f$ i_{th} \f$
* @param xd redundancy coefficient, each node receives \f$ x_d \cdot d \f$ source blocks on average
* @param K number of sensing node in the network
* @return p as has been previously defined
*/
double getp(int d, double xd, int K);

/**
* Violation Probability is defined as \f$ Pr(Y < d | X = d) \f$.
* Y is the actual degree i.e. the actual number of distinct source blocks stopping at a node
* and is defined as \f$ \sum\limits_{i=1}^K Y_i \f$. Since \f$ Y_i \f$ are iid Bernoulli r.v.
* Y is a Binomial distribution.
* So we have \f$ Pr(Y < d | X = d) = \sum\limits_{j=0}^{d-1} {K \choose j} p^j(1-p)^{K-j} \f$
*
* @param d chosen degree thanks to Robust Solition distribution
* @param xd redundancy coefficient
* @param K number of sensing node in the network
* @return Violation probability
*/
double getViolationProb(int d, double xd, int K);

#endif
