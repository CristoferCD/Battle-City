#pragma once
#include <GL\glut.h>


class punto {
public:
	punto() {}
	punto(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat z = 0.0;

	punto operator*(punto a) {
		return punto(a.x*x, a.y*y, a.z*z);
	}

	punto& operator += (const punto& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
};

class AABB {
public:
	AABB() : c(), r() {}
	AABB(const punto& centro, const punto& radios) : c(centro), r(radios) {}

	punto c;
	punto r;

	void dibujar() {
		glPushMatrix();
		glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
		glVertex3f(c.x - r.x, c.y - r.y, c.z);
		glVertex3f(c.x + r.x, c.y - r.y, c.z);
		glVertex3f(c.x + r.x, c.y + r.y, c.z);
		glVertex3f(c.x - r.x, c.y + r.y, c.z);
		glEnd();
		glPopMatrix();
	}
};
