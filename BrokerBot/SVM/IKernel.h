/*
 * IKernel.h
 *
 *  Created on: Dec 11, 2013
 *      Author: antonio
 */

#ifndef IKERNEL_H_
#define IKERNEL_H_

#include "armadillo"
#include <CGAL/MP_Float.h>

class IKernel{
public:
	virtual CGAL::MP_Float K(arma::vec X, arma::vec X2) = 0;

};



#endif /* IKERNEL_H_ */
