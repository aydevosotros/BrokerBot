/*
 * LinearKernel.h
 *
 *  Created on: Dec 11, 2013
 *      Author: antonio
 */

#ifndef LINEARKERNEL_H_
#define LINEARKERNEL_H_

#include "IKernel.h"
#include <CGAL/MP_Float.h>

class LinearKernel: public IKernel {
public:
	LinearKernel();
	virtual ~LinearKernel();

	CGAL::MP_Float K(arma::vec X1, arma::vec X2);
};

#endif /* LINEARKERNEL_H_ */
