#include "SVMachine.h"

SVMachine::SVMachine() {
	C_kernel = new LinearKernel();
	C_nFeatures = 0;
	C_m = 0;
	C_executionMode = 1;

}

SVMachine::SVMachine(KernelType t) {
	C_nFeatures = 0;
	C_m = 0;

	switch(t){
	case Linear:
		C_kernel = new LinearKernel();
		break;
	case Polynomial:
		C_kernel = new PolynomialKernel(2);
		break;
	case RBF:
		C_kernel = new RBFKernel();
		break;
	default:
		C_kernel = new LinearKernel();
	}
}

SVMachine::~SVMachine() {}

void SVMachine::setParameters(char *argv[]){
//	std::cout << "I'm setting parameters with the SVMachine" << std::endl;

	//esta variable luego se sustituirá por la variable de clase

	C_executionMode = atoi(argv[2]);
//	std::cout << "Execution mode set to " << C_executionMode << std::endl;

	if(C_executionMode == 0){
		C_trainingFile = argv[3];
		C_testingFile = argv[4];

//		std::cout	<< "Training with " << C_trainingFile
//					<< " and testing with " << C_testingFile << std::endl;
		int margin = atoi(argv[5]);

		if(margin == 0){
	//		std::cout << "The margin is soft" << std::endl;
		} else if(margin == 1){
	//		std::cout << "The margin is hard" << std::endl;
		}

		int ker = atoi(argv[6]);
		switch(ker){
		case 0: C_kernel = new LinearKernel();
		break;
		case 1: C_kernel = new PolynomialKernel(6);
		break;
		case 2: C_kernel = new RBFKernel();
		break;
		}

		C_treshold = atoi(argv[7]);
	} else 	if(C_executionMode == 1){
		C_inputFile = argv[3];

//		std::cout	<< "Predicting " << C_inputFile << std::endl;
		int margin = atoi(argv[4]);

		if(margin == 0){
	//		std::cout << "The margin is soft" << std::endl;
		} else if(margin == 1){
	//		std::cout << "The margin is hard" << std::endl;
		}

		int ker = atoi(argv[5]);
		switch(ker){
		case 0: C_kernel = new LinearKernel();
		break;
		case 1: C_kernel = new PolynomialKernel(2); // Esto ha de entrar por parámetro
		break;
		case 2: C_kernel = new RBFKernel();
		break;
		}

		C_treshold = atoi(argv[6]);
	}
}

