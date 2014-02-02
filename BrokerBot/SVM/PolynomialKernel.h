/*
 * PolynomialKernel.h
 *
 *  Created on: Dec 12, 2013
 *      Author: antonio
 */

#ifndef POLYNOMIALKERNEL_H_
#define POLYNOMIALKERNEL_H_

#include "IKernel.h"

class PolynomialKernel: public IKernel {
public:
	PolynomialKernel(int Q);
	virtual ~PolynomialKernel();

	CGAL::MP_Float K(arma::vec X1, arma::vec X2);

private:
	int Q;
	int C;
};

#endif /* POLYNOMIALKERNEL_H_ */
