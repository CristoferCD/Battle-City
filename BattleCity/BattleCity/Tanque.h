#pragma once

#include <glm.h>
#include "Objeto.h"
#include "Proyectil.h"

class Tanque;
static Tanque *instanciaTanque;

class Tanque : public Objeto
{
public:
	GLfloat vel = 0.0f;	//Velocidad del coche en función de los ejes
	int rotacion;
	float aceleracion;
	int velMaxima;	
	Proyectil *bala;

	Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima = 1, float aceleracion = 0.5f, int cadenciaDisparo = 1);
	~Tanque();
	
	void dibujar() override;
	static void updateWrapper(int);
	void disparar();

private:
	int cadenciaDisparo;		//Número de proyectiles simultáneos puede disparar
	int disparosRealizados;		//Número de proyectiles actualmente en tránsito

	void update();
};

extern "C" {
	GLMmodel* readOBJ(const char* obj);
}
