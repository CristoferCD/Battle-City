#pragma once
#include <vector>
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

	std::vector <Objeto*> objetosDestruibles;		//Desaparecen con el contacto de una bala y no se pueden atravesar
	std::vector <Objeto*> objetosEstaticos;			//Paran las balas y no atravesables.
	std::vector <Objeto*> objetosNoDestruibles;		//Inmunes a las balas (atraviesan) y atravesables.
};

