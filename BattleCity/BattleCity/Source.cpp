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
#include "Mapa.h"
#include "Enemigo.h"
#include "Tanque.h"

using namespace std;

Mapa *mapa;
Tanque *tanque;
glCallback *callback;

vector <Proyectil*> balas;
vector <Enemigo*> enemigos;

void cargarLuces() {

}

 void moveUpdate(int a) {
	tanque->update();

	for each (Enemigo* ene in enemigos)
	{
		ene->update();
		ene->IA();
	}
	glutTimerFunc(15, moveUpdate, 0);
}

void initComponents() {
	tanque = new Tanque("models\\MainTank.obj", "", punto(0, 10, 2));
	mapa = new Mapa("mapas\\nivel1.map");
	callback = new glCallback(tanque, glCallback::VIEW_DRONE);
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(0, 26, 2)), 1, 0.6));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(13, 26, 2)), 1, 0.6));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(25, 26, 2)), 1, 0.6));
}

void display() {
	camaraWrapper(callback->camaraActual);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1, 1, 1);
	mapa->dibujar();
	tanque->dibujar();	
	for each (Enemigo *ene in enemigos)
	{
		ene->dibujar();
	}


	//Colisiones de tanque
	for each (Objeto *var in mapa->objetosDestruibles) {
		if (glCallback::testColision(tanque, var)) {
			tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
			tanque->enColision = true;
		}
		for each (Enemigo *ene in enemigos)
			if (glCallback::testColision(ene, var))
				ene->colision();
	}
	for each (Objeto *var in mapa->objetosEstaticos) {
		if (glCallback::testColision(tanque, var)) {
			tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
			tanque->enColision = true;
		}
		for each (Enemigo *ene in enemigos)
			if (glCallback::testColision(ene, var))
				ene->colision();
	}
	//Colisiones de bala
	//if (tanque->bala->enAire) {
		for (int i = 0; i < mapa->objetosDestruibles.size(); i++) {
			if (glCallback::testColision(tanque->bala, mapa->objetosDestruibles.at(i))) {
				mapa->objetosDestruibles.erase(mapa->objetosDestruibles.begin() + i);
				tanque->bala->enAire = false;
				tanque->disparosRealizados--;
			}
			for each (Enemigo *ene in enemigos)
				if (glCallback::testColision(ene->bala, mapa->objetosDestruibles.at(i))) {
					mapa->objetosDestruibles.erase(mapa->objetosDestruibles.begin() + i);
					ene->bala->enAire = false;
					ene->disparosRealizados--;
				}					
		}
		for (int i = 0; i < mapa->objetosEstaticos.size(); i++) {
			if (glCallback::testColision(tanque->bala, mapa->objetosEstaticos.at(i))) {
				tanque->bala->enAire = false;
				tanque->disparosRealizados--;
			}
			for each (Enemigo *ene in enemigos)
				if (glCallback::testColision(ene->bala, mapa->objetosEstaticos.at(i))) {
					ene->bala->enAire = false;
					ene->disparosRealizados--;
				}
		}
	//}	

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
	glutTimerFunc(15, moveUpdate, 0);
	
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