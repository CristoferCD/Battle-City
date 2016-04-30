/*
Cristofer Canosa Dominguez
28/04/2016
Proyecto Final CoGa - Battle City
*/

#include <vector>
#include <GL\glut.h>
#include <GL\SOIL.h>
#include "figuras.h"
#include "glCallback.h"
#include "Objeto.h"
#include "Mapa.h"

const Mapa *mapa;

void cargarLuces() {

}

void initComponents() {
	mapa = new Mapa("mapas\\nivel1.map");
}

void Display() {
	for (std::vector<Objeto*>::const_iterator it = (mapa->objetosDestruibles.begin()); it != mapa->objetosDestruibles.end();) {
		if (glCallback::testColision(bala, it))
			it = objetosDestruibles.erase(it);
	}
}