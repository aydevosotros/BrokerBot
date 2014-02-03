#include "LinRMachine.h"
#include "Utils.h"

LinRMachine::LinRMachine() {
	//iterTrain y alphaTrain están hardcodeados aquí si no se inicializan.
	C_iterations = 1000;
	C_alpha = 0.001;
	C_trainingType = 2; //1 normal, 2 gradiente
	C_executionMode = 1;
}

LinRMachine::~LinRMachine() {}

void LinRMachine::setParameters(char *argv[]) {
//	std::cout << "I'm setting parameters with the LinRMachine" << std::endl;

	C_executionMode = atoi(argv[2]);
//	std::cout << "Execution mode set to " << C_executionMode << std::endl;

	if(C_executionMode == 0){
		C_trainingFile = argv[3];
		C_testingFile = argv[4];

//		std::cout << "Training with " << C_trainingFile << std::endl;
//		std::cout << "Testing with " << C_testingFile << std::endl;

		C_trainingType = atoi(argv[5]);

//		std::cout << "Training type " << C_trainingType << std::endl;

		if(C_trainingType == 2){
			C_alpha = atof(argv[6]);

//			std::cout << "Alpha " << C_alpha << std::endl;
		}

		C_treshold = atof(argv[7]);
	} else 	if(C_executionMode == 1){
		C_inputFile = argv[3];

//		std::cout	<< "Predicting " << C_inputFile << std::endl;
		C_treshold = atof(argv[4]);
	}
}

