/*
Cristofer Canosa Dominguez
28/04/2016
Proyecto Final CoGa - Battle City
*/

#include <vector>
#include <iostream>
#include <GL\glut.h>
#include <GL\SOIL.h>
#include "figuras.h"
#include "glCallback.h"
#include "Objeto.h"
#include "Mapa.h"

using namespace std;

Mapa *mapa;
Tanque *tanque;
glCallback *callback;

void cargarLuces() {

}

void initComponents() {
	tanque = new Tanque("models\\MainTank.obj", "", punto(0, 10, 2));
	mapa = new Mapa("mapas\\nivel1.map");
	callback = new glCallback(tanque, glCallback::VIEW_DRONE);
}

void display() {
	camaraWrapper(callback->camaraActual);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1, 1, 1);
	mapa->dibujar();
	tanque->dibujar();
	glColor3f(1, 0, 0);
	//TODO vector de objetos para dibujar proyectiles
	//TODO override dibujar para incluir la comprobación
	if (tanque->bala->enAire)
		tanque->bala->dibujar();

	for each (Objeto *var in mapa->objetosDestruibles) {
		if (glCallback::testColision(tanque, var)) {
			tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
			tanque->colision = true;
		}
	}
	for each (Objeto *var in mapa->objetosEstaticos) {
		if (glCallback::testColision(tanque, var)) {
			tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
			tanque->colision = true;
		}
	}
	if(tanque->bala->enAire)
		for (int i = 0; i < mapa->objetosDestruibles.size(); i++) {
			if (glCallback::testColision(tanque->bala, mapa->objetosDestruibles.at(i))) {
				mapa->objetosDestruibles.erase(mapa->objetosDestruibles.begin() + i);
				tanque->bala->enAire = false;
				//Tanque -> disparosRealizados--;
			}
		}
	

	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(960, 540);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Battle City");
	initComponents();

	glutDisplayFunc(display);
	glutKeyboardFunc(tecladoWrapper);
	glutKeyboardUpFunc(tecladoUpWrapper);
	glutReshapeFunc(resizeWrapper);
	//glutJoystickFunc(joyWrapper, 100);
	glutTimerFunc(15, tanque->updateWrapper, 0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glClearDepth(1.0f);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_FOG);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	glutMainLoop();
	return 0;
}