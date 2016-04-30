#pragma once

#include <glm.h>
#include "Objeto.h"
class Tanque;
static Tanque *instanciaTanque;

class Tanque : public Objeto
{
public:
	GLfloat vel = 0.0f;	//Velocidad del coche en funci�n de los ejes
	int rotacion;
	float aceleracion;
	float velMaxima;
	
	Tanque(const char *Modelo, const char *rutaTextura, punto Posicion, float velMaxima = 2.0f, float aceleracion = 0.5f);
	~Tanque();
	
	void dibujar() override;
	static void updateWrapper(int);

private:
	unsigned int velActualizacion;

	void update();
};

extern "C" {
	GLMmodel* readOBJ(const char* obj);
}