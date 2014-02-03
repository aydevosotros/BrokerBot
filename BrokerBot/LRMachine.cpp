#include "LRMachine.h"
#include "Utils.h"

//Hay cosas hardcodeadas y tal
LRMachine::LRMachine() {
	iterTrain = 1000;
	alphaTrain = 0.01;
	trainType = 1; //1 normal, 2 gradiente
	C_executionMode = 1;
}

LRMachine::~LRMachine() {}

void LRMachine::setParameters(char *argv[]) {
//	std::cout << "I'm setting parameters with the LRMachine" << std::endl;

	C_executionMode = atoi(argv[2]);
//	std::cout << "Execution mode set to " << C_executionMode << std::endl;

	if(C_executionMode == 0){
		C_trainingFile = argv[3];
		C_testingFile = argv[4];

//		std::cout	<< "Training with " << C_trainingFile
//					<< " and testing with " << C_testingFile << std::endl;

		C_lambda = atoi(argv[5]);
//		std::cout << "Lambda set to " << C_lambda << std::endl;

		C_treshold = atof(argv[6]);
	} else 	if(C_executionMode == 1){
		C_inputFile = argv[3];

//		std::cout	<< "Predicting " << C_inputFile << std::endl;

		C_lambda = atof(argv[4]);
//		std::cout << "Lambda set to " << C_lambda << std::endl;
	}

//	Algo más a partir de aquí
}

