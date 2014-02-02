/*
 * LinearKernel.cpp
 *
 *  Created on: Dec 11, 2013
 *      Author: antonio
 */

#include "LinearKernel.h"

LinearKernel::LinearKernel() {
	// TODO Auto-generated constructor stub

}

LinearKernel::~LinearKernel() {
	// TODO Auto-generated destructor stub
}

CGAL::MP_Float LinearKernel::K(arma::Col<double> X1, arma::Col<double> X2) {
//	std::cout << "Calculo el kernel como producto de: " << X1 << " " << X2 << std::endl;
	return CGAL::MP_Float(arma::as_scalar(X1.t()*X2));
}

