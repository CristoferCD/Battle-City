#include "Tanque.h"
#include <math.h>


Tanque::Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, float velMaxima, int aceleracion)
	: Objeto(glmList(readOBJ((char*)Modelo), GLM_SMOOTH | GLM_TEXTURE), punto(2, 2, 2), Posicion, punto(2, 2, 2), rutaTextura)
{
	this->velMaxima = velMaxima;
	this->aceleracion = aceleracion;
	rotacion = 90;
	instanciaTanque = this;
}


Tanque::~Tanque()
{
}

void Tanque::dibujar()
{
	glPushMatrix();
	glTranslatef(posActual.x, posActual.y, posActual.z);
	//El -90 ajusta la rotación del modelo
	glRotatef(rotacion-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
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
	switch (rotacion)
	{
	case 0:
		posActual.x += vel;
		break;
	case 90:
		posActual.y += vel;
		break;
	case 180:
		posActual.x -= vel;
		break;
	case 270:
		posActual.y -= vel;
		break;
	default:
		break;
	}
	this->boundingBox.c = posActual;

	glutPostRedisplay();
	glutTimerFunc(15, updateWrapper, 0);
}

GLMmodel * readOBJ(const char * obj)
{
	return glmReadOBJ((char*)obj);
}

