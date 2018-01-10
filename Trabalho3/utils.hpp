#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define PI 3.1415926535897932384626433832795

#include <vector>
#include <iostream>
#include "player.h"

class Circulo{
	public:

		Circulo();

		void desenhaCirculo();

		float raio;

		float centroX,centroY;
		
		std::string cor;		
};

//void desenhaCirculo();
void fillColor(std::string color);
float dist2Points(float centroX, float centroY, float x, float y);
bool colisaoObsMaior(Player jogador, std::vector<Circulo>& obsMaiorList);
bool colisaoObsMenor(Player jogador, std::vector<Circulo>& obsMaiorList);
bool colisaoDonutMenor(Player jogador, Circulo donutMenor);
bool colisaoDonutMaior(Player jogador, Circulo donutMaior);
#endif
