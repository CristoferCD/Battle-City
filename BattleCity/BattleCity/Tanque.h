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
	float aceleracion;
	int velMaxima;
	
	Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, int velMaxima = 1, float aceleracion = 0.5f);
	~Tanque();
	
	void dibujar() override;
	static void updateWrapper(int);

private:
	void update();
};

extern "C" {
	GLMmodel* readOBJ(const char* obj);
}
