/*
 * PolynomialKernel.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: antonio
 */

#include "PolynomialKernel.h"

PolynomialKernel::PolynomialKernel(int Q) {
	this->Q = Q;
	this->C = 1;
}

PolynomialKernel::~PolynomialKernel() {
	// TODO Auto-generated destructor stub
}

CGAL::MP_Float PolynomialKernel::K(arma::vec X1, arma::vec X2) {
	if(Q==0) return CGAL::MP_Float(1.0);
	CGAL::MP_Float partial(C+arma::as_scalar(X1.t()*X2));
	if(Q==1) return partial;
	for(int i=0; i<Q; i++){
		partial *= partial;
	}
	std::cout << "El innerProduct vale: " << partial << std::endl;
	return partial;
}
