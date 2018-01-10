#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include <GL/glu.h>

class Player
{
public:

	// Class constructor
  Player();

  // Draw a player
  void desenhaJogador();

  // Translate the player in x axis
  void translateX(GLfloat dx);

  // Translate the player in y axis
  void translateY(GLfloat dy);

  // Draw the arm
  void drawArm(int x, int y, GLfloat gTheta1, GLfloat gTheta2, GLfloat gTheta3);

  // Draw a leg
  void drawLeg();

  void desenhaBraco(float x, float y, GLfloat red, GLfloat blue, GLfloat green);

  void desenhaPerna(GLfloat red, GLfloat blue, GLfloat green);

  // Draw a shoulder
  void drawShoulder();

  // Shot a bullet
  void shot();

  void moveBraco(float angle);

  // Draw the head
  void drawHead(float radius, GLfloat red,GLfloat green,GLfloat blue);

  // Rotate player
  void rotatePlayer();

  void desenhaElipse(float radiusX, float radiusY);

  void desenhaOmbro(float raioX, float raioY, GLfloat red, GLfloat blue, GLfloat green);

  // Fill color of player
  void fillColor();

  void anda(float distance);

  void viraJogador(float angle);

  // Current robot's position at his own Coordinate System
  GLfloat centroX,centroY;

  float armAngle;

  // RGB colors to fill the Robot
  GLfloat base_collorR,base_collorG, base_collorB;

  float angleLeg;

  float difY;

  float shotSpeed;

  float playerSpeed;

  float playerAngle;

  float difArmX;

  float difArmY;

  float shoulderW;

  float shoulderH;

  float taxaAumentoX;

  float taxaAumentoY;

  // ?
  GLfloat direction;

  // Player's head radius
  GLfloat raio;

  // ?
  GLfloat gTheta1,gTheta2,gTheta3,gThetaWheel;

  // Draw a circle
  void desenhaCirculo(int r, GLfloat red, GLfloat blue, GLfloat green);

  // Draw a rectangle
  void drawRect(GLint w, GLint h, GLfloat red,GLfloat blue,GLfloat green);

  // Update Robot's position at ? Coordinate System
  void updatePosition();

};

#endif