void LinRMachine::loadTrainingSet(std::string filename) {
//	std::cout << "I'm loading training set with the LinRMachine from " << filename << std::endl;

	std::string line;
	std::ifstream trainingFile(filename.c_str());

	if(trainingFile.is_open()){
		while(std::getline(trainingFile,line)) {
			Sample tmp;

			tmp.setInput(Utils::vStovD(Utils::split(line,';')));

			std::getline(trainingFile,line);

			double res = atof(line.c_str());

			tmp.setRResult(res);

			C_trainingSet.push_back(tmp);
		}

		trainingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void LinRMachine::loadTestingSet(std::string filename) {
//	std::cout << "I'm loading testing set with the LinRMachine from " << filename << std::endl;

	std::string line;
	std::ifstream testingFile(filename.c_str());

	if(testingFile.is_open()){
		while(std::getline(testingFile,line)) {
			Sample tmp;

			tmp.setInput(Utils::vStovD(Utils::split(line,';')));

			std::getline(testingFile,line);

			double res = atof(line.c_str());

			tmp.setRResult(res);

			C_testingSet.push_back(tmp);
		}

		testingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//Cargo el input, y el result lo pongo a 0
void LinRMachine::loadInput(std::string filename) {
//	std::cout << "I'm loading input with the LinRMachine from " << filename << std::endl;

	std::string line;
	std::ifstream inputFile(filename.c_str());

	if(inputFile.is_open()){
		std::getline(inputFile,line);

		C_input.setInput(Utils::vStovD(Utils::split(line,';')));

		C_input.setRResult(atof(line.c_str()));

		inputFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void LinRMachine::loadThetas(std::string filename) {
//	std::cout << "I'm loading Thetas with the LinRMachine from " << filename << std::endl;

	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	std::vector<std::string> trainingFileParts(Utils::split(filename,'-'));

	std::string root = trainingFileParts[0].substr(0,10);

	std::string machinefolder = "LR/";

	std::string value = trainingFileParts[0].substr(10,trainingFileParts[0].length());

	std::string route = "";

	route.append(root);

	route.append(machinefolder);

	route.append(value);	route.append("/");

	std::string prefix = "";

	prefix.append(root);
	prefix.append(value);

	std::string thetaName = filename.substr(prefix.length()+1,filename.length());

	route.append(thetaName);

	C_thetaFileName = route;

//	std::cout << "EL archivo que vamos a leer es: " << thetasFileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ifstream thetasFile(C_thetaFileName.c_str());

	if(thetasFile.is_open()){

		while(std::getline(thetasFile,line)) {
			C_theta.push_back(atof(line.c_str()));
		}

		thetasFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void LinRMachine::run(){
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
//			std::cout << std::endl << C_trainingSet[i].getRResult() << std::endl;
//		}
//
//		std::cout << std::endl;
//
//		for(unsigned int i = 0; i < C_testingSet.size(); i++){
//			for(unsigned int j = 0; j < C_testingSet[i].getInput().size(); j++){
//				std::cout << C_testingSet[i].getInput()[j] << ' ';
//			}
//			std::cout << std::endl << C_testingSet[i].getRResult() << std::endl;
//		}
//
//		//Fin de la comprobacion

		train();
		test();
	} else if(C_executionMode == 1){
//		std::cout << "Predict" << std::endl;


//		loadInput(C_inputFile);

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
//		std::cout << std::endl << C_input.getRResult() << std::endl;
//
//		//Fin de la comprobacion

//		std::cout << predict(C_input) << std::endl;

		//Escribir lo que devuelve input en algun lado?
	}
}

void LinRMachine::train(){
//	std::cout << "I'm training with the LinRMachine" << std::endl;

	C_nFeatures = C_trainingSet[0].getNFeatures();

//	std::cout << "nFeatures es " << C_nFeatures << std::endl;

	if(C_trainingType == 2){
		trainByGradient(C_iterations, C_alpha);
	} else{
		trainByNormal();
	}

	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	std::string command = "mkdir -p ";

	std::vector<std::string> trainingFileParts(Utils::split(C_trainingFile,'-'));

	std::string root = trainingFileParts[0].substr(0,10);

	std::string machinefolder = "LinR/";

	std::string value = trainingFileParts[0].substr(10,trainingFileParts[0].length());

	std::string route = "";

	route.append(root);

	route.append(machinefolder);

	route.append(value);	route.append("/");

	command.append(route);

	system(command.c_str());

//	std::cout << "El comando ha sido: "<< std::endl << command << std::endl;

	std::string prefix = "";

	prefix.append(root);
	prefix.append(value);

	std::string thetaName = C_trainingFile.substr(prefix.length()+1,C_trainingFile.length());

	route.append(thetaName);

	C_thetaFileName = route;

//	std::cout << "EL archivo creado es: " << C_thetaFileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ofstream thetasFile(C_thetaFileName.c_str());

	if(thetasFile.is_open()){
		thetasFile << C_theta[0];

		for(unsigned int i = 1; i < C_theta.size(); i++){
			thetasFile << "\n" << C_theta[i];
		}

		thetasFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//Hay que repasar cuando los outputs sean buenos
void LinRMachine::test(){
//	std::cout << "I'm testing with the LinRMachine" << std::endl;

	fillActualY();

	std::vector<double> auxY;

	for(unsigned int i = 0; i < C_testingSet.size(); i++){
		double p = (double)predict(C_testingSet[i]);

		std::cout << "Predigo p = " << p << std::endl;

		C_predictedY.push_back(p);
	}

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

	double precission = tPositives / (tPositives + fPositives);

	std::cout << "Precission = " << precission << std::endl;

	double recall = tPositives / (tPositives + fNegatives);

	std::cout << "Recall = " << recall << std::endl;

	double fScore = 2 * ( (precission * recall) / (precission + recall) );

	std::cout << "fScore = " << fScore << std::endl;
}

double LinRMachine::predict(Sample input){
//	std::cout << "I'm predicting with the LinRMachine" << std::endl;
	if(C_executionMode == 1)
		loadThetas("../Values/LinR/MSFT/20101001-20131201-1d-14d-OpenValue-Training");

	return h(input.getInput());
}

void LinRMachine::clearTrainingSet() {
	C_trainingSet.clear();
}

double LinRMachine::cost(std::vector<double> theta) {
	double J = 0.0;

	for(unsigned int i = 0; i < C_y.size(); i++){

		// Calculo el valor de la hipótises para la theta dada
		double hipotesis = h(C_X[i]);

		// Calculo el valor del coste para la theta dada
		J += pow((hipotesis-C_y[i]),2.0);
	}

	return J/(2*C_y.size());
}

double LinRMachine::h(std::vector<double> x){
	double p = 0.0;

	for(int i = 0; i < C_nFeatures+1; i++){
		if(i == 0) {
			p = C_theta[i];
		} else {
			p += C_theta[i]*x[i-1];
		}
	}

	return p;
}

void LinRMachine::trainByGradient(int iter, double alpha) {
	fillX();
	fillY();
	fillTheta();

	double variacion = 0.01;
	double costePrevio = 0.0;

	//Para tantas iteraciones como queramos
	for(int i = 0; i < iter; i++){
		//Calculamos el coste, para ver si mejora o no
		double coste = cost(C_theta);
//		std::cout << "Para la iteracion " << i << " el coste es " << coste << std::endl;

		//Calculamos el gradiente
		for(int j = 0; j < C_nFeatures+1; j++){
			double sum = 0.0;

			for(unsigned int k = 0; k < C_y.size(); k++){
				sum += (h(C_X[k])-C_y[k])*C_X[k][j];
			}

			C_theta[j] = C_theta[j] - (C_alpha*(sum/C_y.size()));
		}

		//Analizamos la variación
		variacion = std::abs(coste-costePrevio);
//		std::cout << "La variación en el coste para la iteracion "<< i <<" es de: " << variacion << std::endl;

		if(variacion < 0.0001 && !std::isnan(variacion)){ // Truquillo porque a veces es nan
//			std::cout << "Estoy suficientemente entrenado!!!!!!\n";
			break;
		}else if(std::isnan(variacion)){
//			std::cout << "Tengo un NaN!!!!\n";
		}

		costePrevio = coste;
	}
}

void LinRMachine::trainByNormal() {
	// Obtengo la X
	arma::mat X = arma::mat(C_trainingSet.size(), C_nFeatures+1);

	for(unsigned int i = 0; i < C_trainingSet.size(); i++){
		for(int j = 0; j < C_nFeatures+1; j++){
			if(j==0){
				X(i,j) = 1.0;
			} else{
				X(i,j) = C_trainingSet[i].getInput()[j-1];
			}
		}
	}

//	std::cout << "X es " << std::endl << X << std::endl;

	// Obtengo la Y
	arma::mat y = arma::mat(C_trainingSet.size(), 1);

	for(unsigned int i=0; i<C_trainingSet.size(); i++){
		y(i) = (double)C_trainingSet[i].getRResult();
	}

//	std::cout << "y es " << std::endl << y << std::endl;

	// Inicializo theta
	arma::mat theta = arma::mat(C_nFeatures+1, 1);

	// Calculo vectorialmente
	theta = arma::pinv(X.t()*X)*X.t()*y;

//	std::cout << "theta es " << std::endl << theta << std::endl;

	//Me guardo las nuevas thetas
	C_theta.clear();

	for(int i = 0; i < C_nFeatures+1; i++){
		C_theta.push_back(theta(i));
	}
}

//Funciones rellenadoras
void LinRMachine::fillX() {
	for(unsigned int i = 0; i < C_trainingSet.size(); i++){
		std::vector<double> aux;

		for(int j = 0; j < C_nFeatures+1; j++){
			if(j == 0){
				aux.push_back(1.0);
			} else {
				aux.push_back(C_trainingSet[i].getInput()[j-1]);
			}
		}

		C_X.push_back(aux);
	}
}

void LinRMachine::fillTheta() {
	for(int i = 0; i < C_nFeatures+1; i++) {
		C_theta.push_back(0.0);
	}
}

void LinRMachine::fillY() {
	for(unsigned int i=0; i<C_trainingSet.size(); i++){
		C_y.push_back(C_trainingSet[i].getRResult());
	}
}

void LinRMachine::fillActualY(){
	for(unsigned int i=0; i<C_testingSet.size(); i++){
		C_actualY.push_back(C_testingSet[i].getRResult());
	}
}
