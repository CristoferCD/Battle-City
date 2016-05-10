#include "Proyectil.h"



Proyectil::Proyectil(GLuint idLista, punto& tamBase, punto& posInicial, punto& factorEscalado, GLuint textura, bool Iluminada) :
	Objeto(idLista, tamBase, posInicial, factorEscalado, textura)
{
	aceleracionX = 0;
	aceleracionY = 0;
	enAire = false;
	iluminada = Iluminada;
}

Proyectil::~Proyectil()
{
}


void Proyectil::update()
{
	posActual.x += aceleracionX;
	posActual.y += aceleracionY;

	this->boundingBox.c = posActual;
	iluminada ? dibujarIluminada() : dibujar();
	boundingBox.dibujar();

	glutPostRedisplay();
}

void Proyectil::dibujarIluminada()
{
	if (enAire && idLista) {
		glEnable(GL_LIGHT1);
		//GLfloat luzEm[] = { 1.0f, 1.0f, 1.0f, 1.0 };
		//glMaterialfv(GL_FRONT, GL_EMISSION, luzEm);
		/*GLfloat LuzPos[] = { posActual.x, posActual.y, posActual.z, 0.0f };
		GLfloat SpotDir[] = { posActual.x, posActual.y, posActual.z };
		glLightfv(GL_LIGHT1, GL_POSITION, LuzPos);*/
		//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, SpotDir);
		dibujar();
		//glDisable(GL_LIGHT1);
	}
}

void Proyectil::dibujar()
{
	if (enAire && idLista) {
		glPushMatrix();
		glTranslatef(posActual.x, posActual.y, posActual.z);
		glScalef(escala.x, escala.y, escala.z);
		glBindTexture(GL_TEXTURE_2D, textura);
		glCallList(idLista);
		glPopMatrix();
	}
}
