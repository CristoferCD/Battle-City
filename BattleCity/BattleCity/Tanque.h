#pragma once

#include <glm.h>
#include "Objeto.h"
#include "Proyectil.h"

class Tanque : 
	public Objeto
{
public:
	GLfloat vel = 0.0f;	//Velocidad del coche en funci�n de los ejes
	int rotacion;
	float aceleracion; 
	int velMaxima;	
	Proyectil *bala;
	bool enColision;		//Indica si est� en contacto con alg�n objeto

	Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima = 1, float aceleracion = 0.5f);
	~Tanque();
	
	void dibujar() override;
	void update();
	void disparar();
};

extern "C" {
	GLMmodel* readOBJ(const char* obj);
}
