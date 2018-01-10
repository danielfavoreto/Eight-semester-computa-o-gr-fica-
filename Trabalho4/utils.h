#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <vector>
#include <iostream>
#include "player.h"
#include "circle.h"

float dist2Points(float centroX, float centroY, float x, float y);
bool colisaoEntreInimigos(Player inimigo, std::vector<Player>& listaInimigos);
bool colisaoJogadorInimigos(Player jogador, std::vector<Player>& listaInimigos);
bool colisaoObsMenor(Player jogador, std::vector<Circulo>& obsMaiorList);
bool colisaoDonutMenor(Player jogador, Circulo donutMenor);
bool colisaoDonutMaior(Player jogador, Circulo donutMaior);

#endif
