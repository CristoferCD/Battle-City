#pragma once
#include "Objeto.h"

class Proyectil : public Objeto
{
public:
	int aceleracionX;
	int aceleracionY;
	bool enAire;	//Indica si esta instancia ha sido disparada y aun no lleg� a su destino

	Proyectil(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, GLuint textura);
	~Proyectil();

	void update();
};

