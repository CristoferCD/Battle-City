#include "Tanque.h"
#include "figuras.h"
#include <math.h>


Tanque::Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima, float aceleracion, int cadenciaDisparo)
	: Objeto(glmList(readOBJ((char*)Modelo), GLM_SMOOTH | GLM_TEXTURE), punto(1.2, 1.2, 1.2), Posicion, punto(2, 2, 2), rutaTextura)
{
	this->velMaxima = velMaxima;
	this->aceleracion = aceleracion;
	rotacion = 270;
	//Hacer un vector de balas de este tamaño
	this->cadenciaDisparo = cadenciaDisparo;

	int listBala = glGenLists(1);
	glNewList(listBala, GL_COMPILE);
	glColor3f(1, 0, 0);
	cubo();
	glEndList();

	bala = new Proyectil(listBala, punto(1, 1, 1), Posicion, punto(1, 1, 1), 0);
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
	bala->dibujar();
}

void Tanque::disparar()
{
	if (cadenciaDisparo > disparosRealizados) {		
		bala->enAire = true;
		bala->posActual.z = 10;
		bala->update();				//Se fuerza la primera actualización
		switch (rotacion)
		{
		case 0:
			bala->aceleracionX = 2;			//La bala se mueve hacia la derecha
			bala->aceleracionY = 0;
			bala->posActual = posActual;	//Se dispara desde el tanque
			bala->posActual.x += 2;			//Con un desfase en función de a donde se apunta
			break;
		case 90:
			bala->aceleracionY = 2;
			bala->aceleracionX = 0;
			bala->posActual = posActual;
			bala->posActual.y += 2;
			break;
		case 180:
			bala->aceleracionX = -2;
			bala->aceleracionY = 0;
			bala->posActual = posActual;
			bala->posActual.x -= 2;
			break;
		case 270:
			bala->aceleracionY = -2;
			bala->aceleracionX = 0;
			bala->posActual = posActual;
			bala->posActual.y -= 2;
			break;
		default:
			break;
		}
		disparosRealizados++;
	}
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

	//Para que en las colisiones no siga a la deriva
	vel = vel < 0 ? 0 : vel;

	if (bala->enAire)
		bala->update();
	else
		bala->posActual.z = -30;		//Se oculta el objeto debajo del escenario

	glutPostRedisplay();
}

GLMmodel * readOBJ(const char * obj)
{
	return glmReadOBJ((char*)obj);
}

