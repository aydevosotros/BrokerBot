#ifndef NNMACHINE_H_
#define NNMACHINE_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>


#include "armadillo"

#include "Utils.h"
#include "IMachine.h"

//For optimization
#include "stdafx.h"
#include "optimization.h"

using namespace alglib;

typedef arma::mat M2;



class NNMachine: public IMachine {
public:
	NNMachine();
	virtual ~NNMachine();

	void setParameters(char *argv[]);
	void loadTrainingSet(std::string filename);
	void loadTestingSet(std::string filename);
	void loadInput(std::string filename);
	void train();
	void run();
	void test();
	double predict(Sample input);
	void clearTrainingSet();

	void loadThetas();

private:
	int executionMode;
	std::string trainingFile;
	std::string testFile;

	//Aguilañadidos
	std::string inputFile;
	std::string thetasFileName;
	Sample input;

	int L; //Número de capas
	int nFeatures;
	double lambda;
	double alpha;
	int iteraciones;
	std::vector<int> actualY;
	std::vector<int> predictedY;
	std::vector<int> s_l; //número de nodos por capa
	std::vector<Sample> trainingSet; //Conjunto de samples para el entrenamiento
	std::vector<Sample> testingSet;
	std::vector<arma::mat> thetas; //L-1 matrices
	std::vector<arma::Col<double> > a;
	std::vector<arma::mat> upperDelta;
	std::vector<arma::mat> D;
	std::vector<double> y;

	void forwardPropagate(Sample s);
	void backPropagate();

	void trainByGradient(int iter, double alpha);

	void initTraining();
	void initTrainingXNOR();
	void initRandomThetas();
	double sigmoid(double z);
	void gradChecking();
	double cost();

	//Metodos de pruebas
	void pruebaXorBasica();
	void saveThetas();
	void showThetas();
	void readThetas(std::vector<std::string> lectura);
	void init();

	//Para los tests
	void fillTestingY();

	//AdvancedOptimization
	void trainByOM();
	void s1_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr);
};

#endif /* NNMACHINE_H_ */
