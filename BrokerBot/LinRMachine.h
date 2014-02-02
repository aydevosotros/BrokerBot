#ifndef LINRMACHINE_H_
#define LINRMACHINE_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "armadillo"

#include "IMachine.h"

class LinRMachine : public IMachine {
public:
	LinRMachine();
	virtual ~LinRMachine();

	void setParameters(char *argv[]);
	void loadTrainingSet(std::string filename);
	void loadTestingSet(std::string filename);
	void loadInput(std::string filename);
	void run();
	void train();
	void test();
	double predict(Sample input);
	void clearTrainingSet();

private:
	//ATRIBUTOS

	//Atributos pasados por parámetro
	int C_executionMode;
	std::string C_trainingFile;
	std::string C_testingFile;
	std::string C_inputFile;

	std::string C_thetaFileName;

	//Atributos cargados desde archivo
	std::vector<Sample> C_trainingSet;
	std::vector<Sample> C_testingSet;
	Sample C_input;
	std::vector<double> C_theta;

	//Usados en el training
	int C_nFeatures;
	std::vector<std::vector<double> > C_X;
	std::vector<double> C_y;

	//Usados en el testing
	std::vector<double> C_actualY;
	std::vector<double> C_predictedY; //Estos son los que calculamos nosotros. Precission y recall

	//Usados en los dos tipos de training
	int C_trainingType; //1 para normal y 2 para gradiente
	int C_iterations; //Numero de iteraciones, en el gradiente
	double C_alpha; //Alpha del gradiente

	double C_treshold;

	//FUNCIONES

	// Internal functions
	double cost(std::vector<double> theta);
	void trainByGradient(int iter, double alpha);
	void trainByNormal();
	double h(std::vector<double> x);

	// Auxiliar functions
	void fillX();
	void fillTheta();
	void fillY();
	void fillActualY();

	void loadThetas(std::string filename);
};

#endif /* LINRMACHINE_H_ */
