#pragma once
#include "Tanque.h"

class Enemigo :
	public Tanque
{
public:
	Enemigo(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima = 1, float aceleracion = 0.5f);
	~Enemigo();
	
	bool mtx;		//No funciona el 100% de las veces, pero no puedo usar un mutex por culpa de glut

	void cambiarDireccion();	//IA: Acci�n a realizar cuando se produce una colisi�n.
	void IA();
	void update() override;
	void retroceder();
};

