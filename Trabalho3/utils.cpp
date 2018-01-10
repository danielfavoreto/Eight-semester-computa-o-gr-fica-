#include <math.h>
#include <GL/freeglut.h>
#include <string.h>
#include "utils.hpp"

Circulo::Circulo(){

	raio = 0;
	centroX = 0;
	centroY = 0;
	cor = "blue";
}

void Circulo::desenhaCirculo(){

	GLfloat twicePi = 2.0f * PI;

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
	else { // black color

		glColor3f(0,0,0);
	}
}

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

bool colisaoObsMaior(Player jogador, std::vector<Circulo>& obsMaiorList){

    bool colidiu = false;

	for (int i = 0; i < obsMaiorList.size(); ++i){
	
        float dist = dist2Points(jogador.centroX, jogador.centroY, obsMaiorList[i].centroX, obsMaiorList[i].centroY);

		// raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
        float soma = obsMaiorList[i].raio + jogador.raio*2; 

        (dist >= soma) ? : colidiu = true;
        
        if (colidiu) return colidiu;
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
