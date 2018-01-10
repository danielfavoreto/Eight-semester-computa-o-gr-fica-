#include <GL/freeglut.h>
#include "tinyxml2.h"
#include "utils.h"
#include "player.h"
#include "bullet.h"
#include "circle.h"
#include <time.h>
#include <math.h>
#include <list>
// -----------------------------Trabalho 4 CG -------------------------- Prof TOD Santos ---------------------------- Aluno: Daniel Favoreto

bool keyFlags[256];
Circulo donutMaior;
Circulo donutMenor;
Player jogador;
std::vector<Circulo> obsMaiorList;
std::vector<Circulo> obsMenorList;
std::vector<Player> inimigosList;

// parametros da janela
int largWindow, altWindow;

int maxScore = 0;

static char str[2000];

bool winGame = false;

int directionArm = 1;

float raioOriginal;

float raio50PorCento;

clock_t begin;

float raioAux = 0.0;

float raioMinimo = 5.0;

float taxaCurva = 0.5;

float freqTiroInimigo = 0.0;

float velTiroInimigo = 0.0;

float velInimigo = 0.0;

float alturaObs = 0;

// busca o arquivo xml e retorna o doc
tinyxml2::XMLDocument* criaDoc(int argc, std::string argv){

	if (argc != 2){

		std::cout << "Argumentos invalidos\n";

		return NULL;
	}

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	std::string caminho = argv + "config.xml";

	const char* pFileName = NULL;

	pFileName = caminho.data();

	if (doc->LoadFile(pFileName) == tinyxml2::XML_ERROR_FILE_NOT_FOUND){

		std::cout << "Erro de parser do arquivo config.xml\n";
		return NULL;

	}
	else {

		return doc;
	}
}

// faz a leitura do arquivo svg
void setaSVG(tinyxml2::XMLElement* ElementSVG){

    // primeiro circulo
    tinyxml2::XMLElement* elemento = ElementSVG->FirstChildElement("circle");

	for (;elemento != NULL ; elemento = elemento->NextSiblingElement("circle")){

		Circulo circle;

		circle.cor = elemento->Attribute("fill");

		elemento->QueryFloatAttribute("cx", &circle.centroX);

		elemento->QueryFloatAttribute("cy", &circle.centroY);

		elemento->QueryFloatAttribute("r", &circle.raio);

		if (0 == strcmp(circle.cor.c_str(), "blue")){

			donutMaior = circle;
		}
		else if (0 == strcmp(circle.cor.c_str(), "white")){

			donutMenor = circle;
		}
		else if (0 == strcmp(circle.cor.c_str(), "green")) {

			jogador.red = 0.0;
			jogador.green = 0.8;
			jogador.blue = 0.0;

			jogador.centroX = circle.centroX;
			jogador.centroY = circle.centroY;
			jogador.raio = circle.raio/2.0;

			raioOriginal = jogador.raio/2.0;
			raio50PorCento = jogador.raio/2.0;
		}
		else if (0 == strcmp(circle.cor.c_str(), "red")) {

			Player inimigo;

			inimigo.centroX = circle.centroX;
			inimigo.centroY = circle.centroY;
			inimigo.raio = circle.raio/2.0;

			inimigo.playerSpeed = velInimigo;
			inimigo.shotSpeed = velTiroInimigo;

			inimigosList.push_back(inimigo);

			maxScore++;
		}
		else if (0 == strcmp(circle.cor.c_str(), "black")) {

			obsMenorList.push_back(circle);
		}
	}
}

