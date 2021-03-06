#pragma once
#include "auxiliares.h"
#include <GL\glut.h>

class Objeto
{
public:
	punto posActual;
	AABB boundingBox;

	Objeto(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, GLuint textura);
	Objeto(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, const char* rutaTextura);
	~Objeto();

	virtual void dibujar();
	GLuint getTextura();
protected:
	GLuint idLista;
	punto escala;
	GLuint textura;
};

