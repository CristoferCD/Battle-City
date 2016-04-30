#pragma once

#include "Tanque.h"
#include "Objeto.h"
#include "auxiliares.h"

class Coche;
class glCallback;
static glCallback *instanciaCB;


static class glCallback
{
public:

	enum camaras {VIEW_1P = 0, VIEW_3P, VIEW_DRONE};
	camaras camaraActual;
	Tanque *tanque;
	GLint height = 960;
	GLint width = 540;

	glCallback(camaras camaraDefecto, Tanque *Tanque);
	~glCallback();

	void joy(unsigned int mask, int x, int y, int z);
	void camara(camaras i);
	void teclado(unsigned char c, int x, int y);
	void tecladoUp(unsigned char key, int x, int y);
	void resize(GLint newWidth, GLint newHeight);
	void cargarTextura(GLuint &textura, const char* rutaTextura);
	bool testColision(Objeto *a, Objeto *b);

private:
	const float PI = 3.1416;
	bool iluminacion = true;
	void cam1persona();
	void cam3persona();
	void camDrone();
	void iluminarTanque();
};

extern "C" {
	void joyWrapper(unsigned int mask, int x, int y, int z);
	void camaraWrapper(glCallback::camaras i);
	void tecladoWrapper(unsigned char c, int x, int y);
	void tecladoUpWrapper(unsigned char key, int x, int y);
	void resizeWrapper(GLint newWidth, GLint newHeight);
}