// faz a busca do arquivo svg e retorna um doc
void criaDocSVG(tinyxml2::XMLDocument* doc){

	tinyxml2::XMLNode* svg = doc->FirstChild()->FirstChild(); // svg

	tinyxml2::XMLNode* nodeJogador = svg->NextSiblingElement(); // jogador

	tinyxml2::XMLNode* nodeInimigo = nodeJogador->NextSiblingElement(); // inimigo

	tinyxml2::XMLNode* nodeObs = nodeInimigo->NextSiblingElement(); // obstaculo

	// leitura arquivoArena svg
	tinyxml2::XMLElement* elemento = svg->ToElement();

	std::string nomeArquivo = elemento->FindAttribute("nome")->Value();

	std::string tipoArquivo = elemento->FindAttribute("tipo")->Value();

	std::string caminhoArquivo = elemento->FindAttribute("caminho")->Value();

	// fim leitura arquivoArena svg

	// leitura jogador

	tinyxml2::XMLElement* elementoJogador = nodeJogador->ToElement();

	elementoJogador->QueryFloatAttribute("velTiro", &jogador.shotSpeed);

	elementoJogador->QueryFloatAttribute("vel", &jogador.playerSpeed);

	// fim leitura jogador

	// leitura inimigo

	tinyxml2::XMLElement* elementoInimigo = nodeInimigo->ToElement();

	elementoInimigo->QueryFloatAttribute("freqTiro", &freqTiroInimigo);

	elementoInimigo->QueryFloatAttribute("velTiro", &velTiroInimigo);

	elementoInimigo->QueryFloatAttribute("vel", &velInimigo);

	// fim leitura inimigo

	// leitura obstaculo

	tinyxml2::XMLElement* elementoObs = nodeObs->ToElement();

	elementoObs->QueryFloatAttribute("altura", &alturaObs);

	// fim leitura obstaculo

	std::string caminhoCompletoSVG = caminhoArquivo + nomeArquivo + "." + tipoArquivo;

    tinyxml2::XMLDocument* docSVG = new tinyxml2::XMLDocument();

    if (docSVG->LoadFile(caminhoCompletoSVG.c_str())){

	    printf("Erro de parser do arquivo arena.svg\n");
	    exit(1);
    }

    tinyxml2::XMLElement* rootSVG = docSVG->FirstChildElement("svg");

    setaSVG(rootSVG);
}

void aumentaJogador(Player &jogador){

    jogador.t = glutGet(GLUT_ELAPSED_TIME);

    jogador.dt = (jogador.t - jogador.lastT) * 0.001; // segundos

    if (jogador.dt <= 1.0){

    	if (jogador.dt >= (alturaObs/100) && colisaoObsMenor(jogador,obsMenorList) && !jogador.emCima){

			jogador.emCima = true;
			jogador.dt = 2 - jogador.dt;
			jogador.dtAntigo = jogador.dt;

    	}
    	else if (jogador.puloDeCima){

        	jogador.taxaAumentoX = (((jogador.dt * 0.5) + 1) * jogador.taxaAumento);
        	jogador.taxaAumentoY = (((jogador.dt * 0.5) + 1) * jogador.taxaAumento);

    	}
    	else {

        	jogador.taxaAumentoX = ((jogador.dt * 0.5) + 1);
        	jogador.taxaAumentoY = ((jogador.dt * 0.5) + 1);
        	jogador.taxaAumento = jogador.taxaAumentoX;

    	}
    }
    else if (jogador.dt > 1.0f && jogador.dt < 2.0f){

    	if (jogador.dt <= (2 - alturaObs/100) && colisaoObsMenor(jogador, obsMenorList) && !jogador.emCima){

			jogador.emCima = true;
			jogador.dtAntigo = jogador.dt;

    	}
    	else if (jogador.puloDeCima){

        	jogador.taxaAumentoX = ((((2-jogador.dt) * 0.5) + 1) * jogador.taxaAumento);
        	jogador.taxaAumentoY = ((((2-jogador.dt) * 0.5) + 1) * jogador.taxaAumento);

    	}
		else if (jogador.emCima){

			if (jogador.taxaAumentoX > ((alturaObs/100) * 0.5) + 1){

        		jogador.taxaAumentoX = (((2-jogador.dt) * 0.5) + 1);
        		jogador.taxaAumentoY = (((2-jogador.dt) * 0.5) + 1);
        		jogador.taxaAumento = jogador.taxaAumentoX;
			}
			else {

				jogador.aumentando = 0;
				keyFlags['p'] = false;
			}
      	}
		else {

        	jogador.taxaAumentoX = (((2-jogador.dt) * 0.5) + 1);
        	jogador.taxaAumentoY = (((2-jogador.dt) * 0.5) + 1);
        	jogador.taxaAumento = jogador.taxaAumentoX;

        }
    }
    else if (jogador.puloDeCima && jogador.emCima){

        jogador.aumentando = 0;
        keyFlags['p'] = false;
        jogador.puloDeCima = false; 

    }
    else {

        jogador.aumentando = 0;
        keyFlags['p'] = false;
        jogador.emCima = false;
        jogador.puloDeCima = false; 

    }
}

