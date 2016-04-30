#include "Mapa.h"
#include <fstream>
#include <iostream>
#include <glm.h>
#include "figuras.h"

Mapa::Mapa(const char* rutaMapa, int tileSize, int numTiles)
{
	std::ifstream inFile;
	int idObjeto;
	inFile.open(rutaMapa);
	if (!inFile) {
		std::cout << "Error al abrir el mapa";
		exit(1);
	}
	GLuint textVacio = 0;
	GLuint textLadrillo = 0;
	GLuint textMuro = 0;
	GLuint textArbusto = 0;
	GLuint textAgua = 0;

	GLuint listaCubo = glGenLists(1);
	glNewList(listaCubo, GL_COMPILE);
	glScalef(tileSize, tileSize, tileSize);
	cubo();
	glEndList();

	if (!listaCubo) {
		std::cout << "Error al crear la listaCubo";
		exit(1);
	}

	//Creación de objetos del mapa
	int i = 0, j = numTiles;
	while (inFile >> idObjeto) {
		switch (idObjeto)
		{
		case VACIO:
			if (!textVacio) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), "textures\\suelo.png");
				textVacio = cubo->getTextura();
				objetosEstaticos.push_back(cubo);
			}
			else {
				objetosEstaticos.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), textVacio));
			}
			break;
		case LADRILLO:
			if (!textLadrillo) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), "textures\\ladrillo.png");
				textVacio = cubo->getTextura();
				objetosDestruibles.push_back(cubo);
			}
			else {
				objetosDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), textLadrillo));
			}
			break;
		case MURO:
			if (!textMuro) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), "textures\\muro.png");
				textVacio = cubo->getTextura();
				objetosEstaticos.push_back(cubo);
			}
			else {
				objetosEstaticos.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), textMuro));
			}
			break;
		case ARBUSTO:
			if (!textArbusto) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), "textures\\arbusto.png");
				textVacio = cubo->getTextura();
				objetosNoDestruibles.push_back(cubo);
			}
			else {
				objetosNoDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), textArbusto));
			}
			break;
		case AGUA:
			if (!textAgua) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), "textures\\agua.png");
				textVacio = cubo->getTextura();
				objetosNoDestruibles.push_back(cubo);
			}
			else {
				objetosNoDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize, j*tileSize, 0.0f), punto(tileSize, tileSize, 1.0f), textAgua));
			}
			break;
		default:
			break;
		}
		i += tileSize;
		i = i%numTiles;
		if (i == 0) j -= tileSize;
	}

	//Carga la base
	GLuint listaBase = glmList(glmReadOBJ("modelos\\base.obj"), GL_FLAT | GL_TEXTURE);
	objetosDestruibles.push_back(new Objeto(listaBase, punto(1.0, 1.0, 1.0), punto(numTiles*tileSize / 2, numTiles*tileSize / 2, 0.0f), punto(2.0, 2.0, 1.0), "tex\\base.png"));

	inFile.close();
}


Mapa::~Mapa()
{
}
