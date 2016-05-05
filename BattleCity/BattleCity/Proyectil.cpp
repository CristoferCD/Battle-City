#include "Proyectil.h"



Proyectil::Proyectil(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, GLuint textura) :
	Objeto(idLista, tamBase, posInicial, factorEscalado, textura)
{
	aceleracionX = 0;
	aceleracionY = 0;
	enAire = false;
}

Proyectil::~Proyectil()
{
}


void Proyectil::update()
{
	posActual.x += aceleracionX;
	posActual.y += aceleracionY;

	this->boundingBox.c = posActual;

	dibujar();

	glutPostRedisplay();
}