void diminuiJogador(Player &jogador){

	if (!colisaoObsMenor(jogador, obsMenorList) && jogador.emCima && !jogador.aumentando){

	    if (jogador.dt <= 1.0f){
	    	jogador.dt = 2 - jogador.dt;
	    }
    
		jogador.diminui = true;
		jogador.t = glutGet(GLUT_ELAPSED_TIME);
	
		jogador.dt = 4 - (2 * jogador.taxaAumento);
		jogador.lastT = jogador.t - (jogador.dt / 0.001);				

 	   	jogador.emCima = false;
		
	}

	if (jogador.diminui ){

    	jogador.t = glutGet(GLUT_ELAPSED_TIME);

    	jogador.dt = (jogador.t - jogador.lastT) * 0.001; // segundos

		if (jogador.dt > 1.0f && jogador.dt < 2.0f){

        	jogador.taxaAumentoX = (((2-jogador.dt) * 0.5) + 1);
        	jogador.taxaAumentoY = (((2-jogador.dt) * 0.5) + 1);
		}
		else {

			jogador.diminui = false;
		}
	}

}

void pulaJogador(Player &jogador){

    if (jogador.aumentando){

        aumentaJogador(jogador);
    }
}

void gameScore(GLfloat x, GLfloat y){

    char *scoreString;

	glColor3d(0.0,0.0,0.0);

	sprintf(str, "Score = %d", jogador.hits);

    glRasterPos2f(x, y);

    scoreString = str;

    while (*scoreString){

        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *scoreString);

        scoreString++;

    }
}

bool isOutsideScreen(Bullet& b){

	if (b.referenceCircle.centroX < donutMaior.centroX - donutMaior.raio || b.referenceCircle.centroX > donutMaior.centroX + donutMaior.raio){

		return true;

	}
	else if (b.referenceCircle.centroY > donutMaior.centroY + donutMaior.raio || b.referenceCircle.centroY < donutMaior.centroY - donutMaior.raio){

		return true;
	}

	return false;
}

