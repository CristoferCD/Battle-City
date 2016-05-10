#include <Windows.h>
#include "glCallback.h"
#include <math.h>
#include <GL\SOIL.h>

glCallback::glCallback(Tanque *Tanque, camaras camaraDefecto)
{
	camaraActual = camaraDefecto;
	tanque = Tanque;
	instanciaCB = this;
}

glCallback::~glCallback()
{
}

//No adaptada al tanque, desactivada por ahora
void glCallback::joy(unsigned int mask, int x, int y, int z)
{
	//El valor máximo del stick es 1000, así produce un aumento entre 0.1 y 0.5
	if (y > 300 || y < -300)
		tanque->vel -= y / 5000.0f;
	x = tanque->vel < 0 ? -x : x;
	if (x > 300 || x < -300)
		tanque->rotacion -= x / 1000.0f;

	if (mask == GLUT_JOYSTICK_BUTTON_A)
		tanque->vel = 0;
	if (mask == GLUT_JOYSTICK_BUTTON_B) {
		tanque->posActual.x = 0;
		tanque->posActual.y = 0;
	}
	if (mask == GLUT_JOYSTICK_BUTTON_C) {
		switch (camaraActual)
		{
		case glCallback::VIEW_1P:
			camaraActual = camaras::VIEW_3P;
			break;
		case glCallback::VIEW_3P:
			camaraActual = camaras::VIEW_DRONE;
			break;
		case glCallback::VIEW_DRONE:
			camaraActual = camaras::VIEW_1P;
			break;
		default:
			break;
		}
	}

	tanque->vel = tanque->vel > tanque->velMaxima ? tanque->velMaxima : tanque->vel;	//Velocidad máxima

	glutPostRedisplay();						

}

void glCallback::camara(camaras i)
{
	switch (i)
	{
	case VIEW_1P:
		cam1persona();
		break;
	case VIEW_3P:
		cam3persona();
		break;
	case VIEW_DRONE:
		camDrone();
		break;
	default:
		break;
	}
}

void glCallback::teclado(unsigned char c, int x, int y)
{
	if (!tanque->enColision) {
		switch (c) {
		case 'w':
			if (tanque->rotacion != 90) tanque->rotacion = 90;
			tanque->vel += tanque->aceleracion;
			break;
		case 'a':
			if (tanque->rotacion != 180) tanque->rotacion = 180;
			tanque->vel += tanque->aceleracion;
			break;
		case 's':
			if (tanque->rotacion != 270) tanque->rotacion = 270;
			tanque->vel += tanque->aceleracion;
			break;
		case 'd':
			if (tanque->rotacion != 0) tanque->rotacion = 0;
			tanque->vel += tanque->aceleracion;
			break;
		case 'j':
			if (!tanque->bala->enAire)
				PlaySound(TEXT("sounds\\disparo.wav"), NULL, SND_ASYNC | SND_FILENAME);
			tanque->disparar();
			break;
		default:
			break;
		}

		tanque->vel = tanque->vel > tanque->velMaxima ? tanque->velMaxima : tanque->vel;	//Velocidad máxima

		glutPostRedisplay();
	}
}

void glCallback::tecladoUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'a':
	case 's':
	case 'd':
		tanque->vel = tanque->enColision ? -tanque->aceleracion : 0;
		tanque->enColision = false;
		break;
	case 'c':
		switch (camaraActual)
		{
		case glCallback::VIEW_DRONE:
			camaraActual = camaras::VIEW_3P;
			break;
		case glCallback::VIEW_3P:
			camaraActual = camaras::VIEW_DRONE;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void glCallback::cam1persona()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	punto objetivo;
	objetivo.x = tanque->posActual.x + cos(tanque->rotacion*M_PI / 180)*60.0;
	objetivo.y = tanque->posActual.y + sin(tanque->rotacion*M_PI / 180)*60.0;
	objetivo.z = tanque->posActual.z;
	gluPerspective(60.0, (GLdouble)this->width / this->height, 0.1, 800.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tanque->posActual.x + cos(tanque->rotacion*M_PI / 180) * 10, tanque->posActual.y + sin(tanque->rotacion*M_PI / 180)*10.0, 30,
		objetivo.x, objetivo.y, objetivo.z, 0, 0, 1);
}

void glCallback::cam3persona()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	punto objetivo;
	objetivo.x = tanque->posActual.x;
	objetivo.y = tanque->posActual.y + 80.0;
	objetivo.z = tanque->posActual.z;
	gluPerspective(60.0, (GLdouble)this->width / this->height, 0.1, 800.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tanque->posActual.x, tanque->posActual.y - 50.0, 50, objetivo.x, objetivo.y, objetivo.z, 0, 0, 1);
}

void glCallback::camDrone()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)this->width / this->height, 1.0, 100.0);
	//glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tanque->posActual.x, tanque->posActual.y, 100, tanque->posActual.x, tanque->posActual.y, tanque->posActual.z, 0, 1, 0);
}

void glCallback::iluminarTanque()
{
	GLfloat luzPos[] = { tanque->posActual.x, tanque->posActual.y, tanque->posActual.z, 1.0 };
	GLfloat luzDir[] = { cos(tanque->rotacion*M_PI / 180) , sin(tanque->rotacion*M_PI / 180) , 0.1 };

	glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, luzDir);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 64.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 64.0);
}

void glCallback::resize(GLint newWidth, GLint newHeight) {
	if (newHeight == 0)
		newHeight = 1;

	this->width = newWidth;
	this->height = newHeight;
	// Ajusta la vista a las dimensiones de la ventana
	glViewport(0, 0, this->width, this->height);

	// Reinicia el sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Establece el volumen de trabajo
	gluPerspective(60.0f, (GLdouble)this->width / this->height, 1.0f, 1000.0f);
}

void glCallback::cargarTextura(GLuint & textura, const char * rutaTextura)
{
	textura = SOIL_load_OGL_texture(rutaTextura, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

bool glCallback::testColision(Objeto *a, Objeto *b)
{
	bool xOverlap = true;
	bool yOverlap = true;
	bool zOverlap = true;

	punto centroA = a->boundingBox.c;
	punto centroB = b->boundingBox.c;
	punto radioA = a->boundingBox.r;
	punto radioB = b->boundingBox.r;

	if (fabs(centroA.x - centroB.x) > (radioA.x + radioB.x)) xOverlap = false;
	if (fabs(centroA.y - centroB.y) > (radioA.y + radioB.y)) yOverlap = false;
	//Las colisiones se calculan en 2D
	//if (fabs(centroA.z - centroB.z) > (radioA.z + radioB.z)) zOverlap = false;

	return xOverlap && yOverlap && zOverlap;
}


void joyWrapper(unsigned int mask, int x, int y, int z)
{
	instanciaCB->joy(mask, x, y, z);
}
void camaraWrapper(glCallback::camaras i)
{
	instanciaCB->camara(i);
}
void tecladoWrapper(unsigned char c, int x, int y)
{
	instanciaCB->teclado(c, x, y);
}
void tecladoUpWrapper(unsigned char key, int x, int y)
{
	instanciaCB->tecladoUp(key, x, y);
}
void resizeWrapper(GLint newWidth, GLint newHeight) {
	instanciaCB->resize(newWidth, newHeight);
}