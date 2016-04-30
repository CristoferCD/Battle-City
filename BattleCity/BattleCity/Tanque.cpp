#include "Tanque.h"
#include <math.h>


Tanque::Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, float velMaxima, float aceleracion)
	: Objeto(glmList(readOBJ((char*)Modelo), GLM_SMOOTH | GLM_TEXTURE), punto(2, 2, 2), Posicion, punto(1, 1, 1), rutaTextura)
{
	this->velMaxima = velMaxima;
	this->aceleracion = aceleracion;
	rotacion = 0;
	instanciaTanque = this;
}


Tanque::~Tanque()
{
}

void Tanque::dibujar()
{
	glPushMatrix();
	glRotatef(rotacion, 0, 0, 1);
	glScalef(escala.x, escala.y, escala.z);
	//glBindTexture(GL_TEXTURE_2D, textura);
	//Temporalmente color sólido, hasta crear una textura
	glColor3f(1.0f, 1.0f, 0.0f);
	glCallList(idLista);
	glPopMatrix();
}

void Tanque::updateWrapper(int)
{
	instanciaTanque->update();
}

void Tanque::update()
{
	posActual = punto(cos(rotacion + M_PI / 180)*vel, sin(rotacion + M_PI / 180)*vel, 10);
	this->boundingBox.c = posActual;

	glutPostRedisplay();
	glutTimerFunc(15, updateWrapper, 0);
}

GLMmodel * readOBJ(const char * obj)
{
	return glmReadOBJ((char*)obj);
}