void idle(){
	
	static GLdouble previousTime = 0;

	static GLdouble shotTime = 0;

    GLdouble currentTime;

    GLdouble timeDiference;

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	timeDiference = currentTime - previousTime;

	previousTime = currentTime;

	if (!jogador.isDead)
	{
		if (keyFlags['w']){

			if (keyFlags['d']){

				jogador.viraJogador(taxaCurva * jogador.playerSpeed * timeDiference);

			}
			if (keyFlags['a']){

				jogador.viraJogador(-taxaCurva * jogador.playerSpeed * timeDiference);

			}

			jogador.anda(-timeDiference*jogador.playerSpeed);

			jogador.increment += 0.05;

			jogador.difY = cos(jogador.increment);

			pulaJogador(jogador);

			if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoJogadorInimigos(jogador, inimigosList) || colisaoObsMenor(jogador, obsMenorList) && !jogador.emCima ){

				jogador.anda(timeDiference*jogador.playerSpeed);

			}
		}
		if (keyFlags['s']){

			if (keyFlags['d']){

				jogador.viraJogador(-taxaCurva * jogador.playerSpeed * timeDiference);

			}
			if (keyFlags['a']){

				jogador.viraJogador(taxaCurva * jogador.playerSpeed * timeDiference);

			}

			jogador.anda(timeDiference*jogador.playerSpeed);

			jogador.increment -= 0.05;

			jogador.difY = cos(jogador.increment);

			pulaJogador(jogador);

			if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoJogadorInimigos(jogador, inimigosList) || (colisaoObsMenor(jogador, obsMenorList) && !jogador.emCima)){

				jogador.anda(-timeDiference*jogador.playerSpeed);

			}
		}
		if (keyFlags['d']){

			if (!keyFlags['w'] && !keyFlags['s']){

				jogador.viraJogador(jogador.playerSpeed*timeDiference);

				if (!colisaoObsMenor(jogador, obsMenorList) && jogador.emCima){

				    jogador.emCima = false;

				}

				if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoJogadorInimigos(jogador, inimigosList) || (colisaoObsMenor(jogador, obsMenorList) && !jogador.emCima)){

					jogador.centroX = jogador.centroX - 1;

				}
			}
		}
		if (keyFlags['a']){

			if (!keyFlags['w'] && !keyFlags['s']){

				jogador.viraJogador(-jogador.playerSpeed*timeDiference);

				if (!colisaoObsMenor(jogador, obsMenorList) && jogador.emCima){

				    jogador.emCima = false;
				}

				if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoJogadorInimigos(jogador, inimigosList) || (colisaoObsMenor(jogador, obsMenorList) && !jogador.emCima)){

					jogador.centroX = jogador.centroX + 1;

				}
			}
		}
		if (keyFlags['p']){

		    if (!jogador.aumentando){

		        jogador.aumentando = 1;

	            jogador.lastT = glutGet(GLUT_ELAPSED_TIME);

	            if (jogador.emCima){

	            	jogador.puloDeCima = true;
	            }
		    }
		}
	}
	/* Movimento dos inimigos*/

	for (int i = 0; i < inimigosList.size(); ++i){

		float distanciaAleatoria = ( i % 2 == 0) ? -timeDiference*inimigosList[i].playerSpeed : timeDiference*inimigosList[i].playerSpeed;

		inimigosList[i].moveBraco(1 * directionArm);

		if (inimigosList[i].armAngle < -44.999)
		{

			inimigosList[i].armAngle = -45;

			directionArm = 1;
		}
		else if (inimigosList[i].armAngle > 44.999)
		{

			inimigosList[i].armAngle = 45;

			directionArm = -1;
		}

		int rotacaoAleatoria = rand() % 100;

		if (rotacaoAleatoria > 1)
		{

			if (rotacaoAleatoria < 10)
			{

				inimigosList[i].viraJogador(-taxaCurva * inimigosList[i].playerSpeed * timeDiference);
			}

		}
		else
		{

			inimigosList[i].viraJogador(taxaCurva * inimigosList[i].playerSpeed * timeDiference);
		}

		//inimigosList[i].viraJogador(taxaCurva * inimigosList[i].playerSpeed * timeDiference);

		inimigosList[i].anda(distanciaAleatoria);

		inimigosList[i].increment += 0.05;

		inimigosList[i].difY = cos(inimigosList[i].increment);

		pulaJogador(inimigosList[i]);

		if (!colisaoObsMenor(inimigosList[i], obsMenorList) && inimigosList[i].emCima){

		    if (inimigosList[i].dt <= 1.0f){

		    	inimigosList[i].dt = 2 - inimigosList[i].dt;
		    }

			inimigosList[i].diminui = true;
			inimigosList[i].t = glutGet(GLUT_ELAPSED_TIME);
			inimigosList[i].lastT = inimigosList[i].t - (inimigosList[i].dt / 0.001);
		    inimigosList[i].emCima = false;

		}

		if (colisaoObsMenor(inimigosList[i], obsMenorList) && !inimigosList[i].emCima){

			if (!inimigosList[i].aumentando){

		        inimigosList[i].aumentando = 1;

	            inimigosList[i].lastT = glutGet(GLUT_ELAPSED_TIME);
		    }
		}

		if (colisaoObsMenor(inimigosList[i], obsMenorList) && !inimigosList[i].emCima || colisaoDonutMaior(inimigosList[i], donutMaior) || colisaoDonutMenor(inimigosList[i], donutMenor) || colisaoEntreInimigos(inimigosList[i], inimigosList) || colisaoJogadorInimigos(jogador, inimigosList)){

			inimigosList[i].anda(-1*distanciaAleatoria);

			inimigosList[i].viraJogador(taxaCurva * inimigosList[i].playerSpeed * timeDiference);

		}

		pulaJogador(inimigosList[i]);

		diminuiJogador(inimigosList[i]);
	}

	/* Fim movimento dos inimigos*/

	/* Tiro dos inimigos */

	for (int i = 0; i < inimigosList.size(); ++i){

		inimigosList[i].balasList.remove_if(isOutsideScreen);

		for (std::list<Bullet>::iterator it = inimigosList[i].balasList.begin(); it != inimigosList[i].balasList.end();){

			it->updatePosition(timeDiference);

			if (it->colisaoBalaObs(obsMenorList)){

				it = inimigosList[i].balasList.erase(it);
			}
			else if (jogador.colisaoBalaNoJogador(it)){

				it = inimigosList[i].balasList.erase(it);

				jogador.isDead = true;
			}
			else {

				++it;
			}
		}
	}

	if ((currentTime - shotTime)/1000 > 1.0/freqTiroInimigo){

		for (int i = 0; i < inimigosList.size(); ++i){

			if (!inimigosList[i].aumentando && !inimigosList[i].emCima){

				float topoArmaX = inimigosList[i].centroX + sin((inimigosList[i].playerAngle + 90) *  M_PI/180) * inimigosList[i].difArmX + cos((inimigosList[i].playerAngle + inimigosList[i].armAngle + 90) *  M_PI/180) * inimigosList[i].difArmY;

				float topoArmaY = inimigosList[i].centroY - cos((inimigosList[i].playerAngle + 90) *  M_PI/180) * inimigosList[i].difArmX + sin((inimigosList[i].playerAngle + inimigosList[i].armAngle + 90) * M_PI/180) * inimigosList[i].difArmY;

				Bullet bala;

				bala.speed = inimigosList[i].shotSpeed;

				bala.playerAngle = inimigosList[i].playerAngle;

				bala.armAngle = inimigosList[i].armAngle;

				bala.referenceCircle.raio = 0.15 * inimigosList[i].raio;

				bala.referenceCircle.cor = "black";

				bala.referenceCircle.centroX = topoArmaX;

				bala.referenceCircle.centroY = topoArmaY;

				bala.draw();

				inimigosList[i].balasList.push_back(bala);
			}
		}

		shotTime = currentTime;
	}

	/* Fim tiro dos inimigos */

	pulaJogador(jogador);

	diminuiJogador(jogador);

	/* Tiro do jogador*/

	jogador.balasList.remove_if(isOutsideScreen);

	for (std::list<Bullet>::iterator it = jogador.balasList.begin(); it != jogador.balasList.end(); /*++it*/){

		it->updatePosition(timeDiference);

		if (it->colisaoBalaObs(obsMenorList)){

			it = jogador.balasList.erase(it);
		}
		else if (jogador.colisaoBalaNoInimigo(inimigosList,it)){

			it = jogador.balasList.erase(it);

			jogador.hits++;

			if (jogador.hits == maxScore){

				winGame = true;
			}
		}
		else {

			++it;
		}
	}

	/* Fim tiro do jogador */

	glutPostRedisplay();
}

