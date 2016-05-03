#pragma once

#include <glm.h>
#include "Objeto.h"
class Tanque;
static Tanque *instanciaTanque;

class Tanque : public Objeto
{
public:
	GLfloat vel = 0.0f;	//Velocidad del coche en función de los ejes
	int rotacion;
	int aceleracion;
	float velMaxima;
	
	Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, float velMaxima = 2.0f, int aceleracion = 1);
	~Tanque();
	
	void dibujar() override;
	static void updateWrapper(int);

private:
	void update();
};

extern "C" {
	GLMmodel* readOBJ(const char* obj);
}
