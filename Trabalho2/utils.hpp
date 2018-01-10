#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#define PI 3.1415926535897932384626433832795

#include <vector>
#include <iostream>

struct Circulo{
	float raio;
	float centroX,centroY;
	std::string cor;
	void desenhaCirculo();
};

void desenhaCirculo();
void fillColor(std::string color);
float dist2Points(float centroX, float centroY, float x, float y);
bool colisaoObsMaior(Circulo jogador, std::vector<Circulo>& obsMaiorList);
bool colisaoObsMenor(Circulo jogador, std::vector<Circulo>& obsMaiorList);
bool colisaoDonutMenor(Circulo jogador, Circulo donutMenor);
bool colisaoDonutMaior(Circulo jogador, Circulo donutMaior);
#endif