void LRMachine::loadTrainingSet(std::string filename) {
//	std::cout << "I'm loading training set with the LRMachine from " << filename << std::endl;

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

void LRMachine::loadTestingSet(std::string filename) {
//	std::cout << "I'm loading testing set with the LRMachine from " << filename << std::endl;

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

//Cargo el input, y el result lo pongo a 0
void LRMachine::loadInput(std::string filename) {
//	std::cout << "I'm loading input with the LRMachine from " << filename << std::endl;

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

void LRMachine::loadThetas(std::string filename) {
//	std::cout << "I'm loading Thetas with the LRMachine from " << filename << std::endl;

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

void LRMachine::run(){
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
//			std::cout << std::endl << C_trainingSet[i].getResult() << std::endl;
//		}
//
//		std::cout << std::endl;
//
//		for(unsigned int i = 0; i < C_testingSet.size(); i++){
//			for(unsigned int j = 0; j < C_testingSet[i].getInput().size(); j++){
//				std::cout << C_testingSet[i].getInput()[j] << ' ';
//			}
//			std::cout << std::endl << C_testingSet[i].getResult() << std::endl;
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
//		std::cout << std::endl << C_input.getResult() << std::endl;
//
//		//Fin de la comprobacion

//		predict(C_input);

		//Escribir lo que devuelve input en algun lado?
	}
}

void LRMachine::train(){
//	std::cout << "I'm training with the LRMachine" << std::endl;

	C_nFeatures=C_trainingSet[0].getNFeatures();

	if(trainType == 2){
		trainByGradient(iterTrain, alphaTrain);
	} else{
		trainByNormalEcuation();
	}

	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	std::string command = "mkdir -p ";

	std::vector<std::string> trainingFileParts(Utils::split(C_trainingFile,'-'));

	std::string root = trainingFileParts[0].substr(0,10);

	std::string machinefolder = "LR/";

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

void LRMachine::test(){
//	std::cout << "I'm testing with the LRMachine" << std::endl;

	fillActualY();

	std::vector<double> auxY;

	for(unsigned int i = 0; i < C_testingSet.size(); i++){
		double p = (double)predict(C_testingSet[i]);

		auxY.push_back(p);
	}

	C_predictedY.push_back(auxY);

	//Al final, cuando tengamos lleno el C_predictedY, calculamos Precission y Recall

	double tPositives = 0.0;
	double fPositives = 0.0;
	double tNegatives = 0.0;
	double fNegatives = 0.0;

	for(unsigned int i = 0; i < C_actualY.size(); i++){
		if(C_actualY[0][i] > C_treshold && C_predictedY[0][i] > C_treshold){
			tPositives++;
		} else if(C_actualY[0][i] > C_treshold && C_predictedY[0][i] < C_treshold){
			fNegatives++;
		} else if(C_actualY[0][i] < C_treshold && C_predictedY[0][i] > C_treshold){
			fPositives++;
		} else if(C_actualY[0][i] < C_treshold && C_predictedY[0][i] < C_treshold){
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

double LRMachine::predict(Sample input){
//	std::cout << "I'm predicting this input with the LRMachine" << std::endl;
	if(C_executionMode == 1)
		loadThetas("../Values/LR/MSFT/20101001-20131201-1d-14d-OpenValue-Training");

	// Como tengo un sigmoide, con un threshold voy to cheto
	double p = 0.0;

	for(int i=0; i<C_nFeatures+1; i++){
		if(i==0) {
			p = C_theta[i];
		} else {
			p += C_theta[i]*input.getInput()[i-1];
		}
	}

	return sigmoid(p);
}

void LRMachine::clearTrainingSet() {
	C_trainingSet.clear();
}

double LRMachine::sigmoid(double z) {
	double e = 2.71828182845904523536;
	return 1/(1+pow(e,-z));
}

//Presuntamente correcto, y las Ys?
double LRMachine::cost(std::vector<double> theta, std::vector<std::vector<double> > X, std::vector<double> y) {
	double J = 0.0;
	for(unsigned int i=0; i<y.size(); i++){
		// Calculo el valor de la hipótises para la theta dada
		double h = 0.0;
		for(unsigned int j=0; j<theta.size(); j++){
			h += theta[j]*X[i][j];
		}
		h = sigmoid(h);
		// Calculo el valor del coste para la theta dada
		J+=(-y[i]*std::log(h))-((1-y[i])*std::log(1-h));
	}
	return J/y.size();
}

void LRMachine::grad(std::vector<double> tetha, std::vector<std::vector<double> > X, std::vector<double> y, std::vector<double> grad) {
	for(unsigned int j=0; j<C_theta.size(); j++){
		double parcial = 0.0;
		for(unsigned int i=0; i<y.size(); i++){
			// Calculo el valor de la hipótises para la theta dada
			double h = 0.0;
			for(unsigned int k=0; k<C_theta.size(); k++){
				h += C_theta[k]*X[i][k];
			}
			h = sigmoid(h);
			parcial += (h-y[i])*X[i][j];
		}
		grad[j] = parcial/C_theta.size();
	}
}

void LRMachine::trainByGradient(int iter, double alpha) {
	double vari = 0.01;
	double pCoste = 0.0;
	std::vector<double> gradiente;
	fillX();
	fillTheta();
	fillY();

	for(int k=0; k<iter; k++){
		// Calculo el coste
		double coste = cost(C_theta, C_X, C_y[0]);
		std::cout << "Para la iteración " << k << " el coste es: " << coste << std::endl;
		// Recalculo theta para la siguiente iteracion
		grad(C_theta, C_X, C_y[0], gradiente);
//		std::cout << "El nuevo theta para la it " << k << " es: ";
		for(unsigned int i=0; i<C_theta.size(); i++){
			C_theta[i]=C_theta[i]-alpha*gradiente[i];
//			std::cout << theta[i]-alpha*gradiente[i] << " (" << alpha*gradiente[i] << ") ";
		}
//		std::cout << std::endl;
		vari = std::abs(coste-pCoste);
		std::cout << "La variación en el coste para la iteración "<< k <<" es de: " << vari << std::endl;
		if(vari < 0.0001 && !std::isnan(vari)){ // Truquillo porque a veces es nan
			std::cout << "Estoy suficientemente entrenado!!!!!!\n";
			break;
		} else if (std::isnan(vari)){
			std::cout << "Tengo un NaN!!!!\n";
		}
		pCoste = coste;
	}

}

void LRMachine::trainByNormalEcuation() {
	// Actualizo aqui el número de características
	int nFeaturesCuad = 2*C_nFeatures;

	// Obtengo la X
	arma::mat X = arma::mat(C_trainingSet.size(), nFeaturesCuad+1);

	for(unsigned int i = 0; i < C_trainingSet.size(); i++){
		for(int j = 0; j < C_nFeatures+1; j++){
			if(j==0){
				X(i,j) = 1.0;
			} else{
				X(i,j) = C_trainingSet[i].getInput()[j-1];
			}
		}
	}

	// Elementos cuadráticos
	for(unsigned int i = 0; i < C_trainingSet.size(); i++){
		for(int j = 0; j < C_nFeatures; j++){
			X(i,C_nFeatures+j+1) = pow(C_trainingSet[i].getInput()[j],2.0);
		}
	}

	// Obtengo la Y
	arma::mat y = arma::mat(C_trainingSet.size(), C_trainingSet[0].getResult().size());

	for(unsigned int i=0; i<C_trainingSet.size(); i++){
		for(unsigned int j=0; j<C_trainingSet[0].getResult().size(); j++){
			y(i,j) = C_trainingSet[i].getResult()[j];
		}
	}

	// Calculo la matriz de alpha
	arma::mat Alpha(nFeaturesCuad+1, nFeaturesCuad+1);
	Alpha.eye();
	Alpha(0,0)=0;
	alphaTrain*Alpha;

//	std::cout << Alpha;

	// Inicializo theta
	arma::mat theta = arma::mat(nFeaturesCuad+1, 1);

	// Calculo vectorialmente
	theta = arma::pinv(X.t()*X+Alpha)*X.t()*y;

//	std::cout << theta;
	C_theta.clear();

	for(int i=0; i<nFeaturesCuad+1; i++){
		C_theta.push_back(theta(i));
	}
}

void LRMachine::fillX() {
	for(unsigned int i=0; i<C_trainingSet.size(); i++){
		for(int j=0; j<C_nFeatures+1; j++){
			if(j==0)
				C_X[i][j] = 1.0;
			else C_X[i][j] = C_trainingSet[i].getInput()[j-1];
		}
	}
}

void LRMachine::fillTheta() {
	for(int i=0; i<C_nFeatures+1; i++) {
		C_theta.push_back(0.5);
	}
}

void LRMachine::fillY() {
	std::vector<double> aux;

	for(unsigned int i=0; i<C_trainingSet.size(); i++){
		aux.clear();

		for(unsigned int j=0; j<C_trainingSet[0].getResult().size(); j++){
			aux.push_back((double)C_trainingSet[i].getResult()[j]);
		}

		C_y.push_back(aux);
	}
}

void LRMachine::fillActualY(){
	std::vector<double> aux;

	for(unsigned int i=0; i<C_testingSet.size(); i++){
		aux.clear();

		for(unsigned int j=0; j<C_testingSet[0].getResult().size(); j++){
			aux.push_back((double)C_testingSet[i].getResult()[j]);
		}

		C_actualY.push_back(aux);
	}
}
