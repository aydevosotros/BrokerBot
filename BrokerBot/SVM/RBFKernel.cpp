/*
 * RBFKernel.cpp
 *
 *  Created on: Dec 13, 2013
 *      Author: antonio
 */

#include "RBFKernel.h"

RBFKernel::RBFKernel() {
	// TODO Auto-generated constructor stub
	this->sigma = 1.0;
}

RBFKernel::~RBFKernel() {
	// TODO Auto-generated destructor stub
}

CGAL::MP_Float RBFKernel::K(arma::vec X1, arma::vec X2) {
//	std::cout << "X1:" << std::endl << X1 << std::endl << "X2: " << std::endl << X2;
	CGAL::MP_Float value(std::exp(CGAL::to_double(computeGamma()*squaredEuclidean(X1, X2))));
	return value;
}

CGAL::MP_Float RBFKernel::computeGamma() {
//	std::cout << "El valor de sigma es: " << CGAL::MP_Float(-1.0/2.0*std::pow(sigma, 2)) << std::endl;
	return CGAL::MP_Float(-1.0/2.0*std::pow(sigma, 2));
}

CGAL::MP_Float RBFKernel::squaredEuclidean(arma::vec X1, arma::vec X2) {
	double sum = 0.0;
//	#pragma omp parallel for
	for(int i=0; i< X1.size(); i++){
		sum+=std::pow(X1.at(i)-X2.at(i),2);
//		std::cout << "Para el elemento " << i << " la distancia suma: " << sum << std::endl;
	}
//	std::cout << "Y el valor de la distancia cuadrada es: " << sum << std::endl;
	return CGAL::MP_Float(sum);
}
