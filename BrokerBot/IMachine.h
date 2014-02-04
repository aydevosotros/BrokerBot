#ifndef IMACHINE_H_
#define IMACHINE_H_

#include "Sample.h"

class IMachine {
public:
	virtual void setParameters(char *argv[])=0;
	virtual void loadTrainingSet(std::string filename)=0;
	virtual void loadTestingSet(std::string filename)=0;
	virtual void loadInput(std::string filename)=0;
	virtual void run()=0;
	virtual void train()=0;
	virtual void test()=0;
	virtual double predict(Sample input)=0;
	virtual void clearTrainingSet()=0;
	virtual void loadThetas()=0;
};

#endif

