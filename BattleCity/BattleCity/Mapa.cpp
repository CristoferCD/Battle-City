#include "Mapa.h"
#include <fstream>
#include <iostream>
#include <glm.h>
#include "figuras.h"

using namespace std;

Mapa::Mapa(const char* rutaMapa, int tileSize, int numTiles)
{
	std::ifstream inFile;
	char idObjeto;
	inFile.open(rutaMapa);
	if (!inFile.is_open()) {
		cout << "Error al abrir el mapa";
		exit(1);
	}
	GLuint textVacio = 0;
	GLuint textLadrillo = 0;
	GLuint textMuro = 0;
	GLuint textArbusto = 0;
	GLuint textAgua = 0;

	GLuint listaCubo = glGenLists(1);
	glNewList(listaCubo, GL_COMPILE);
	cubo();
	glEndList();

	if (!listaCubo) {
		cout << "Error al crear la listaCubo";
		exit(1);
	}

	//Creaci�n de objetos del mapa
	int i = 0, j = numTiles;
	while (!inFile.eof()) {
		inFile >> idObjeto;
		cout << idObjeto << endl;
		switch (idObjeto)
		{
		case VACIO:
			if (!textVacio) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), "textures\\suelo.jpg");
				textVacio = cubo->getTextura();
				objetosNoDestruibles.push_back(cubo);
			}
			else {
				objetosNoDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), textVacio));
			}
			break;
		case LADRILLO:
			if (!textLadrillo) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), "textures\\ladrillo.jpg");
				textLadrillo = cubo->getTextura();
				objetosDestruibles.push_back(cubo);
			}
			else {
				objetosDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), textLadrillo));
			}
			break;
		case MURO:
			if (!textMuro) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), "textures\\muro.jpg");
				textMuro = cubo->getTextura();
				objetosEstaticos.push_back(cubo);
			}
			else {
				objetosEstaticos.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), textMuro));
			}
			break;
		case ARBUSTO:
			if (!textArbusto) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), "textures\\arbusto.jpg");
				textArbusto = cubo->getTextura();
				objetosNoDestruibles.push_back(cubo);
			}
			else {
				objetosNoDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), textArbusto));
			}
			break;
		case AGUA:
			if (!textAgua) {
				Objeto *cubo = new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), "textures\\agua.jpg");
				textAgua = cubo->getTextura();
				objetosNoDestruibles.push_back(cubo);
			}
			else {
				objetosNoDestruibles.push_back(new Objeto(listaCubo, punto(1.0, 1.0, 1.0), punto(i*tileSize*2, j*tileSize*2, 0.0f), punto(tileSize, tileSize, 1), textAgua));
			}
			break;
		default:
			break;
		}
		i++;
		i = i%numTiles;
		if (i == 0) j--;
		//Final del mapa (y, en teor�a, del archivo)
		if (j == 0) break;
	}

	//Carga la base en 3d (para la versi�n final)
	/*
	GLuint listaBase = glmList(glmReadOBJ("modelos\\base.obj"), GL_FLAT | GL_TEXTURE);
	objetosDestruibles.push_back(new Objeto(listaBase, punto(1.0, 1.0, 1.0), punto(numTiles*tileSize / 2, numTiles*tileSize / 2, 0.0f), punto(2.0, 2.0, 1.0), "tex\\base.jpg"));
	*/
	//GLuint listaBase = glGenLists(1);
	//glNewList(listaBase, GL_COMPILE);
	//plano();
	//glEndList();
	//objetosDestruibles.push_back(new Objeto(listaBase, punto(1.0, 1.0, 1.0), punto(numTiles*tileSize / 2, numTiles*tileSize / 2, 0.0f), punto(2.0, 2.0, 1.0), "tex\\base.jpg"));
	//
	inFile.close();
}


Mapa::~Mapa()
{
}

void Mapa::dibujar()
{
	for each (Objeto *var in objetosDestruibles)
	{
		var->dibujar();
	}
	for each (Objeto *var in objetosEstaticos)
	{
		var->dibujar();
	}
	for each (Objeto *var in objetosNoDestruibles)
	{
		var->dibujar();
	}
}
