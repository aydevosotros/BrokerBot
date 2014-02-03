#include "NNMachine.h"

NNMachine::NNMachine() {	executionMode = 1;
}

NNMachine::~NNMachine() {}

void NNMachine::setParameters(char* argv[]) {
	this->executionMode = atoi(argv[2]);


	if(executionMode == 0){
		this->trainingFile = argv[3];
		this->testFile = argv[4];

		this->lambda = atof(argv[5]);

		this->alpha = atof(argv[6]);

		this->iteraciones = atoi(argv[7]);
	} else if(executionMode == 1){
		this->inputFile = argv[3];
		this->trainingFile = argv[3];
	}
}

void NNMachine::loadTrainingSet(std::string filename) {
	//	std::cout << "I'm loading training set with the LinRMachine from " << filename << std::endl;

	std::string line;
	std::ifstream trainingFile(this->trainingFile.c_str());

	if(trainingFile.is_open()){
		while(std::getline(trainingFile,line)) {
			Sample tmp;

			tmp.setInput(Utils::vStovD(Utils::split(line,';')));

			std::getline(trainingFile,line);

			int res = atoi(line.c_str());

			std::vector<int> result;

			result.push_back(res);

			tmp.setResult(result);

			this->trainingSet.push_back(tmp);
		}

		trainingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void NNMachine::loadTestingSet(std::string filename) {
	//	std::cout << "I'm loading testing set with the LinRMachine from " << filename << std::endl;

	std::string line;
	std::ifstream testingFile(this->testFile.c_str());

	if(testingFile.is_open()){
		while(std::getline(testingFile,line)) {
			Sample tmp;
			tmp.setInput(Utils::vStovD(Utils::split(line,';')));
			std::getline(testingFile,line);
			double res = atof(line.c_str());
			std::vector<int> result;
			result.push_back(res);
			tmp.setResult(result);
			this->testingSet.push_back(tmp);
		}
		testingFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

//Innecesario en el final
void NNMachine::loadInput(std::string filename) {
	//	std::cout << "I'm loading input with the NNMachine from " << filename << std::endl;

	std::string line;
	std::ifstream inputFile(this->inputFile.c_str());

	if(inputFile.is_open()){
		std::getline(inputFile,line);
		this->input.setInput(Utils::vStovD(Utils::split(line,';')));
		inputFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void NNMachine::loadThetas(){
	//	std::cout << "I'm loading Thetas with the NNMachine" << std::endl;

	//Ahora averiguaremos el nombre que debe tener el archivo de thetas

	/*std::vector<std::string> trainingFileParts(Utils::split(inputFile,'-'));
	std::string root = trainingFileParts[0].substr(0,10);
	std::string machinefolder = "NN/";
	std::string value = trainingFileParts[0].substr(10,trainingFileParts[0].length());
	std::string route = "";
	route.append(root);
	route.append(machinefolder);
	route.append(value);	route.append("/");
	std::string prefix = "";
	prefix.append(root);
	prefix.append(value);
	std::string thetaName = inputFile.substr(prefix.length()+1,inputFile.length());
	route.append(thetaName);*/
//	thetasFileName = route;
	thetasFileName = "../Values/LR/rap2_training/15d-OpenValue-CloseValue-HighValue-LowValue-VolumeValue-Scaled-Training";
//	std::cout << "EL archivo que vamos a leer es: " << thetasFileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ifstream thetasFile(thetasFileName.c_str());

	if(thetasFile.is_open()){
		this->thetas.clear();

		std::getline(thetasFile,line);
		for(int l=0; l<L-1; l++){
			arma::mat thetaL(s_l[l+1], s_l[l]+1);
			for(int i=0; i<s_l[l+1]; i++){
				std::vector<double> aux = Utils::vStovD(Utils::split(line,';'));
				for(int j=0; j<s_l[l]+1; j++){
					thetaL(i,j) = aux[j];
				}
				std::getline(thetasFile,line);
			}
			this->thetas.push_back(thetaL);
		}
		thetasFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
}

void NNMachine::saveThetas(){
	//Ahora averiguaremos el nombre que debe tener el archivo de thetas
	std::string command = "mkdir -p ";
	std::vector<std::string> trainingFileParts(Utils::split(trainingFile,'-'));
	std::string root = trainingFileParts[0].substr(0,10);
	std::string machinefolder = "NN/";
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
	std::string thetaName = trainingFile.substr(prefix.length()+1,trainingFile.length());
	route.append(thetaName);
	thetasFileName = route;
	std::cout << "EL archivo creado es: " << thetasFileName << std::endl;

	//Y para terminar, escribimos las thetas en un archivo

	std::string line;
	std::ofstream thetasFile(thetasFileName.c_str());
	if(thetasFile.is_open()){
		for(int l=0; l<L-1; l++){
			for(int i=0; i<s_l[l+1]; i++){
				thetasFile << thetas[l](i,0);
				for(int j=1; j<s_l[l]+1; j++){
					thetasFile << ";" << thetas[l](i,j);
				}
				thetasFile << std::endl;
			}
		}
		thetasFile.close();
	} else{
		std::cout << "Unable to open file" << std::endl;
	}
	std::cout << "LLego hasta aquí después de guardar las thetas" << std::endl;
}

void NNMachine::showThetas(){
	for(int l=0; l<L-1; l++){
		for(int i=0; i<s_l[l+1]; i++){
			std::cout << thetas[l](i,0);
			for(int j=1; j<s_l[l]+1; j++){
				std::cout << ";" << thetas[l](i,j);
			}
			std::cout << std::endl;
		}
	}
}

void NNMachine::clearTrainingSet(){
	this->trainingSet.clear();
}

void NNMachine::run() {
	if(this->executionMode == 0){
		std::cout << "Cargando el training set..." << std::endl;
		loadTrainingSet(trainingFile);
		std::cout << "Cargando el testing set..." << std::endl;
		loadTestingSet(testFile);
		std::cout << "Entrenando..." << std::endl;
		train();
		std::cout << "Después de entrenar hago el test" << std::endl;
		test();
	} else if(this->executionMode == 1){
		std::cout << "Cargando input" << std::endl;
		loadInput(inputFile);
		init();
		std::cout << "Cargando thetas" << std::endl;
		loadThetas();
		std::cout << "Guardando thetas" << std::endl;
//		showThetas();
	}
}

void NNMachine::test() {
	double treshold = 0.5;

	this->fillTestingY();
	std::cout << "I'm testing with the NNMachine" << std::endl;
	std::vector<int> auxY;

	for(unsigned int i = 0; i < this->testingSet.size(); i++){
		double p = (double)predict(this->testingSet[i]);

		std::cout << "Obtengo una predicción de: " << p << std::endl;

		if((p>treshold && this->actualY[i] > 0) || (p<=treshold && this->actualY[i] < 0)){
			if(p>treshold){
				std::cout << "Predigo que el siguiente periodo será de subida" << std::endl;
			} else {
				std::cout << "Predigo que el siguiente periodo será de bajada" << std::endl;
			}
//
			std::cout << "Ni Sandro Rey" << std::endl;

		} else if((p>treshold && this->actualY[i] < 0) || (p<=treshold && this->actualY[i] > 0)){
			if(p>treshold){
				std::cout << "Predigo que el siguiente periodo será de subida" << std::endl;
			} else {
				std::cout << "Predigo que el siguiente periodo será de bajada" << std::endl;
			}

			std::cout << "Pinyico..." << std::endl;
		} else {
			std::cout << "No se que carajo ha pasado" << std::endl;
		}
		if(p>treshold)
			this->predictedY.push_back(1);
		else this->predictedY.push_back(-1);
	}

	//Al final, cuando tengamos lleno el C_predictedY, calculamos Precission y Recall

	double tPositives = 0.0;
	double fPositives = 0.0;
	double tNegatives = 0.0;
	double fNegatives = 0.0;

	for(unsigned int i = 0; i < this->actualY.size(); i++){
		if(this->actualY[i] > 0 && this->predictedY[i] > 0){
			tPositives++;
		} else if(this->actualY[i] > 0 && this->predictedY[i] < 0){
			fNegatives++;
		} else if(this->actualY[i] < 0 && this->predictedY[i] > 0){
			fPositives++;
		} else if(this->actualY[i] < 0 && this->predictedY[i] < 0){
			tNegatives++;
		}
	}

	std::cout << "Acierto: " << (tPositives + tNegatives) / this->actualY.size() << std::endl;

	double precission = tPositives / (tPositives + fPositives);

	std::cout << "Precission = " << precission << std::endl;

	double recall = tPositives / (tPositives + fNegatives);

	std::cout << "Recall = " << recall << std::endl;

	double fScore = 2 * ( (precission * recall) / (precission + recall) );

	std::cout << "fScore = " << fScore << std::endl;
}

double NNMachine::predict(Sample input) {
	std::cout << "Cargando la thetas" << std::endl;
	if(executionMode == 1)
		loadThetas();
	std::cout << "Propagando..." << std::endl;
	forwardPropagate(input);
	return this->a[L-1](0);
}

void NNMachine::train(){
	init();
	this->initRandomThetas();
	this->initTraining();
	std::cout << "Iniciando el descenso por gradiente..." << std::endl;
	trainByGradient(this->iteraciones, this->alpha);
	saveThetas();
}

void NNMachine::init(){
	if(executionMode == 0){
		nFeatures = trainingSet[0].getNFeatures();
	} else if (executionMode == 1){
		nFeatures = input.getNFeatures();
	}
//	loadTrainingSet("trainingFireDoorEscaper.txt");
	s_l.clear();
	s_l.push_back(this->nFeatures);
	s_l.push_back(this->nFeatures*2);
//	s_l.push_back(this->trainingSet[0].getNFeatures()*3);
	s_l.push_back(this->nFeatures*2);
//	s_l.push_back(this->trainingSet[0].getNFeatures());
	s_l.push_back(1);
	L = s_l.size();
}

void NNMachine::forwardPropagate(Sample s) {
	// Implementación vectorizada
	//Seteo el input en la primera capa
	for(int i=0; i<s_l[0]+1; i++){
		if(i==0)
			this->a[0](i) = 1.0; // Seteo el bias
		else this->a[0](i) = s.getInput()[i-1];
	}
	// Forwarding
	for(int l=1; l<L; l++){
//		std::cout << "Propago por la capa: " << l << std::endl;
		// Calculo z para la capa
		arma::mat zL = this->thetas[l-1]*this->a[l-1];
//		std::cout << "Prepara lo capa " << l << " con la z:" << std::endl << zL << std::endl;
		// Y calculo la activación para la capa
		if(l<L-1){
			for(int i=0; i<s_l[l]+1; i++){
				if(i==0)
					this->a[l](i)=1.0;
				else this->a[l](i)=sigmoid(zL(i-1));
			}
		} else {
			for(int i=0; i<s_l[l]; i++)
				this->a[l](i)=sigmoid(zL(i));
		}
//		std::cout << "El valor de la activación es: " << std::endl << a[l] << std::endl;
	}
}

void NNMachine::backPropagate() {
	// Inicializo upperDelta a cero
	this->upperDelta.clear();
	for(int l=0; l<L-1; l++)
		this->upperDelta.push_back(arma::zeros(s_l[l+1], s_l[l]+1));
	for(int s=0; s<trainingSet.size(); s++){
		forwardPropagate(this->trainingSet[s]);
		std::vector<arma::Col<double> > lowerDelta;
		// Inicializo los lowerDelta porque como empiezo al revés al hacer pushback y los índices y los petes...
		for(int l=0; l<L; l++){
			if(l==L-1)
				lowerDelta.push_back(arma::Col<double>(s_l[l]));
			else lowerDelta.push_back(arma::Col<double>(s_l[l]+1));
		}
		for(int l=L-1; l>0; l--){
			if(l==L-1){
				int r = (trainingSet[s].getResult()[0]==-1)?0:1;
//				std::cout << "La activación para la última capa es: " << a[l] << "y el resultado es: " << this->trainingSet[s].getResult()[0] << std::endl;
				lowerDelta[l](0) = a[l](0)-r; // esto es lo que tengo que generalizar para muchas salidas
			} else {
				arma::Col<double> aux;
				arma::mat gP = this->a[l]%(1-a[l]);
//				std::cout << "Para la capa " << l << " tenemos una g': " << std::endl << gP;
				if(l+1 == L-1)
					aux = thetas[l].t()*lowerDelta[l+1];
				else
					aux = thetas[l].t()*lowerDelta[l+1].rows(1,s_l[l+1]);
//				std::cout << "Para la capa " << l << " tenemos un aux': " << std::endl << aux;
				lowerDelta[l] = aux%gP;
			}
//			std::cout << "Para la capa " << l << " he obtenido un lowerDelta de: " << std::endl << lowerDelta[l] << std::endl;
		}
		for(int l=0; l<L-1; l++){
//			std::cout << "Calculo upperDelta para la capa: " << l << " " << std::endl << lowerDelta[l+1] << " " << s_l[l+1] << std::endl;
			if(l+1 == L-1)
				this->upperDelta[l] += lowerDelta[l+1]*this->a[l].t();
			else
				this->upperDelta[l] += lowerDelta[l+1].rows(1,s_l[l+1])*this->a[l].t();
//			std::cout << "El valor de upperDelta es: " << std::endl << upperDelta[l] << "para un theta: " << std::endl << thetas[l] << std::endl;
		}
	}
	// Regularizo y obtengo la D
	for(int l=0; l<L-1; l++){
		for(int i=0; i<s_l[l+1]; i++){
			for(int j=0; j<s_l[l]+1; j++){
				if(j==0)
					this->D[l](i,j) = (this->upperDelta[l](i,j)/this->trainingSet.size());
				else
					this->D[l](i,j) = (this->upperDelta[l](i,j)/this->trainingSet.size())+(lambda*this->thetas[l](i,j));
//				std::cout << "El valor de D para (" << l << "," << i << "," << j << ") es : " << D[l](i,j) << std::endl;
			}
		}
	}
}

void NNMachine::initTraining() {
	// Init thetas and a
	this->D.clear();
	this->a.clear();
	for(int l=0; l<L; l++){
//		std::cout << "Inicializo a para esta capa " << l << std::endl;
		if(l==L-1){
			this->a.push_back(arma::Col<double>(s_l[l]));
		} else {
			this->a.push_back(arma::Col<double>(s_l[l]+1));
//			std::cout << "Inicializo D para esta capa " << l << std::endl;
			this->D.push_back(arma::mat(s_l[l+1], s_l[l]+1));
		}
	}
}

void NNMachine::initRandomThetas() {
	this->thetas.clear();
	for(int l=0; l<L-1; l++){
		arma::mat thetaL(s_l[l+1], s_l[l]+1);
		for(int i=0; i<s_l[l+1]; i++)
			for(int j=0; j<s_l[l]+1; j++)
				thetaL(i,j) = Utils::uniformRandomDouble(0.0,10.0);
		this->thetas.push_back(thetaL);
	}
}

void NNMachine::initTrainingXNOR() {
	// Init thetas and a
	this->thetas.clear();
	arma::mat thetaL1(2,3);
	thetaL1(0,0)=-30.0;
	thetaL1(0,1)=20.0;
	thetaL1(0,2)=20.0;
	thetaL1(1,0)=10.0;
	thetaL1(1,1)=-20.0;
	thetaL1(1,2)=-10.0;
	this->thetas.push_back(thetaL1);
	arma::mat thetaL2(1,3);
	thetaL2(0,0) = -10.0;
	thetaL2(0,1) = 20.0;
	thetaL2(0,2) = 20.0;
	this->thetas.push_back(thetaL2);
	// Inicializo los a
	for(int l=0; l<L; l++){
		if(l==L-1)
			this->a.push_back(arma::Col<double>(s_l[l]));
		else{
			this->a.push_back(arma::Col<double>(s_l[l]+1));
			this->D.push_back(arma::mat(s_l[l+1], s_l[l]+1));
		}
		this->upperDelta.push_back(arma::mat(s_l[l+1], s_l[l]+1));
	}
}

double NNMachine::sigmoid(double z) {
	double e = 2.71828182845904523536;
	return 1/(1+pow(e,-z));
}

void NNMachine::gradChecking() {
	std::vector<arma::mat> cThetasPlus;
	std::vector<arma::mat> cThetasMinus;

	double epsilon = 0.001; // Esto tiene que entrar por parámetro
	// Inicializo los thetas random
//	initRandomThetas();
	// Calculo el coste sumando y restando epsilon a nuestro theta
	for(int l=0; l<L-1; l++){
		arma::mat cTPL(s_l[l+1], s_l[l]+1);
		arma::mat cTML(s_l[l+1], s_l[l]+1);
		for(int i=0; i<s_l[l+1]; i++){
			for(int j=0; j<s_l[l]+1; j++){
				this->thetas[l](i,j) += epsilon;
//				std::cout << "Voy por: " << l << "," << i << "," << j << std::endl;
				cTPL(i,j) = cost();
//				std::cout << "Calculo el coste para plus: " << cTPL(i,j) << std::endl;
				this->thetas[l](i,j) -= 2*epsilon;
				cTML(i,j) = cost();
//				std::cout << "Calculo el coste para menos: " << cTML(i,j) << std::endl;
				this->thetas[l](i,j) += epsilon;
			}
		}
		cThetasPlus.push_back(cTPL);
		cThetasMinus.push_back(cTML);
	}
	// Calculo la aproximación
	std::vector<arma::mat> gradAprox;
	for(int l=0; l<L-1; l++){
		gradAprox.push_back(arma::mat(s_l[l+1], s_l[l]+1));
		for(int i=0; i<s_l[l+1]; i++){
			for(int j=0; j<s_l[l]+1; j++){
				gradAprox[l](i,j) = (cThetasPlus[l](i,j)-cThetasMinus[l](i,j))/(2*epsilon);
				std::cout << "Para la capa " << l << " desde el nodo " << i << " al nodo " << j <<
						" tengo un aproximado de: " << gradAprox[l](i,j) << " y una DX " << this->D[l](i,j) << std::endl;
			}
		}
	}
}

double NNMachine::cost(){
	double J = 0.0;
	for(int i=0; i<this->trainingSet.size(); i++){
		int r = (trainingSet[i].getResult()[0]==-1)?0:1;
		this->forwardPropagate(trainingSet[i]);
		J += r*std::log(a[L-1](0)) + (1-r)*std::log(1-a[L-1](0));
	}
	J /= trainingSet.size()*(-1.0);
//	std::cout << "El coste sin regularizar: " << J << std::endl;
	double regularization = 0.0;
	for(int l=0; l<L-1; l++)
		for(int i=0; i<s_l[l]; i++)
			for(int j=0; j<s_l[l+1]; j++)
				regularization+=std::pow(this->thetas[l](j,i),2);
	regularization = (this->lambda/2.0*this->trainingSet.size())*regularization;
	return J+regularization;
}

void NNMachine::trainByGradient(int iter, double alpha) {
	double pCoste = 0.0;
	for(int it=0; it<iter; it++){
		// Calculo el coste
		backPropagate();
//		std::cout << "Un poco basto pero..." << std::endl;
//		for(int l=0; l<L-1; l++)
//			std::cout << "D para la capa " << l << " vale: " << std::endl << this->D[l];
//		gradChecking();
		double coste = cost();
		std::cout << "Para la iteración " << it << " el coste es: " << coste << std::endl;
		// Recalculo theta para la siguiente iteracion
		for(int l=0; l<L-1; l++){
//			for(int i=0; i<s_l[l+1]; i++)
//				for(int j=0; j<s_l[l]+1; j++)
//					this->thetas[l](i,j)-=alpha*this->D[l](i,j);
			this->thetas[l] = this->thetas[l] - (alpha*this->D[l]);
		}
		double vari = std::abs(pCoste-coste);
		std::cout << "La variación en el coste para la iteración "<< it <<" es de: " << vari << std::endl;
		if(it>0){
			if(vari <= 0.00001){
				std::cout << "Estoy suficientemente entrenado!!!!!!\n";
				break;
			}
//			if(std::isnan(coste))
//				break;
		}
//		std::cout << it << "," << coste << std::endl;
		pCoste = coste;
	}
	std::cout << "Dejo el gradiente con un coste de: " << this->cost() << std::endl;
}

void NNMachine::pruebaXorBasica() {
	// Unas cosas previas pa tener que probar
	s_l.clear();
	s_l.push_back(2);
	s_l.push_back(2);
	s_l.push_back(1);
	L = s_l.size();
	Sample s;
	std::vector<double> input;
	input.push_back(0);
	input.push_back(1);
	s.setInput(input);
	std::vector<int> result;
	result.push_back(1);
	s.setResult(result);
	this->trainingSet.push_back(s);
	std::cout << "LLego hasta aquí" << std::endl;
	// Con esto hago una thetas y fp and bp
	this->initTrainingXNOR();
	this->backPropagate();
	this->gradChecking();
}

void NNMachine::fillTestingY() {
	for(int i=0; i<this->testingSet.size(); i++)
		this->actualY.push_back(this->testingSet[i].getResult()[0]);
}

void NNMachine::trainByOM() {

}
