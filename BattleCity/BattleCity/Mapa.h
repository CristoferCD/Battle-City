#pragma once
#include <vector>
#include <mutex>
#include "Objeto.h"

#define VACIO '0' 
#define LADRILLO '1'
#define MURO '2'
#define ARBUSTO '3'
#define AGUA '4'

class Mapa
{
public:
	Mapa(const char* rutaMapa, int tileSize = 2, int numTiles = 26);
	~Mapa();

	void dibujar();
	punto getPosicion(punto coordRelativas);		//Devuelve las coordenadas absolutas en base a la posición contada en forma de tiles del mapa

	std::vector <Objeto*> objetosDestruibles;		//Desaparecen con el contacto de una bala y no se pueden atravesar
	std::vector <Objeto*> objetosEstaticos;			//Paran las balas y no atravesables.
	std::vector <Objeto*> objetosNoDestruibles;		//Las balas lo atraviesan, pero los tanques no.
	std::vector <Objeto*> objetosAtravesables;		//Tanto las balas como los tanques los atraviesan
	std::mutex mtx;

private:
	int tileSize;
};

