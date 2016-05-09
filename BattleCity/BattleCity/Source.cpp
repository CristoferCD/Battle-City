/*
Cristofer Canosa Dominguez
28/04/2016
Proyecto Final CoGa - Battle City
*/

#include <vector>
#include <iostream>
#include <mutex>
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
mutex mtxEne;		//Mutex para el vector de enemigos
int golpeado = 0;	//Indica el número de frames durante los que la niebla estará roja

vector <Proyectil*> balas;
vector <Enemigo*> enemigos;

void cargarLuces() {

}

 void moveUpdate(int a) {
	tanque->update();

	for each (Enemigo* ene in enemigos)
	{
		ene->IA();
	}
	glutTimerFunc(15, moveUpdate, 0);
}

 void colisiones() {
	 while (1) {
		 //Colisiones de tanque con Objetos Destruibles
		 for each (Objeto *var in mapa->objetosDestruibles) {
			 if (glCallback::testColision(tanque, var)) {
				 tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
				 tanque->enColision = true;
			 }
			 //Colisiones de enemigos con Objetos Destruibles
			 for each (Enemigo *ene in enemigos) {
				 if (glCallback::testColision(ene, var)) {
					 do {
						 ene->retroceder();
					 } while (glCallback::testColision(ene, var));
					 ene->cambiarDireccion();
				 }
			 }
		 }
		 //Colisiones de tanque con Objetos Estáticos
		 for each (Objeto *var in mapa->objetosEstaticos) {
			 if (glCallback::testColision(tanque, var)) {
				 tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
				 tanque->enColision = true;
			 }
			 //Colisiones de enemigos con Objetos Estáticos
			 for each (Enemigo *ene in enemigos) {
				 if (glCallback::testColision(ene, var)) {
					 do {
						 ene->retroceder();
					 } while (glCallback::testColision(ene, var));
					 ene->cambiarDireccion();
				 }	
			 }
		 }
		 //Colisiones de tanque con Objetos no Destruibles
		 for each (Objeto *var in mapa->objetosNoDestruibles) {
			 if (glCallback::testColision(tanque, var)) {
				 tanque->vel = tanque->vel != 0 ? 0 : -tanque->aceleracion;
				 tanque->enColision = true;
			 }
			 //Colisiones de enemigos con Objetos no Destruibles
			 for each (Enemigo *ene in enemigos) {
				 if (glCallback::testColision(ene, var)) {
					 do {
						 ene->retroceder();
					 } while (glCallback::testColision(ene, var));
					 ene->cambiarDireccion();
				 }
			 }
		 }
		 //Colisiones de balas con Objetos Destruibles
		 for (int i = 0; i < mapa->objetosDestruibles.size(); i++) {
			 if (glCallback::testColision(tanque->bala, mapa->objetosDestruibles[i])) {
				 mapa->mtx.lock();
				 mapa->objetosDestruibles.erase(mapa->objetosDestruibles.begin() + i);
				 mapa->mtx.unlock();
				 tanque->bala->enAire = false;
			 }
			 else {
				 //Balas de enemigos
				 for each (Enemigo* ene in enemigos)
				 {
					 if (glCallback::testColision(ene->bala, mapa->objetosDestruibles[i])) {
						 mapa->mtx.lock();
						 mapa->objetosDestruibles.erase(mapa->objetosDestruibles.begin() + i);
						 mapa->mtx.unlock();
						 ene->bala->enAire = false;
						 break;	//No comprueba el resto de enemigos
					 }
				 }
			 }
		 }
		 //Colisiones de balas con Objetos Estáticos
		 for (auto it = mapa->objetosEstaticos.begin(); it != mapa->objetosEstaticos.end(); it++) {
			 if (glCallback::testColision(tanque->bala, *it)) {
				 tanque->bala->enAire = false;
			 }
			 //Balas de enemigos
			 for each (Enemigo* ene in enemigos)
			 {
				 if (glCallback::testColision(ene->bala, *it)) {
					 ene->bala->enAire = false;
				 }
			 }
		 }
		 //Colisiones de bala propia con enemigos
		 for (int i = 0; i < enemigos.size(); i++) {
			 if (glCallback::testColision(tanque->bala, enemigos[i])) {
				 mtxEne.lock();
				 enemigos.erase(enemigos.begin() + i);
				 mtxEne.unlock();
				 tanque->bala->enAire = false;
				 break;
			 }
		 }
		 //Colisiones entre balas enemigas y propias, las enemigas se atraviesan entre si
		 for each(Enemigo *ene in enemigos) {
			 if (glCallback::testColision(ene->bala, tanque->bala)) {
				 ene->bala->enAire = false;
				 tanque->bala->enAire = false;
				 break;
			 }
		 }
		 //Colisiones entre balas enemigas y el jugador
		 for each (Enemigo *ene in enemigos) {
			 if (glCallback::testColision(ene->bala, tanque)) {
				 ene->bala->enAire = false;
				 golpeado = 20;
			 }
		 }
		 this_thread::sleep_for(chrono::milliseconds(5));
	 }
 }

void initComponents() {
	mapa = new Mapa("mapas\\nivel2.map");
	tanque = new Tanque("models\\MainTank.obj", "", mapa->getPosicion(punto(3, 1, 2)));
	callback = new glCallback(tanque, glCallback::VIEW_DRONE);
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(0.5, 25.5, 2)), 1, 0.3));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(12.5, 25.5, 2)), 1, 0.3));
	enemigos.push_back(new Enemigo("models\\MainTank.obj", "", mapa->getPosicion(punto(24, 25.5, 2)), 1, 0.3));
}

void display() {
	camaraWrapper(callback->camaraActual);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1, 1, 1);
	mapa->dibujar();
	tanque->dibujar();
	tanque->boundingBox.dibujar();

	mtxEne.lock();
	for each (Enemigo *ene in enemigos)
	{
		ene->dibujar();
	}
	mtxEne.unlock();

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