void display(){

	glClearColor(1,1,1,0);

	glClear(GL_COLOR_BUFFER_BIT);

	donutMaior.desenhaCirculo();

	donutMenor.desenhaCirculo();

    for (int i = 0; i < obsMenorList.size(); i++){

        obsMenorList[i].desenhaCirculo();
    }

	for (int i = 0; i < inimigosList.size(); i++){

		inimigosList[i].desenhaJogador();
	}


    if (!jogador.isDead)
    {

		jogador.desenhaJogador();
	}
	/* Desenho das balas do jogador */

	for (std::list<Bullet>::iterator it = jogador.balasList.begin(); it != jogador.balasList.end(); ++it){

		it->draw();
	}

	/* Fim desenho das balas do jogador */

	/* Desenho das balas dos inimigos */

	for (std::vector<Player>::iterator itInimigos = inimigosList.begin(); itInimigos != inimigosList.end(); ++itInimigos){

		for (std::list<Bullet>::iterator itBalas = itInimigos->balasList.begin(); itBalas != itInimigos->balasList.end(); ++itBalas){

			itBalas->draw();
		}
	}

	/* Fim desenho das balas dos inimigos */

	gameScore(donutMaior.centroX + 0.5 * donutMaior.raio, donutMaior.centroY - 0.9 * donutMaior.raio);

	if (winGame){

		char *scoreString;

		sprintf(str,"You win");

		glRasterPos2f(donutMaior.centroX, donutMaior.centroY);

		scoreString = str;

		while (*scoreString){

	        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *scoreString);

	        scoreString++;
	    }
	}
	else if (jogador.isDead){

		char *scoreString;

		sprintf(str,"You lose");

		glRasterPos2f(donutMaior.centroX, donutMaior.centroY);

		scoreString = str;

		while( *scoreString ){

	        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *scoreString);

	        scoreString++;
	    }
	}

	glutSwapBuffers();
}

