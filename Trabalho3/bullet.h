#ifndef BALA_H
#define BALA_H

#include "player.h"
#include "utils.hpp"
#include <math.h>

struct Bullet{

	double speed;

	float x,y;

	Circulo referenceCircle;

	float playerAngle, armAngle, playerRadius;

	Bullet(Player&, float topoX, float topoY);

	void draw();

	void updatePosition(float);

	void desenhaCirculo(int r, GLfloat red, GLfloat blue, GLfloat green);

	bool colisaoBalaObs(std::vector<Circulo>& obsMenorList);

};

#endif