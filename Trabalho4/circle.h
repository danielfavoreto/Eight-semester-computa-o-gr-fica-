#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include <iostream>

class Circulo{
	public:

		Circulo();

		void desenhaCirculo();

		void fillColor(std::string color);		

		float dist2Points(float centroX, float centroY, float x, float y);

		float raio;

		float centroX,centroY;

		std::string cor;
};

#endif