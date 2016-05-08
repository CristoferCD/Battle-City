/*
Cristofer Canosa Dominguez
28/04/2016
Proyecto Final CoGa - Battle City
*/

#include <vector>
#include <iostream>
#include <GL\glut.h>
#include <GL\SOIL.h>
#include <thread>
#include <chrono>
#include <mutex>
#include "figuras.h"
#include "glCallback.h"
#include "Mapa.h"
#include "Enemigo.h"
#include "Tanque.h"

using namespace std;

Mapa *mapa;
Tanque *tanque;
glCallback *callback;
thread physics;

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

 void colisiones() {
	 while (1) {
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
		 for (auto it = mapa->objetosDestruibles.begin(); it != mapa->objetosDestruibles.end(); it++) {
			 if (glCallback::testColision(tanque->bala, *it)) {
				 mapa->mtx.lock();
				 mapa->objetosDestruibles.erase(it); 
				 mapa->mtx.unlock();
				 if(it != mapa->objetosDestruibles.begin()) it = prev(it);
				 tanque->bala->enAire = false;
			 }
			 for each (Enemigo* ene in enemigos)
			 {
				 if (glCallback::testColision(ene->bala, *it)) {
					 mapa->mtx.lock();
					 mapa->objetosDestruibles.erase(it); 
					 mapa->mtx.unlock();
					 if(it != mapa->objetosDestruibles.begin()) it = prev(it);
					 ene->bala->enAire = false;
				 }
			 }
		 }
		 for (auto it = mapa->objetosEstaticos.begin(); it != mapa->objetosEstaticos.end(); it++) {
			 if (glCallback::testColision(tanque->bala, *it)) {
				 tanque->bala->enAire = false;
			 }
			 for each (Enemigo* ene in enemigos)
			 {
				 if (glCallback::testColision(ene->bala, *it)) {
					 ene->bala->enAire = false;
				 }
			 }
		 }
		 this_thread::sleep_for(chrono::milliseconds(5));
	 }
 }

void initComponents() {
	mapa = new Mapa("mapas\\nivel1.map");
	tanque = new Tanque("models\\MainTank.obj", "", mapa->getPosicion(punto(3, 1, 2)));
	callback = new glCallback(tanque, glCallback::VIEW_DRONE);
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(0.5, 25.5, 2)), 1, 0.1));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(12.5, 25.5, 2)), 1, 0.1));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(24, 25.5, 2)), 1, 0.1));
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
	
	physics = thread(colisiones);
	
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