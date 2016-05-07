#pragma once
#include "Tanque.h"

class Enemigo :
	public Tanque
{
public:
	Enemigo(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima = 1, float aceleracion = 0.5f, int cadenciaDisparo = 1);
	~Enemigo();

	void colision();	//IA: Acción a realizar cuando se produce una colisión.
	void IA();
};

