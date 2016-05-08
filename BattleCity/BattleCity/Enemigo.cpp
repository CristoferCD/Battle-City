#include "Enemigo.h"
#include <time.h>



Enemigo::Enemigo(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima, float aceleracion) :
	Tanque(Modelo, rutaTextura, Posicion, velMaxima, aceleracion)
{
}


Enemigo::~Enemigo()
{
}

void Enemigo::colision()
{
	int rotaciones[4] = { 0,90,180,270 };
	static int i = 0;
	this->rotacion = rotaciones[i];
	this->vel = this->vel != 0 ? 0 : -2;
	i = (i + time(NULL)) % 4;
	//Por culpa de glut no puedo incluir cstdlib para usar un rand.
}

void Enemigo::IA()
{
	if(!enColision) {
		vel += aceleracion;
		vel = vel > velMaxima ? velMaxima : vel;	//Velocidad máxima
	}
	else {
		colision();
	}
	if(!bala->enAire)
		this->disparar();
}



