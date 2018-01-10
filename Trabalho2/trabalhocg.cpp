#include <GL/freeglut.h>
#include "tinyxml2.h"
#include "utils.hpp"
#include <time.h>
#include <math.h>
// -----------------------------Trabalho 2 CG -------------------------- Prof TOD Santos ---------------------------- Aluno: Daniel Favoreto

bool keyFlags[256];
Circulo donutMaior;
Circulo donutMenor;
Circulo jogador;
std::vector<Circulo> obsMaiorList;
std::vector<Circulo> obsMenorList;

// parametros da janela
int largWindow, altWindow;

float raioOriginal;

float raio50PorCento;

float lastT;

bool emCima = false;

int aumentando = 0;
clock_t begin;

float difX = 0.0;
float difY = 0.0;

float raioAux = 0.0;

float raioMinimo = 5.0;

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

		Circulo* circle = (Circulo*) malloc(sizeof *circle);

		circle->cor = elemento->FindAttribute("fill")->Value();

		elemento->QueryFloatAttribute("cx", &circle->centroX);

		elemento->QueryFloatAttribute("cy", &circle->centroY);

		elemento->QueryFloatAttribute("r", &circle->raio);

		if (0 == strcmp(circle->cor.c_str(), "blue")){

			donutMaior = *circle;
		}
		else if (0 == strcmp(circle->cor.c_str(), "white")){

			donutMenor = *circle;
		} 
		else if (0 == strcmp(circle->cor.c_str(), "green")) {

			jogador = *circle;
			raioOriginal = jogador.raio;
			raio50PorCento = jogador.raio/2.0;
		}
		else if (0 == strcmp(circle->cor.c_str(), "red")) {
			
			obsMaiorList.push_back(*circle);			
		}
		else if (0 == strcmp(circle->cor.c_str(), "black")) {

			obsMenorList.push_back(*circle);
		}
		free (circle);
	}
}

// faz a busca do arquivo svg e retorna um doc
void criaDocSVG(tinyxml2::XMLDocument* doc){

	tinyxml2::XMLNode* svg = doc->FirstChild()->FirstChild(); // svg

	// leitura config svg
	tinyxml2::XMLElement* elemento = svg->ToElement();

	std::string nomeArquivo = elemento->FindAttribute("nome")->Value();

	std::string tipoArquivo = elemento->FindAttribute("tipo")->Value();

	std::string caminhoArquivo = elemento->FindAttribute("caminho")->Value();

	// fim leitura config svg

	std::string caminhoCompletoSVG = caminhoArquivo + nomeArquivo + "." + tipoArquivo;

    tinyxml2::XMLDocument* docSVG = new tinyxml2::XMLDocument();

    if (docSVG->LoadFile(caminhoCompletoSVG.c_str())){
	    printf("Erro de parser do arquivo arena.svg\n");
	    exit(1);
    }

    tinyxml2::XMLElement* rootSVG = docSVG->FirstChildElement("svg");
    
    setaSVG(rootSVG);
}

void aumentaCirculo(){

    float t = glutGet(GLUT_ELAPSED_TIME);
    
    float dt = (t - lastT) * 0.001; // segundos
    
    if (dt <= 1.0){

        jogador.raio = raioOriginal * ((dt * 0.5) + 1);
        
    }
    else if (dt > 1.0f && dt < 2.0f){

        jogador.raio = raioOriginal * (((2-dt) * 0.5) + 1);        
    }
    else {
        
        jogador.raio = raioOriginal;
        aumentando = 0;
        keyFlags['p'] = false;
        
		if (colisaoObsMenor(jogador, obsMenorList)){
		    emCima = true;
		}
    }
}

void pulaJogador(){

    if (aumentando){
    
        aumentaCirculo();
    }
}

void idle(){
	
	if (keyFlags['w']){
	
		jogador.centroY = jogador.centroY - 1;
		
		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){
		    emCima = false;
		}
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
			jogador.centroY = jogador.centroY + 1;
		}
	}
	if (keyFlags['s']){
	
		jogador.centroY = jogador.centroY + 1;
		
		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){
		    emCima = false;
		}		
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
			jogador.centroY = jogador.centroY - 1;
		}
	}
	if (keyFlags['d']){
	
		jogador.centroX = jogador.centroX + 1;
		
		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){
		    emCima = false;
		}		
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
		jogador.centroX = jogador.centroX - 1;
		
		}
	}
	if (keyFlags['a']){
	
		jogador.centroX = jogador.centroX - 1;

		if (!colisaoObsMenor(jogador, obsMenorList) && emCima){
		    emCima = false;
		}		
		
		if (colisaoDonutMaior(jogador, donutMaior) || colisaoDonutMenor(jogador, donutMenor) || colisaoObsMaior(jogador, obsMaiorList) || (colisaoObsMenor(jogador, obsMenorList) && !aumentando && !emCima)){
		
		jogador.centroX = jogador.centroX + 1;
		
		}
	}
	if (keyFlags['p']){
	    
	    if (!aumentando){
	    
	        aumentando = 1;    
            
            lastT = glutGet(GLUT_ELAPSED_TIME);
	    }
	}

	pulaJogador();
	
	glutPostRedisplay();
}


void display(){

	glClear(GL_COLOR_BUFFER_BIT);

	donutMaior.desenhaCirculo();

	donutMenor.desenhaCirculo();

	for (int i = 0; i < obsMaiorList.size(); i++){
		obsMaiorList[i].desenhaCirculo();
	}
    for (int i = 0; i < obsMenorList.size(); i++){
        obsMenorList[i].desenhaCirculo();
    }

	jogador.desenhaCirculo();

	glutSwapBuffers();
}

// inicializa o rgb, camera, etc.
void inicializaGlut(int& largWindow, int& altWindow){

	glClearColor(1,1,1,0); 
	glMatrixMode(GL_PROJECTION);

	// posiciona a camera com os respectivos min e max (min x, max x, min y, max y, min z, max z)
	glOrtho(donutMaior.centroX - donutMaior.raio,donutMaior.centroX + donutMaior.raio,
    	    donutMaior.centroY + donutMaior.raio,donutMaior.centroY - donutMaior.raio,
    	    -1.0,1.0);
    	    
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
	glutCreateWindow("Trabalho 2 CG"); 

	inicializaGlut(largWindow, altWindow);

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutKeyboardFunc(keyBoardPress);
	glutKeyboardUpFunc(keyBoardUp);

	glutMainLoop();      
     
	return 0;
}
