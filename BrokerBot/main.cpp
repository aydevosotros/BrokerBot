/*
 * main.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: antonio
 */


#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include "Sample.h"
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

//
// Send a Random BUY/SELL command, or nothing
//
void sendRandomCommand()
{
    unsigned r = rand() % 1000;
    double c = (double)(rand() % 1000) / 999.0;
    switch(r) {
        case 0: std::cout << "BUY " << c << std::endl; break;
        case 1: std::cout << "SELL " << c << std::endl; break;
        default: break;
    }
}

//quiza predict deberia devolver 1--> compra / 0 --> no hace nada / -1 --> venta
bool predict(Sample candle){

	return false;
}
double cantidad_a_vender(double cantidad_actual){
	double vender = (cantidad_actual * 50)/100;

	return vender;
}
double cantidad_a_comprar(double cantidad_actual){
	double comprar = (cantidad_actual * 50)/100;

	return comprar;
}
Sample crear_sample(std::string s){
	Sample candle;
	std::stringstream ss;
	 ss.clear(); ss.str(s);
	unsigned unix_time;
	double high,open,close,low,volume;
	ss >> s >> unix_time >> high >> open >> close >> low >> volume;
	candle.input[0] = high;
	candle.input[1] = open;
	candle.input[2] = close;
	candle.input[3] = low;
	candle.input[4] = volume;

	return candle;
}
int main(void)
{
    bool reg = false;
    std::string s = "", subs;
	double cantidad_a_vender = 0;
	double cantidad_a_comprar = 0;
	double numero_acciones = 0;
	double precio_accion = 0;
	double precio_total = 0;

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
    std::cerr << "START " << mode << duration << candletime << initfiat;
	double cantidad_actual = initfiat;
    // Until the end of the simulation, BUY and SELL
    while (s != "END") {
        nb_getline(std::cin, s);
        if (s != ""){
			subs = "";
			subs = s.substr(0, 3);
			if(subs == "NEXT"){
				Sample candle = crear_sample(s);
				std::cerr << "Recibido: (" << s << ")" << std::endl;
				//si predict devuelve true quiere decir que va a subir
				if(predict(candle)){
					cantidad_a_comprar = cantidad_a_comprar(cantidad_actual); //calculamos cuanto queremos comprar
					std::cout << "BUY " << cantidad_a_comprar << std::endl; //enviamos el mensaje		
				}else{
					cantidad_a_vender = cantidad_a_vender(cantidad_actual); //calculamos la cantidad a vender
					std::cout << "SELL " << cantidad_a_vender << std::endl; //mandamos el mensaje sell				
				}
			}else if(subs == "SOLD"){
				std::cerr << s << std::endl; //mostramos el mensaje de sold
				std::stringstream ss;
				ss.clear(); ss.str(s);

				ss >> s >> numero_acciones >> precio_accion; //fragmentamos el mensaje

				precio_total = numero_acciones * precio_accion; //calculamos el precio total
				cantidad_actual = cantidad_actual + precio_total; //aumentamos nuestra cantidad_actual
			}else if(subs == "BOUG"){
				std::cerr << s << std::endl; //mostramos el mensaje de bought

				std::stringstream ss;
				ss.clear(); ss.str(s);

				ss >> s >> numero_acciones >> precio_accion; //fragmentamos el mensaje

				precio_total = numero_acciones * precio_accion; //calculamos el precio total
				cantidad_actual = cantidad_actual - precio_total; //lo descontamos de nuestra cantidad
			}else if(subs == "NOT_"){// no entiende el mensaje enviado,
			
			}
		}
        //sendRandomCommand();
        usleep(50000);      // Sleep for 0.05 seconds (20 times per second)
    }

    return 0;
}

					