//copypasteao tal cual de la LR
void SVMachine::loadTrainingSet(std::string filename){
//	std::cout << "I'm loading training set with the SVMachine" << std::endl;

	std::string line;
	std::ifstream trainingFile(filename.c_str());

	if(trainingFile.is_open()){
		while(std::getline(trainingFile,line)) {
			Sample tmp;

			tmp.setInput(Utils::vStovD(Utils::split(line,';')));

			std::getline(trainingFile,line);

			std::vector<int> res;

			res.push_back(atoi(line.c_str()));

			tmp.setResult(res);

			C_trainingSet.push_back(tmp);
		}

		trainingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//copypasteao tal cual de la LR
void SVMachine::loadTestingSet(std::string filename){
//	std::cout << "I'm loading testing set with the SVMachine" << std::endl;

	std::string line;
	std::ifstream testingFile(filename.c_str());

	if(testingFile.is_open()){
		while(std::getline(testingFile,line)) {
			Sample tmp;

			tmp.setInput(Utils::vStovD(Utils::split(line,';')));

			std::getline(testingFile,line);

			std::vector<int> res;

			res.push_back(atoi(line.c_str()));

			tmp.setResult(res);

			C_testingSet.push_back(tmp);
		}

		testingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//copypasteao tal cual de la LR
void SVMachine::loadInput(std::string filename){
//	std::cout << "I'm loading input with the SVMachine" << std::endl;

	std::string line;
	std::ifstream inputFile(filename.c_str());

	if(inputFile.is_open()){
		std::getline(inputFile,line);

		C_input.setInput(Utils::vStovD(Utils::split(line,';')));

		std::vector<int> res;

		res.push_back(0);

		C_input.setResult(res);

		inputFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//copypaste de LR
void SVMachine::run(){
//	std::cout << "I'm running the mode ";

	if(C_executionMode == 0){
//		std::cout << "Test" << std::endl;

		loadTrainingSet(C_trainingFile);
		loadTestingSet(C_testingFile);

//		//Comprobamos que se han cargado bien los dos archivos
//
//		for(unsigned int i = 0; i < C_trainingSet.size(); i++){
//			for(unsigned int j = 0; j < C_trainingSet[i].getInput().size(); j++){
//				std::cout << C_trainingSet[i].getInput()[j] << ' ';
//			}
//			std::cout << std::endl << C_trainingSet[i].getResult()[0] << std::endl;
//		}
//
//		std::cout << std::endl;
//
//		for(unsigned int i = 0; i < C_testingSet.size(); i++){
//			for(unsigned int j = 0; j < C_testingSet[i].getInput().size(); j++){
//				std::cout << C_testingSet[i].getInput()[j] << ' ';
//			}
//			std::cout << std::endl << C_testingSet[i].getResult()[0] << std::endl;
//		}
//
//		//Fin de la comprobacion

		train();
		test();
	} else if(C_executionMode == 1){
//		std::cout << "Predict" << std::endl;

		loadInput(C_inputFile);
		loadParams();

//		//Comprobamos que se leen bien los archivos
//
//		for(unsigned int i = 0; i < C_theta.size(); i++){
//			std::cout << C_theta[i] << ' ';
//		}
//		std::cout << std::endl << std::endl;
//
//		for(unsigned int i = 0; i < C_input.getInput().size(); i++){
//			std::cout << C_input.getInput()[i] << ' ';
//		}
//		std::cout << std::endl << C_input.getResult() << std::endl;
//
//		//Fin de la comprobacion

		predict(C_input);
		showParams();

		//Escribir lo que devuelve input en algun lado?
	}
}

//lleva dentro el istrainingReady
void SVMachine::train(){
//	std::cout << "I'm training with the SVMachine" << std::endl;

	C_nFeatures=C_trainingSet[0].getNFeatures();

	trainByQuadraticProgramming();

	saveParams();
}

void SVMachine::loadParams(){
	//	std::cout << "I'm loading Thetas with the NNMachine" << std::endl;

	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	std::vector<std::string> trainingFileParts(Utils::split(C_inputFile,'-'));

	std::string root = trainingFileParts[0].substr(0,10);

	std::string machinefolder = "SVM/";

	std::string value = trainingFileParts[0].substr(10,trainingFileParts[0].length());

	std::string route = "";

	route.append(root);

	route.append(machinefolder);

	route.append(value);	route.append("/");

	std::string prefix = "";

	prefix.append(root);
	prefix.append(value);

	std::string name = C_inputFile.substr(prefix.length()+1,C_inputFile.length());

	route.append(name);

	C_fileName = "params.txt";

//	std::cout << "EL archivo que vamos a leer es: " << C_fileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ifstream paramsFile(C_fileName.c_str());

	if(paramsFile.is_open()){
		std::getline(paramsFile,line);

		C_b = ET(atof(line.c_str()));

		std::getline(paramsFile,line);

		C_n = atoi(line.c_str());

		init();

		std::getline(paramsFile,line);

		for(int i=0; i<C_n; i++){
			if(C_SupportVectors.at(i) > 0.0){
				C_SupportVectors(i) = atof(line.c_str());

				std::getline(paramsFile,line);

				C_y(i) = atof(line.c_str());

				std::getline(paramsFile,line);

				std::vector<double> aux = Utils::vStovD(Utils::split(line,';'));

				for(int j = 0; j < aux.size(); j++){
					C_X(i,j) = aux[j];
				}

				std::getline(paramsFile,line);
			}
		}

		paramsFile.close();

		std::cout << "Ya he cargado" << std::endl;
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void SVMachine::showParams(){
	std::cout << C_b << std::endl;

	std::cout << C_n << std::endl;

	for(int i=0; i<C_n; i++){
		if(C_SupportVectors.at(i) > 0.0){
			std::cout << C_SupportVectors.at(i) << std::endl;
			std::cout << C_y.at(i) << std::endl;

			std::cout << C_X(i,0);

			for(int j = 1; j < C_X.row(i).n_elem; j++){
				std::cout << ";" << C_X(i,j);
			}

			std::cout << std::endl;
		}
	}
}

void SVMachine::saveParams(){
	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	std::string command = "mkdir -p ";

	std::vector<std::string> trainingFileParts(Utils::split(C_trainingFile,'-'));

	std::string root = trainingFileParts[0].substr(0,10);

	std::string machinefolder = "SVM/";

	std::string value = trainingFileParts[0].substr(10,trainingFileParts[0].length());

	std::string route = "";

	route.append(root);

	route.append(machinefolder);

	route.append(value);	route.append("/");

	command.append(route);

	system(command.c_str());

	std::cout << "El comando ha sido: "<< std::endl << command << std::endl;

	std::string prefix = "";

	prefix.append(root);
	prefix.append(value);

	std::string name = C_trainingFile.substr(prefix.length()+1,C_trainingFile.length());

	route.append(name);

	C_fileName = route;

	std::cout << "EL archivo creado es: " << C_fileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ofstream paramsFile(C_fileName.c_str());

	if(paramsFile.is_open()){
		paramsFile << C_b << std::endl;

		int n = 0;
		for(int i=0; i<C_trainingSet.size(); i++){
			if(C_SupportVectors.at(i) > 0.0){
				n++;
			}
		}
		paramsFile << n << std::endl;

		for(int i=0; i<C_trainingSet.size(); i++){
			if(C_SupportVectors.at(i) > 0.0){
				paramsFile << C_SupportVectors.at(i) << std::endl;
				paramsFile << C_y.at(i) << std::endl;

				paramsFile << C_X(i,0);

				for(int j = 1; j < C_X.row(i).n_elem; j++){
					paramsFile << ";" << C_X(i,j);
				}

				paramsFile << std::endl;
			}
		}

		paramsFile.close();

		std::cout << "Ya he escrito" << std::endl;
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void SVMachine::test(){
//	std::cout << "I'm testing with the SVMachine" << std::endl;
	fillActualY();

	for(unsigned int i = 0; i < C_testingSet.size(); i++){
		double p = predict(C_testingSet[i]);

		std::cout << "Predigo p=" << p << std::endl;

		C_predictedY.push_back(p);
	}

//	std::cout << "PredictedY es:" << std::endl;
//
//	for(unsigned int i = 0; i < C_predictedY.size(); i++){
//		std::cout << C_predictedY[i] << std::endl;
//	}
//
//	std::cout << "ActualY es:" << std::endl;
//
//	for(unsigned int i = 0; i < C_actualY.size(); i++){
//		std::cout << C_actualY[i] << std::endl;
//	}

	//Al final, cuando tengamos lleno el C_predictedY, calculamos Precission y Recall

	double tPositives = 0.0;
	double fPositives = 0.0;
	double tNegatives = 0.0;
	double fNegatives = 0.0;

	for(unsigned int i = 0; i < C_actualY.size(); i++){
		if(C_actualY[i] > C_treshold && C_predictedY[i] > C_treshold){
			tPositives++;
		} else if(C_actualY[i] > C_treshold && C_predictedY[i] < C_treshold){
			fNegatives++;
		} else if(C_actualY[i] < C_treshold && C_predictedY[i] > C_treshold){
			fPositives++;
		} else if(C_actualY[i] < C_treshold && C_predictedY[i] < C_treshold){
			tNegatives++;
		}
	}

	std::cout << "tPositives = " << tPositives << std::endl;
	std::cout << "fNegatives = " << fNegatives << std::endl;
	std::cout << "fPositives = " << fPositives << std::endl;
	std::cout << "tNegatives = " << tNegatives << std::endl;

	std::cout << "Acierto: " << (tPositives + tNegatives) / this->C_actualY.size() << std::endl;

	double precission = tPositives / (tPositives + fPositives);

	std::cout << "Precission = " << precission << std::endl;

	double recall = tPositives / (tPositives + fNegatives);

	std::cout << "Recall = " << recall << std::endl;

	double fScore = 2 * ( (precission * recall) / (precission + recall) );

	std::cout << "fScore = " << fScore << std::endl;
}

double SVMachine::predict(Sample input){
//	std::cout << "I'm predicting with the SVMachine" << std::endl;
	if(C_executionMode == 1)
		loadParams();

    ET aux(0.0);
    // Hago esto provisional para escalar
    std::vector<double> entradaScalada;
    std::vector<Sample> sInput;

    sInput.push_back(input);
//    Utils::scalation(sInput);

    arma::Col<double> Input(input.getNFeatures());

    for(int i=0; i<input.getNFeatures(); i++)
        Input(i)=sInput[0].getInput()[i];

    for(int i=0; i<C_trainingSet.size(); i++){
        if(C_SupportVectors.at(i) > 0.0){
            aux += ET(C_SupportVectors.at(i))*ET(C_y.at(i))*C_kernel->K(C_X.row(i).t(), Input);
//            std::cout << "La suma auxiliar vale: " << aux+b << std::endl;
        }
    }

    double p = CGAL::to_double(aux + C_b);

    std::cout << "Predigo p=" << p << std::endl;

    double treshold = 0.0;

    if(p > treshold){
    	return 1.0;
    } else if(p <= treshold){
    	return -1.0;
    }
}

void SVMachine::clearTrainingSet(){
	C_trainingSet.clear();
}

//bool SVMachine::isTrainingReady() {
//	if(trainingSet.size() > 0){
//		if(trainingSet.size() == 1)
//			nFeatures=trainingSet[0].getNFeatures();
//		if(trainingSet.size() > nFeatures*7 ){
//			trainByQuadraticProgramming();
//			return true;
//		}
//	} else return false;
//}

//void SVMachine::classifySample(Sample sample) {
//	ET aux(0.0);
//	// Hago esto provisional para escalar
//	std::vector<double> entradaScalada;
//	std::vector<Sample> sInput;
//	sInput.push_back(sample);
//	Utils::scalation(sInput);
//	arma::Col<double> Input(sample.getNFeatures());
//	for(int i=0; i<sample.getNFeatures(); i++)
//		Input(i)=sInput[0].input[i];
//	for(int i=0; i<trainingSet.size(); i++){
//		if(SupportVectors.at(i) > 0.0){
//			aux += ET(SupportVectors.at(i))*ET(y.at(i))*kernel->K(X.row(i).t(), Input);
////			std::cout << "La suma auxiliar vale: " << aux+b << std::endl;
//		}
//	}
//	double p = CGAL::to_double(aux + b);
//	std::cout << "Para este sample tenemos una p de: " << p << std::endl;
//	if((p>0 && sample.burn) || (p<=-1 && !sample.burn)){
//		if(p>0)
//			std::cout << "Predigo que la siguiente puerta está encendida" << std::endl;
//		else if(p<0) std::cout << "Predigo que la siguiente puerta está apagada" << std::endl;
//		std::cout << "Ha clasificao de puta madre" << std::endl;
//		this->classifySuccesses++;
//	} else if ((p<=0 && sample.burn) || (p>0 && !sample.burn)){
//		if(p>0)
//			std::cout << "Predigo que la siguiente puerta está encendida" << std::endl;
//		else if(p<0) std::cout << "Predigo que la siguiente puerta está apagada" << std::endl;
//		std::cout << "Pinyico... volviendo a entrenar" << std::endl;
////		this->trainingSet.push_back(sample);
////		this->trainByQuadraticProgramming();
//		this->classifySuccesses--;
//	} else std::cout << "No se que carajo ha pasado" << std::endl;
//	std::cout << "He clasificado correctamente " << classifySuccesses << std::endl;
//}

//bool SVMachine::isDoorOnFire(double input[]) {
//	ET aux(0.0);
//	std::vector<double> entradaScalada;
//	for(int i=0; i<nFeatures; i++)
//		entradaScalada.push_back(input[i]);
//	std::vector<Sample> sInput;
//	Sample s;
//	s.input = entradaScalada;
//	sInput.push_back(s);
//	Utils::scalation(sInput);
//	arma::Col<double> Input(nFeatures);
//	for(int i=0; i<nFeatures; i++)
//		Input(i)=sInput[0].input[i];
//	for(int i=0; i<trainingSet.size(); i++){
//		if(SupportVectors.at(i) > 0.0){
//			aux += ET(SupportVectors.at(i))*ET(y.at(i))*kernel->K(X.row(i).t(), Input);
//			std::cout << "La suma auxiliar vale: " << aux+b << std::endl;
//		}
//	}
//	double p = CGAL::to_double(aux + b);
//	std::cout << "La probabilidad de que la siguiente puerta esté caliente es: " << p << std::endl;
//	if(p>1)
//		return true;
//	else if(p<-1) return false;
//
//	return true;
//}

void SVMachine::init(){
	std::cout << "El numero magico es " << C_n << std::endl;

	if(C_executionMode == 0){
		C_nFeatures = C_trainingSet[0].getNFeatures();
	} else if (C_executionMode == 1){
		C_nFeatures = C_input.getNFeatures();
	}

	C_X = arma::mat(C_n, C_nFeatures);
	C_y = arma::mat(C_n, 1);
	C_SupportVectors = arma::mat(C_n,1);
}

//Está un poco fuzzy, habrá que estructurarlo y tal, pero correcto
void SVMachine::quadraticSolution() {
	int n = C_trainingSet.size();
	int m = 1; // Entiendo que es el numero de restricciones
	Program qp (CGAL::EQUAL);

	// Obtengo la X
//	Utils::scalation(C_trainingSet); // Escalado de parámetros

	std::cout << "n vale: " << n << std::endl;
	std::cout << "nFeatures vale: " << C_nFeatures << std::endl;
	C_X = arma::mat(n, C_nFeatures);

	for(int i = 0; i < n; i++){
		for(int j = 0; j < C_nFeatures; j++){
			C_X(i,j) = C_trainingSet[i].getInput()[j];
		}
	}
	std::cout << "X vale: " << std::endl << C_X;

	// Obtengo la Y
	C_y = arma::mat(n, 1);

	std::cout << "n vale: " << n << std::endl;
	for(int i = 0; i < n; i++){
		C_y(i) = C_trainingSet[i].getResult()[0];
	}

	std::cout << "Y vale: " << std::endl << C_y;
	// Seteo la restriccion
	for(int i = 0; i < n; i++){
		qp.set_a(i,0,ET(C_y.at(i)));
		qp.set_l(i,true,ET(0));
		qp.set_u(i,false);
		qp.set_c(i,ET(-1));
//		std::cout << "y(" << i << "): " << y.at(i) << std::endl;
	}

	qp.set_b(0,ET(0));
	qp.set_r(0,CGAL::EQUAL);
	qp.set_c0(ET(0));

	// Seteo la symmetric positive-semidefinite matrix
	for(int i = 0; i < n; i++){
		for(int j = 0; j<=i; j++){
			ET ip = C_kernel->K(C_X.row(i).t(),C_X.row(j).t());
//			std::cout << "El kernel para " << i << "," << j << " nos dice que el innerproduct es: " << ip << std::endl;
			ET daux = ip*ET(C_y.at(i))*ET(C_y.at(j));
			std::cout << "El producto de " << i << "," << j << ": " << daux << std::endl;
			qp.set_d(i,j,daux);
//			std::cout << "La matriz auxiliar vale:" <<  daux << std::endl;
		}
	}

	 // solve the program, using ET as the exact type
	Solution s = CGAL::solve_quadratic_program(qp, ET());
	// print basic constraint indices (we know that there is only one: 1)
	std::cout << "Y la solución es: " << s << std::endl;
//	arma::mat W = arma::mat(n,1);

	C_SupportVectors = arma::mat(n,1);

	ET sumaB(0.0);

	if(s.is_optimal()) { // we know that, don't we?
		int i = 0;
		for (Solution::Variable_value_iterator it = s.variable_values_begin(); it != s.variable_values_end(); ++it){
			C_SupportVectors(i) = CGAL::to_double(*it);
			if(C_SupportVectors.at(i) != 0.0){
				C_m = i; // Esto lo hago para obtener un sv que me resuelva la b
			}
			i++;
		}
		// Calculo la b
		for(int i=0; i<n; i++)
			if(C_SupportVectors.at(i) != 0.0)
				sumaB += ET(C_SupportVectors.at(i))*ET(C_y.at(i))*C_kernel->K(C_X.row(i).t(), C_X.row(C_m).t());
		C_b = ET(C_y(C_m)) - sumaB;
		std::cout << "Y el valor de b es: "<< C_b << std::endl;
	} else std::cout << "No es optima, vete tu a saber por qué...\n";

//	int pausa; std::cin >> pausa;
}

void SVMachine::trainByQuadraticProgramming() {
	quadraticSolution(); // Esto me da los Support Vectors
}

void SVMachine::fillActualY(){
	for(unsigned int i=0; i<C_testingSet.size(); i++){
		C_actualY.push_back((double)C_testingSet[i].getResult()[0]);
	}
}
