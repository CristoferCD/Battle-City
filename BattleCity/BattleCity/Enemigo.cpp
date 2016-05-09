#include "Enemigo.h"
#include <time.h>



Enemigo::Enemigo(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima, float aceleracion) :
	Tanque(Modelo, rutaTextura, Posicion, velMaxima, aceleracion)
{
}


Enemigo::~Enemigo()
{
}

void Enemigo::cambiarDireccion()
{
	int rotaciones[4] = { 0,90,180,270 };
	static int i = 0;
	this->rotacion = rotaciones[i];
	i = (i + time(NULL)) % 4;
	//Por culpa de glut no puedo incluir cstdlib para usar un rand.
}

void Enemigo::IA()
{
	vel += aceleracion;
	vel = vel > velMaxima ? velMaxima : vel;	//Velocidad máxima
	if (time(NULL) % 10 == 0)					//Cambia de dirección de vez en cuando
		cambiarDireccion();
	update();
	if(!bala->enAire)
		this->disparar();
}

void Enemigo::update()
{
	//Por alguna razón, poner la velocidad como float da problemas de renderizado, así que uso la
	//aceleración para cambiar la posición
	switch (rotacion)
	{
	case 0:
		posActual.x += aceleracion;
		break;
	case 90:
		posActual.y += aceleracion;
		break;
	case 180:
		posActual.x -= aceleracion;
		break;
	case 270:
		posActual.y -= aceleracion;
		break;
	default:
		break;
	}
	this->boundingBox.c = posActual;
	
	if (bala->enAire)
		bala->update();
	else
		bala->posActual.z = -30;		//Se oculta el objeto debajo del escenario

	glutPostRedisplay();
}

void Enemigo::retroceder()
{
	switch (rotacion)
	{
	case 0:
		posActual.x -= 1;
		break;
	case 90:
		posActual.y -= 1;
		break;
	case 180:
		posActual.x += 1;
		break;
	case 270:
		posActual.y += 1;
		break;
	default:
		break;
	}
	this->boundingBox.c = posActual;
}



