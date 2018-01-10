#include <GL/freeglut.h>
#include "tinyxml2.h"
#include "utils.hpp"
#include "player.h"
#include "bullet.h"
#include <time.h>
#include <math.h>
#include <list>
// -----------------------------Trabalho 3 CG -------------------------- Prof TOD Santos ---------------------------- Aluno: Daniel Favoreto

bool keyFlags[256];
Circulo donutMaior;
Circulo donutMenor;
Player jogador;
std::vector<Circulo> obsMaiorList;
std::vector<Circulo> obsMenorList;
std::list<Bullet> balaList;

// parametros da janela
int largWindow, altWindow;

float raioOriginal;

float raio50PorCento;

float lastT;

bool emCima = false;

int aumentando = 0;

clock_t begin;

float raioAux = 0.0;

float raioMinimo = 5.0;

float taxaCurva = 0.5;

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

			jogador.centroX = circle.centroX;
			jogador.centroY = circle.centroY;
			jogador.raio = circle.raio/2.0;

			raioOriginal = jogador.raio/2.0;
			raio50PorCento = jogador.raio/2.0;
		}
		else if (0 == strcmp(circle.cor.c_str(), "red")) {
			
			obsMaiorList.push_back(circle);			
		}
		else if (0 == strcmp(circle.cor.c_str(), "black")) {

			obsMenorList.push_back(circle);
		}
	}
}

// faz a busca do arquivo svg e retorna um doc
void criaDocSVG(tinyxml2::XMLDocument* doc){

	tinyxml2::XMLNode* svg = doc->FirstChild()->FirstChild(); // svg

	tinyxml2::XMLNode* nodeJogador = doc->FirstChild()->LastChild(); // jogador

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

	std::string caminhoCompletoSVG = caminhoArquivo + nomeArquivo + "." + tipoArquivo;

    tinyxml2::XMLDocument* docSVG = new tinyxml2::XMLDocument();

    if (docSVG->LoadFile(caminhoCompletoSVG.c_str())){

	    printf("Erro de parser do arquivo arena.svg\n");
	    exit(1);
    }

    tinyxml2::XMLElement* rootSVG = docSVG->FirstChildElement("svg");
    
    setaSVG(rootSVG);
}

void aumentaJogador(){

    float t = glutGet(GLUT_ELAPSED_TIME);
    
    float dt = (t - lastT) * 0.001; // segundos
    
    if (dt <= 1.0){

        jogador.taxaAumentoX = ((dt * 0.5) + 1);
        jogador.taxaAumentoY = ((dt * 0.5) + 1);

    }
    else if (dt > 1.0f && dt < 2.0f){

        jogador.taxaAumentoX = (((2-dt) * 0.5) + 1);
        jogador.taxaAumentoY = (((2-dt) * 0.5) + 1);

    }
    else {
        
        aumentando = 0;
        keyFlags['p'] = false;
        
		if (colisaoObsMenor(jogador, obsMenorList)){
		  
		    emCima = true;
		}
    }
}

void pulaJogador(){

    if (aumentando){
    
        aumentaJogador();
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

    GLdouble currentTime;

    GLdouble timeDiference;

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	timeDiference = currentTime - previousTime;
			
	previousTime = currentTime;

    static float increment = 0;

	if (keyFlags['w']){
	
		if (keyFlags['d']){
	
			jogador.viraJogador(taxaCurva * jogador.playerSpeed * timeDiference);

		}
		if (keyFlags['a']){
	
			jogador.viraJogador(-taxaCurva * jogador.playerSpeed * timeDiference);

		}

		jogador.anda(-timeDiference*jogador.playerSpeed);

		increment += 0.05;

		jogador.difY = cos(increment);
		
		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){

		    emCima = false;
		}
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
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

		increment -= 0.05;

		jogador.difY = cos(increment);
		
		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){

		    emCima = false;
		}		
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
			jogador.anda(-timeDiference*jogador.playerSpeed);

		}
	}
	if (keyFlags['d']){

		// jogador deve somente girar em torno do proprio eixo
		if (!keyFlags['w'] && !keyFlags['s']){

			jogador.viraJogador(jogador.playerSpeed*timeDiference);
			
			if (!colisaoObsMenor(jogador, obsMenorList) && emCima){

			    emCima = false;
			}		
			
			if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
			
				jogador.centroX = jogador.centroX - 1;
			
			}
		}
	}
	if (keyFlags['a']){
	
		// jogador deve somente girar em torno do proprio eixo
		if (!keyFlags['w'] && !keyFlags['s']){

			jogador.viraJogador(-jogador.playerSpeed*timeDiference);

			if (!colisaoObsMenor(jogador, obsMenorList) && emCima){

			    emCima = false;
			}		
			
			if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
			
				jogador.centroX = jogador.centroX + 1;
			
			}
		}
	}
	if (keyFlags['p']){
	    
	    if (!aumentando){
	    
	        aumentando = 1;    
            
            lastT = glutGet(GLUT_ELAPSED_TIME);
	    }
	}

	pulaJogador();

	balaList.remove_if(isOutsideScreen);

	for (std::list<Bullet>::iterator it = balaList.begin(); it != balaList.end();){

		it->updatePosition(timeDiference);

		if (it->colisaoBalaObs(obsMenorList)){

			it = balaList.erase(it);
		}
		else {

			++it;
		}
	}
	
	glutPostRedisplay();
}

void display(){

	glClearColor(1,1,1,0);

	glClear(GL_COLOR_BUFFER_BIT);

	donutMaior.desenhaCirculo();

	donutMenor.desenhaCirculo();

	for (int i = 0; i < obsMaiorList.size(); i++){
		
		obsMaiorList[i].desenhaCirculo();
	}
    for (int i = 0; i < obsMenorList.size(); i++){
        
        obsMenorList[i].desenhaCirculo();
    }

	jogador.desenhaJogador();

	for (std::list<Bullet>::iterator it = balaList.begin(); it != balaList.end(); ++it){
		
		it->draw();
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
  	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !aumentando && !emCima){

		float topoArmaX = jogador.centroX + sin((jogador.playerAngle + 90)*  M_PI/180) * jogador.difArmX + cos((jogador.playerAngle + jogador.armAngle + 90) *  M_PI/180) * jogador.difArmY;
		
		float topoArmaY = jogador.centroY - cos((jogador.playerAngle + 90)*  M_PI/180) * jogador.difArmX + sin((jogador.playerAngle + jogador.armAngle + 90) * M_PI/180) * jogador.difArmY;

		Bullet* bala = new Bullet(jogador, topoArmaX, topoArmaY);

		bala->draw();

		balaList.push_back(*bala);

		delete bala;

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
	glutCreateWindow("Trabalho 3 CG"); 

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
