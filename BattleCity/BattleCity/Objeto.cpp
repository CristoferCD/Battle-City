#include "Objeto.h"
#include "glCallback.h"

void Objeto::dibujar()
{
	if (idLista) {
		glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(posActual.x, posActual.y, posActual.z);
		glScalef(escala.x, escala.y, escala.z);
		glBindTexture(GL_TEXTURE_2D, textura);
		glCallList(idLista);
		glPopMatrix();
	}
}

GLuint Objeto::getTextura()
{
	return textura;
}


Objeto::Objeto(GLuint idLista, punto & tamBase, punto & posInicial, punto & factorEscalado, GLuint textura)
{
	this->idLista = idLista;
	this->posActual = posInicial;
	this->escala = factorEscalado;
	this->boundingBox = AABB(posActual, tamBase * factorEscalado);
	this->textura = textura;
}

Objeto::Objeto(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, const char* rutaTextura)
{
	this->idLista = idLista;
	this->posActual = posInicial;
	this->escala = factorEscalado;
	this->boundingBox = AABB(posActual, tamBase*factorEscalado);
	glCallback::cargarTextura(textura, rutaTextura);
}

Objeto::~Objeto()
{
	glDeleteLists(idLista, 1);
}
