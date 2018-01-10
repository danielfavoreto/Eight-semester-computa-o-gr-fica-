#include "player.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

Player::Player(){

  centroX = 0;
  centroY = 0;
  raio = 0;
  difY = 1;
  angleLeg = M_PI;
  playerAngle = 0;
  playerSpeed = 0;
  shotSpeed = 0;
  armAngle = 0;
  difArmX = 25;
  difArmY = -40;
  shoulderW = 45;
  shoulderH = 10;
  taxaAumentoX = 1;
  taxaAumentoY = 1;
  red = 0.8;
  green = 0.0;
  blue = 0.0;
  increment = 0.0;
  emCima = false;
  hits = 0;
  isDead = false;
  dt = 0.0;
  t = 0.0;
  diminui = false;
  aumentando = 0;
  lastT = 0.0;
  taxaAumento = 0.0;
  puloDeCima = false;
  dtAntigo = 0.0;

}

void Player::desenhaJogador()
{

/*IMPORTANTE ALTERO EMBAIXO A DIFERENCA EM RELACAO AO CENTRO EM X*/

    difArmX = 1.25 * raio;

/* IMPORTANTE ALTERO EM CIMA A DIFERENCA EM RELACAO AO CENTRO EM X*/

    shoulderW = 2 * raio;
    shoulderH = shoulderW/4;

  // Save the current modelview matrix

  glPushMatrix();

    //Transformation
    glTranslatef(centroX, centroY, 0.0f);

    glRotatef(playerAngle,0,0,1);

    glScalef(taxaAumentoX, taxaAumentoY, 0);

    // Draw the shoulders
    desenhaOmbro(shoulderW,shoulderH,red,green,blue);

    // Draw the BLACK legs
    desenhaPerna(0,0,0);

    // Draw the arm
    desenhaBraco(difArmX,0,red,green,blue);

    // Draw the head
    desenhaCirculo(raio,red,green,blue);

  // Restore the saved modelview matrix
  glPopMatrix();

}

void Player::desenhaPerna(GLfloat red, GLfloat blue, GLfloat green){

  glPushMatrix();

    glTranslatef(- raio/2,0,0.0f);

    glScalef(1.0f,difY,1.0f);

    drawRect(10, 2 * raio,red,blue,green);

  glPopMatrix();

  glPushMatrix();

    glTranslatef(raio/2,0,0.0f);

    glScalef(1.0f,-difY,1.0f);

    drawRect(10, 2 * raio,red,blue,green);

  glPopMatrix();

}

void Player::desenhaBraco(float x, float y, GLfloat red, GLfloat blue, GLfloat green){

/*IMPORTANTE ALTERO EMBAIXO A DIFERENCA EM RELACAO AO CENTRO EM Y*/

    difArmY = - 2 * raio;

/* IMPORTANTE ALTERO EM CIMA A DIFERENCA EM RELACAO AO CENTRO EM Y*/

    glPushMatrix();

      glTranslatef(x,y,0.0f);

      glRotatef(armAngle, 0,0,1);

      drawRect(10.0,difArmY,red,blue,green);

    glPopMatrix();

}

void Player::anda(float distance){

  float aux = M_PI/2 + playerAngle * M_PI/180;

  centroY += distance * sin(aux);
  centroX += distance * cos(aux);

}

void Player::moveBraco(float angle){

  armAngle += angle;

  if (armAngle > 45 || armAngle < -45){

    armAngle -= angle;
  }

}

void Player::desenhaElipse(float radiusX, float radiusY)
{
   int i;

   glBegin(GL_TRIANGLE_FAN);

   float Deg2Grad = M_PI/180;

   for (i = 0; i < 360; i++)
   {

      float rad = i * Deg2Grad;

      glVertex2f(cos(rad)*radiusX, sin(rad)*radiusY);
   }

   glEnd();
}

void Player::desenhaOmbro(float raioX, float raioY, GLfloat red, GLfloat blue, GLfloat green)
{

  glPushMatrix();

    glTranslatef(0.0,0.0,0.0f);

    glColor3f(red,blue,green);

    desenhaElipse(raioX, raioY);

  glPopMatrix();

}

void Player::desenhaCirculo(int r, GLfloat red, GLfloat blue, GLfloat green)
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

void Player::viraJogador(float angle){

    playerAngle += angle;
}

// Draw rectangle
void Player::drawRect(GLint w, GLint h, GLfloat red, GLfloat blue, GLfloat green)
{

  glColor3f(red,blue,green);

    glBegin(GL_QUADS);
     glVertex3f(- (w/2),0,0.0);
     glVertex3f((w/2),0,0.0);
     glVertex3f((w/2),h,0.0);
     glVertex3f(-(w/2), h,0.0);
    glEnd();
}

float Player::dist2Points(float centroX, float centroY, float x, float y){

    return (sqrt((centroX - x)*(centroX - x) + (centroY - y)*(centroY - y)));
}

bool Player::colisaoBalaNoInimigo(std::vector<Player>& listaInimigos, std::list<Bullet>::iterator bala){

    bool colidiu = false;

  for (std::vector<Player>::iterator itInimigos = listaInimigos.begin(); itInimigos != listaInimigos.end();){
      
      float dist = dist2Points(bala->referenceCircle.centroX, bala->referenceCircle.centroY, itInimigos->centroX, itInimigos->centroY);

        float soma = itInimigos->raio*2 + bala->referenceCircle.raio;

        (dist >= soma) ? : colidiu = true;
        
        if (colidiu && !itInimigos->emCima) {

          listaInimigos.erase(itInimigos);

          return colidiu;
        }
        else {

          ++itInimigos;
        }
  }
  
  return false;
}

bool Player::colisaoBalaNoJogador(std::list<Bullet>::iterator bala){

    bool colidiu = false;

    float dist = dist2Points(centroX, centroY, bala->referenceCircle.centroX, bala->referenceCircle.centroY);

    // raio do jogador tem que ser multiplicado por 2 pq eh a metade do raio do circulo de referencia
    float soma = bala->referenceCircle.raio + raio*2;

    (dist >= soma) ? : colidiu = true;

    if (colidiu && !emCima) return colidiu;

    return false;

}