// inicializa o rgb, camera, etc.
void inicializaGlut(void){

	glClearColor(0.0,0.0,0.0,0.0);

	glMatrixMode(GL_PROJECTION);

  	glLoadIdentity();

	// posiciona a camera com os respectivos min e max (min x, max x, min y, max y, min z, max z)
	glOrtho(donutMaior.centroX - donutMaior.raio,donutMaior.centroX + donutMaior.raio,
    	    donutMaior.centroY + donutMaior.raio,donutMaior.centroY - donutMaior.raio,
    	    -100.0,100.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

}

void keyBoardUp(unsigned char key, int x, int y){

	keyFlags[key] = false;

}

void keyBoardPress(unsigned char key, int x, int y){

	switch (key){
		case 's':
			keyFlags[key] = true;
			break;
		case 'w':
			keyFlags[key] = true;
			break;
		case 'd':
			keyFlags[key] = true;
			break;
		case 'a':
			keyFlags[key] = true;
			break;
		case 'p':
		    keyFlags[key] = true;
		    break;
		}
}

void mouse(int button, int state, int x, int y){

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !jogador.aumentando && !jogador.emCima && !jogador.isDead){

		float topoArmaX = jogador.centroX + sin((jogador.playerAngle + 90)*  M_PI/180) * jogador.difArmX + cos((jogador.playerAngle + jogador.armAngle + 90) *  M_PI/180) * jogador.difArmY;

		float topoArmaY = jogador.centroY - cos((jogador.playerAngle + 90)*  M_PI/180) * jogador.difArmX + sin((jogador.playerAngle + jogador.armAngle + 90) * M_PI/180) * jogador.difArmY;

		Bullet bala;

		bala.speed = jogador.shotSpeed;

		bala.playerAngle = jogador.playerAngle;

		bala.armAngle = jogador.armAngle;

		bala.referenceCircle.raio = 0.15 * jogador.raio;

		bala.referenceCircle.cor = "black";

		bala.referenceCircle.centroX = topoArmaX;

		bala.referenceCircle.centroY = topoArmaY;

		bala.draw();

		jogador.balasList.push_back(bala);
	}
}

void passiveMotion(int x, int y){

    static float positionX = jogador.centroX;

    jogador.moveBraco(x-positionX);

    positionX = x;

}

int main(int argc, char** argv) {

	if (!argv[1]){

		std::cout << "Argumentos invalidos\n";
		exit(1);
	}

	tinyxml2::XMLDocument* doc = criaDoc (argc, std::string(argv[1]));

	if (!doc){
		exit(1);
	}

	criaDocSVG(doc);

	altWindow = 2 * donutMaior.raio;
	largWindow = altWindow;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(largWindow, altWindow);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Trabalho 4 CG");

	inicializaGlut();

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutKeyboardFunc(keyBoardPress);
	glutKeyboardUpFunc(keyBoardUp);

	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passiveMotion);

	glutMainLoop();

	return 0;
}
