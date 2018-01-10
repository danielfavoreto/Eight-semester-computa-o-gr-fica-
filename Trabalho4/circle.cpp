#include "circle.h"
#include <math.h>
#include <GL/freeglut.h>
#include <string.h>

Circulo::Circulo(){

	raio = 0;
	centroX = 0;
	centroY = 0;
	cor = "blue";
}

void Circulo::desenhaCirculo(){

	GLfloat twicePi = 2.0f * M_PI;

	glPushMatrix();

		glTranslatef(centroX,centroY,0);

		glBegin(GL_TRIANGLE_FAN);

			fillColor(cor);

			glVertex3d(0,0,0.0);

			for (int i = 0; i <= 360; i++){

				glVertex3f(raio * cos(i * twicePi/360),raio * sin(i * twicePi/360),0.0);

			}

		glEnd();

	glPopMatrix();
}

void Circulo::fillColor(std::string color){

	if (0 == strcmp(color.c_str(),"red")){

		glColor3f(1,0,0);
	}
	else if (0 == strcmp(color.c_str(),"blue")){

		glColor3f(0,0,1);
	}
	else if(0 == strcmp(color.c_str(),"green")){

		glColor3f(0,1,0);
	}
	else if(0 == strcmp(color.c_str(),"white")){

		glColor3f(1,1,1);
	}
	else { // black color

		glColor3f(0,0,0);
	}
}

float Circulo::dist2Points(float centroX, float centroY, float x, float y){

	return (sqrt((centroX - x)*(centroX - x) + (centroY - y)*(centroY - y)));
}