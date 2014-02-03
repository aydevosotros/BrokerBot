
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <limits.h>
#include "Sample.h"
#include "GodMachine.h"

//
// Non-Blocking GetLine:
//    Gets a line of characters from a stream, until it finds a delimiter or the timeout passes
//
void nb_getline(std::istream& in, std::string& str,
                char delim = '\n', unsigned timeout = 3) {
    std::stringstream ss;
    char c = 0;

    // Timeout needs to be incremented +1, 'cause 0 does not count
    if (timeout + 1 != 0) timeout++;

    // Read characters while we do not found delimiter or we run out of time
    while ( timeout > 0 && c != delim ) {
        // Get next character (instream should be non-blocking!)
        in.clear();
        in.get(c);

        if (!in.eof()) {
            // Next character found, put it in provisional stream
            if (c != delim) ss << c;
        } else if (ss.str().length() > 0) {
            // Found eof, and we are in the middle of the line
            while (in.eof() && ss.str().length() > 0 && timeout > 0) {
                // Sleep 1 millisecond and try again
                usleep(1000);
                in.clear();
                timeout--;
            }

        // Nothing ready to be read, let's exit
        } else break;
    }

    str = ss.str();
}

//Return the value of stock to buy or sell
double stockToTrade(double valorAccion, double cantidad_actual){
	return (int)(cantidad_actual * 0.5)/valorAccion;
}

int main(void)
{
	GodMachine *machine = new GodMachine(NeuralNetwork);
    bool reg = false;
    std::string s = "", subs;
	double cantidad_a_vender = 0;
	double cantidad_a_comprar = 0;
	double numero_acciones = 0;
	double precio_total = 0;
	int candletimethetas = 0;  //Hardcodear el tiempo de los candles de nuestras thetas
	int candletimesim = 0;
	int initTime = 0;
	int finishTime = 0;
	int candlesPorMuestra = 0;
	int muestrasAlmacenadas = 0;
	double high = 0;
	double open;
	double close;
	double low = DBL_MAX;
	double volume = 0;


    // Wait for INITIALIZE command
    while (s != "INITIALIZE") {
      nb_getline(std::cin, s);
      usleep(200000);      // Sleep for 0.2 seconds (5 times per second)
    }

    // Register into the match
    std::stringstream ss;
    srand(time(NULL));
    ss << "ply" << rand() % 1000;
    std::cout << "REGISTER " << ss.str() << std::endl;

    // Wait for START command
    subs = "";
    do {
      nb_getline(std::cin, s);
      if (s.length() > 5) {
        subs = s.substr(0, 5);
        std::cerr << "SUBSTR: [" << subs << "]\n";
      }
      usleep(200000);      // Sleep for 0.2 seconds (5 times per second)
    } while (subs != "START");

    // Interpret START command
    ss.clear(); ss.str(s);
    unsigned mode, duration, candletime;
    double initfiat;
    ss >> s >> mode >> duration >> candletime >> initfiat;
    std::cerr << "START " << mode << " " << duration << " " << candletime << " " << initfiat << std::endl;
	double cantidad_actual = initfiat;
    // Until the end of the simulation, BUY and SELL
    while (s != "END") {
        nb_getline(std::cin, s);
        if (s != ""){
//    		std::cerr << "Recibido: (" << s << ")" << std::endl;
			subs = "";
			subs = s.substr(0, 4);

			if(subs == "NEXT"){
				//Almacenamos los datos del candle leído
				std::stringstream ss;
				ss.clear(); ss.str(s);
				double unixtime;
				double tmphigh,tmpopen,tmpclose,tmplow,tmpvolume;
				ss >> s >> unixtime >> tmphigh >> tmpopen >> tmpclose >> tmplow >> tmpvolume;

				//Guardamos la diferencia de tiempo entre candles del simulador
				if (initTime == 0){
					initTime = unixtime;
				}
				else if (finishTime == 0){;
					finishTime = unixtime;
				}
				else{
					candletimesim = finishTime-initTime;
//					candlesPorMuestra = (int)(candletimethetas/candletimesim);
					candlesPorMuestra = 6;

					//Si tenemos los datos suficientes para un candle agrupado, lo creamos y predecimos el siguiente
					if(muestrasAlmacenadas == candlesPorMuestra) {
						std::vector<double> inputs;
						Sample candle;

						inputs.push_back(high);
						inputs.push_back(open);
						inputs.push_back(close);
						inputs.push_back(low);
						inputs.push_back(volume);

						candle.setInput(inputs);

						bool prediction = machine->predict(candle);
						//si predict devuelve true quiere decir que va a subir
						if(prediction == 1){
							cantidad_a_comprar = stockToTrade(close,cantidad_actual); //calculamos cuanto queremos comprar
							std::cerr << "Compro " << cantidad_a_comprar << std::endl;
							std::cout << "BUY " << cantidad_a_comprar << std::endl; //enviamos el mensaje
						}else{
							cantidad_a_vender = stockToTrade(close,cantidad_actual); //calculamos la cantidad a vender
							std::cerr << "Vendo " << cantidad_a_vender << std::endl; //mandamos el mensaje sell
							std::cout << "SELL " << cantidad_a_vender << std::endl; //mandamos el mensaje sell
						}

						//Reinicializamos los valores del nuevo candle agrupado
						muestrasAlmacenadas = 0;
						high = 0;
						low = DBL_MAX;
						volume = 0;
					}

					//Guardamos la información para el candle agrupado
					if(muestrasAlmacenadas == 0){
						 open = tmpopen;
					}
					if (tmphigh > high)
						high = tmphigh;
					if (tmplow < low)
						low = tmplow;
					volume += tmpvolume;
					close = tmpclose;

					muestrasAlmacenadas++;
				}
			}else if(subs == "SOLD"){
				std::cerr << s << std::endl; //mostramos el mensaje de sold
				std::stringstream ss;
				ss.clear(); ss.str(s);

				ss >> s >> numero_acciones >> close; //fragmentamos el mensaje

				precio_total = numero_acciones * close; //calculamos el precio total
				cantidad_actual = cantidad_actual + precio_total; //aumentamos nuestra cantidad_actual
			}else if(subs == "BOUG"){
				std::cerr << s << std::endl; //mostramos el mensaje de bought

				std::stringstream ss;
				ss.clear(); ss.str(s);

				ss >> s >> numero_acciones >> close; //fragmentamos el mensaje

				precio_total = numero_acciones * close; //calculamos el precio total
				cantidad_actual = cantidad_actual - precio_total; //lo descontamos de nuestra cantidad
			}else if(subs == "NOT_"){// no entiende el mensaje enviado,
				std::cerr << "Pinyico" << std::endl;
			}
		}
        usleep(50000);      // Sleep for 0.05 seconds (20 times per second)
    }

    return 0;
}


