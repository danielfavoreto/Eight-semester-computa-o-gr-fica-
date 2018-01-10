#include <math.h>
#include <GL/freeglut.h>
#include <string.h>
#include "utils.hpp"

// desenha um circulo 
void Circulo::desenhaCirculo(){

	GLfloat twicePi = 2.0f * PI;
	
	glBegin(GL_TRIANGLE_FAN);
	
	fillColor(cor);
	
	glVertex3f(centroX,centroY,0.0);
	
	for (int i = 0; i <= 360; i++){
	
		glVertex3f(centroX + (raio * cos(i * twicePi/360)),centroY + (raio * sin(i * twicePi/360)),0.0);
		
	}
		
	glEnd();
}
void fillColor(std::string color){

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
	// black color
	else { 
		glColor3f(0,0,0);
	}
}

float dist2Points(float centroX, float centroY, float x, float y){

	return (sqrt((centroX - x)*(centroX - x) + (centroY - y)*(centroY - y)));
}

bool colisaoDonutMaior(Circulo jogador, Circulo donutMaior){

    float dist = dist2Points(jogador.centroX, jogador.centroY, donutMaior.centroX, donutMaior.centroY);

    float dif = donutMaior.raio - jogador.raio;

    return (dist <= dif) ? false : true;
}

bool colisaoDonutMenor(Circulo jogador, Circulo donutMenor){
	
    float dist = dist2Points(jogador.centroX, jogador.centroY, donutMenor.centroX, donutMenor.centroY);

    float soma = donutMenor.raio + jogador.raio;

    return (dist >= soma) ? false : true;
}

bool colisaoObsMaior(Circulo jogador, std::vector<Circulo>& obsMaiorList){

    bool colidiu = false;

	for (int i = 0; i < obsMaiorList.size(); ++i){
	
        float dist = dist2Points(jogador.centroX, jogador.centroY, obsMaiorList[i].centroX, obsMaiorList[i].centroY);

        float soma = obsMaiorList[i].raio + jogador.raio;

        (dist >= soma) ? : colidiu = true;
        
        if (colidiu) return colidiu;
	}
	
	return false;
}

bool colisaoObsMenor(Circulo jogador, std::vector<Circulo>& obsMenorList){

    bool colidiu = false;

	for (int i = 0; i < obsMenorList.size(); ++i){
	    
	    float dist = dist2Points(jogador.centroX, jogador.centroY, obsMenorList[i].centroX, obsMenorList[i].centroY);

        float soma = obsMenorList[i].raio + jogador.raio;

        (dist >= soma) ? : colidiu = true;
        
        if (colidiu) return colidiu;
	}
	
	return false;
}
