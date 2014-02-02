/*
 * RBFKernel.h
 *
 *  Created on: Dec 13, 2013
 *      Author: antonio
 */

#ifndef RBFKERNEL_H_
#define RBFKERNEL_H_

#include "IKernel.h"

class RBFKernel: public IKernel {
public:
	RBFKernel();
	virtual ~RBFKernel();

	CGAL::MP_Float K(arma::vec X1, arma::vec X2);

private:
	double sigma;

	CGAL::MP_Float computeGamma();
	CGAL::MP_Float squaredEuclidean(arma::vec X1, arma::vec X2);
};

#endif /* RBFKERNEL_H_ */
