#ifndef BALA_H
#define BALA_H

#include "circle.h"
#include <math.h>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>

class Bullet{

	public:

		double speed;

		float x,y;

		Circulo referenceCircle;

		float playerAngle, armAngle, playerRadius;

		Bullet();

		void draw();

		void updatePosition(float);

		void desenhaCirculo(int r, GLfloat red, GLfloat blue, GLfloat green);

		bool colisaoBalaObs(std::vector<Circulo>& obsMenorList);

};

#endif