#include "glCallback.h"
#include <math.h>

glCallback::glCallback(camaras camaraDefecto, Coche *Coche)
{
	camaraActual = camaraDefecto;
	coche = Coche;
	instanciaCB = this;
}

glCallback::~glCallback()
{
}

void glCallback::joy(unsigned int mask, int x, int y, int z)
{
	//El valor máximo del stick es 1000, así produce un aumento entre 0.1 y 0.5
	if (y > 300 || y < -300)
		coche->vel -= y / 5000.0f;
	x = coche->vel < 0 ? -x : x;
	if (x > 300 || x < -300)
		coche->rotacion -= x / 1000.0f;

	if (mask == GLUT_JOYSTICK_BUTTON_A)
		coche->vel = 0;
	if (mask == GLUT_JOYSTICK_BUTTON_B) {
		coche->posActual.x = 0;
		coche->posActual.y = 0;
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

	coche->vel = coche->vel > coche->velMaxima ? coche->velMaxima : coche->vel;	//Velocidad máxima
	coche->vel = coche->vel < coche->velMaximaNegativa ? coche->velMaximaNegativa : coche->vel;	//Velocidad máxima hacia atrás

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
	switch (c) {
	case 'w':
		coche->vel += coche->aceleracion;
		break;
	case 'a':
		if (coche->vel > 0)
			coche->rotacion += 1.7;
		else
			coche->rotacion -= 1.7f;
		break;
	case 's':
		if (coche->vel > 0)
			coche->vel -= coche->frenado;
		else
			coche->vel -= coche->aceleracionNegativa;
		break;
	case 'd':
		if (coche->vel > 0)
			coche->rotacion -= 1.7f;
		else
			coche->rotacion += 1.7f;
		break;
	case 'i':
		iluminacion=!iluminacion;
		break;
	default:
		break;
	}

	coche->vel = coche->vel > coche->velMaxima ? coche->velMaxima : coche->vel;	//Velocidad máxima
	coche->vel = coche->vel < coche->velMaximaNegativa ? coche->velMaximaNegativa : coche->vel;	//Velocidad máxima hacia atrás

	glutPostRedisplay();
}

void glCallback::tecladoEsp(int cursor, int x, int y)
{
	switch (cursor)
	{
	case GLUT_KEY_F1:
		camaraActual = VIEW_1P;
		break;
	case GLUT_KEY_F2:
		camaraActual = VIEW_3P;
		break;
	case GLUT_KEY_F3:
		camaraActual = VIEW_DRONE;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void glCallback::cam1persona()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	punto objetivo;
	objetivo.x = coche->posActual.x + cos(coche->rotacion*PI / 180)*60.0;
	objetivo.y = coche->posActual.y + sin(coche->rotacion*PI / 180)*60.0;
	objetivo.z = coche->posActual.z;
	gluPerspective(60.0, (GLdouble)this->width / this->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(coche->posActual.x + cos(coche->rotacion*PI / 180) * 10, coche->posActual.y + sin(coche->rotacion*PI / 180)*10.0, 30,
		objetivo.x, objetivo.y, objetivo.z, 0, 0, 1);
	if (iluminacion) {
		glEnable(GL_LIGHT0);
		iluminarCoche();
	}
	else
		glDisable(GL_LIGHT0);
}

void glCallback::cam3persona()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	punto objetivo;
	objetivo.x = coche->posActual.x + cos(coche->rotacion*PI / 180)*80.0;
	objetivo.y = coche->posActual.y + sin(coche->rotacion*PI / 180)*80.0;
	objetivo.z = coche->posActual.z;
	gluPerspective(60.0, (GLdouble)this->width / this->height, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(coche->posActual.x - cos(coche->rotacion*PI / 180) * 30, coche->posActual.y - sin(coche->rotacion*PI / 180)*30.0, 30,
		objetivo.x, objetivo.y, objetivo.z, 0, 0, 1);
	if (iluminacion) {
		glEnable(GL_LIGHT0);
		iluminarCoche();
	}
	else
		glDisable(GL_LIGHT0);
}

void glCallback::camDrone()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(coche->posActual.x, coche->posActual.y, 300, coche->posActual.x, coche->posActual.y, coche->posActual.z, 0, 1, 0);
	if (iluminacion) {
		glEnable(GL_LIGHT0);
		iluminarCoche();
	}
	else
		glDisable(GL_LIGHT0);
}

void glCallback::iluminarCoche()
{
	GLfloat luzPos[] = { coche->posActual.x, coche->posActual.y, coche->posActual.z, 1.0 };
	GLfloat luzDir[] = { cos(coche->rotacion*PI / 180) , sin(coche->rotacion*PI / 180) , 0.1 };

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

	// Establece el volumen de trabajo
	gluPerspective(45.0f, (GLdouble)this->width / this->height, 1.0f, 500.0f);
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
	if (fabs(centroA.z - centroB.z) > (radioA.z + radioB.z)) zOverlap = false;

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
void tecladoEspWrapper(int cursor, int x, int y)
{
	instanciaCB->tecladoEsp(cursor, x, y);
}
void resizeWrapper(GLint newWidth, GLint newHeight) {
	instanciaCB->resize(newWidth, newHeight);
}