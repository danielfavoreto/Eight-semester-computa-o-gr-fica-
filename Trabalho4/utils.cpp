#include <math.h>
#include <GL/freeglut.h>
#include <string.h>
#include "utils.h"

float dist2Points(float centroX, float centroY, float x, float y){

    return (sqrt((centroX - x)*(centroX - x) + (centroY - y)*(centroY - y)));
}

bool colisaoDonutMaior(Player jogador, Circulo donutMaior){

    float dist = dist2Points(jogador.centroX, jogador.centroY, donutMaior.centroX, donutMaior.centroY);

	// raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
    float dif = donutMaior.raio - jogador.raio*2;

    return (dist <= dif) ? false : true;
}

bool colisaoDonutMenor(Player jogador, Circulo donutMenor){

    float dist = dist2Points(jogador.centroX, jogador.centroY, donutMenor.centroX, donutMenor.centroY);

	// raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
    float soma = donutMenor.raio + jogador.raio*2;

    return (dist >= soma) ? false : true;
}

bool colisaoEntreInimigos(Player inimigo, std::vector<Player>& listaInimigos){

  bool colidiu = false;

	for (std::vector<Player>::iterator it = listaInimigos.begin(); it != listaInimigos.end(); ++it){

        if (it->centroX == inimigo.centroX && it->centroY == inimigo.centroY){

            continue;
        }
        else {

            float dist = dist2Points(inimigo.centroX, inimigo.centroY, it->centroX, it->centroY);

    		// raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
            float soma = it->raio*2 + inimigo.raio*2;

            (dist >= soma) ? : colidiu = true;

            if (colidiu) return colidiu;

        }
	}

	return false;
}

bool colisaoObsMenor(Player jogador, std::vector<Circulo>& obsMenorList){

    bool colidiu = false;

	for (int i = 0; i < obsMenorList.size(); ++i){

	    float dist = dist2Points(jogador.centroX, jogador.centroY, obsMenorList[i].centroX, obsMenorList[i].centroY);

		// raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
        float soma = obsMenorList[i].raio + jogador.raio*2;

        (dist >= soma) ? : colidiu = true;

        if (colidiu) return colidiu;
	}

	return false;
}

bool colisaoJogadorInimigos(Player jogador, std::vector<Player>& listaInimigos){

    bool colidiu = false;

    for (int i = 0; i < listaInimigos.size(); ++i){

        float dist = dist2Points(jogador.centroX, jogador.centroY, listaInimigos[i].centroX, listaInimigos[i].centroY);

        // raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
        float soma = listaInimigos[i].raio*2 + jogador.raio*2;

        (dist >= soma) ? : colidiu = true;

        if (colidiu) return colidiu;
    }

    return false;

}