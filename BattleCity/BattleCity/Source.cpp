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
	tanque = new Tanque("models\\MainTank.obj", "", punto(50, 50, 50));
	//mapa = new Mapa("mapas\\nivel1.map");
	callback = new glCallback(tanque, glCallback::VIEW_DRONE);
}

void display() {
	camaraWrapper(callback->camaraActual);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1, 1, 1);
	//mapa->dibujar();
	//tanque->dibujar();
	/*Tanque - dibujar*/
	glPushMatrix();
	glTranslatef(tanque->posActual.x, tanque->posActual.y, tanque->posActual.z);
	cout << "Tanque->posActual = [" << tanque->posActual.x << "," << tanque->posActual.y << "," << tanque->posActual.z << "]" << endl;
	glRotatef(tanque->rotacion, 0, 0, 1);
	cout << "Tanque->rotacion = " << tanque->rotacion << endl;
	glRotatef(90, 1, 0, 0);
	//glScalef(escala.x, escala.y, escala.z);
	//glBindTexture(GL_TEXTURE_2D, textura);
	//Temporalmente color s�lido, hasta crear una textura
	glColor3f(1.0f, 1.0f, 0.0f);
	//glCallList(idLista);
	glutSolidTeapot(20);
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glPopMatrix();
	/*
	for (auto it = (mapa->objetosDestruibles.begin()); it != mapa->objetosDestruibles.end();) {
		if (glCallback::testColision(bala, it))
			it = mapa->objetosDestruibles.erase(it);
	}
	*/

	glutSwapBuffers();
	glFlush();
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

	//glEnable(GL_NORMALIZE);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_FOG);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	glutMainLoop();
	return 0;
}