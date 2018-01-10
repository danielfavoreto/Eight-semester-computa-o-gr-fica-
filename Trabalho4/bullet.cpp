#include "bullet.h"   

Bullet::Bullet(){

	speed = 0;

	playerAngle = 0;

	armAngle = 0;

	referenceCircle.raio = 0;

	referenceCircle.cor = "black";

	referenceCircle.centroX = 0;

	referenceCircle.centroY = 0;

}

void Bullet::desenhaCirculo(int r, GLfloat red, GLfloat blue, GLfloat green)
{

  GLfloat twicePi = 2.0f * M_PI;
  
  glBegin(GL_TRIANGLE_FAN);
  
  glColor3f(red,blue,green);
  
  glVertex3d(0.0,0.0,0.0);
  
  for (int i = 0; i <= 360; i++){
  
    glVertex3f(r * cos(i * twicePi/360),r * sin(i * twicePi/360),0.0);
    
  }
    
  glEnd();

}


void Bullet::updatePosition(float timeDiff){

	referenceCircle.centroX += timeDiff * speed * sin((playerAngle + armAngle) * M_PI/180);

	referenceCircle.centroY += (-1) * timeDiff * speed * cos((playerAngle + armAngle) * M_PI/180);

}

void Bullet::draw(){
	
	glPushMatrix();

		glTranslatef(referenceCircle.centroX,referenceCircle.centroY,0);

		desenhaCirculo(referenceCircle.raio, 1,0.85,0.0);

	glPopMatrix();

}

bool Bullet::colisaoBalaObs(std::vector<Circulo>& obsMenorList){

    bool colidiu = false;

	for (int i = 0; i < obsMenorList.size(); ++i){
	    
	    float dist = obsMenorList[i].dist2Points(referenceCircle.centroX, referenceCircle.centroY, obsMenorList[i].centroX, obsMenorList[i].centroY);

        float soma = obsMenorList[i].raio + referenceCircle.raio;

        (dist >= soma) ? : colidiu = true;
        
        if (colidiu) return colidiu;
	}
	
	return false;
}