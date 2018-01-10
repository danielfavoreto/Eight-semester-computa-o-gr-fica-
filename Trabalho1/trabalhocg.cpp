#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "tinyXml.hpp"
#include "circle.hpp"
#include "estados.hpp"
#include "utils.hpp"

// -----------------------------Trabalho 1 CG -------------------------- Prof TOD Santos ---------------------------- Aluno: Daniel Favoreto

// busca o arquivo e retorna o doc
tinyxml::XMLDocument* criaDoc(int argc, std::string argv){
	
	if (argc != 2){

		std::cout << "Argumentos invalidos\n";

		return NULL;
	}

	tinyxml::XMLDocument* doc = new tinyxml::XMLDocument();

	std::string caminho = argv + "config.xml";

	const char* pFileName = NULL;

	pFileName = caminho.data();

	if (doc->LoadFile(pFileName) == tinyxml::XML_ERROR_FILE_NOT_FOUND){

		std::cout << "Erro de parser do arquivo\n";
		return NULL;

	}
	else {

		return doc;
	}
}

// faz a leitura e seta os parametros para a janela
void setaParametros(tinyxml::XMLDocument* doc){

	tinyxml::XMLNode* window = doc->FirstChild()->FirstChild(); // janela

	tinyxml::XMLNode* circle = doc->FirstChild()->LastChild(); // circulo

	// leitura circulo
	tinyxml::XMLElement* elemento = circle->ToElement();

	elemento->QueryFloatAttribute("corR",&circulo.corR);

	elemento->QueryFloatAttribute("corG",&circulo.corG);

	elemento->QueryFloatAttribute("corB",&circulo.corB);

	elemento->QueryIntAttribute("raio",&circulo.raio);
	// fim leitura circulo

	// seta um raio minimo
	if (circulo.raio >= 0.0 && circulo.raio < 5.0){

		raioMinimo = circulo.raio;
	}

	// percorre pelos elementos do arquivo
	for (tinyxml::XMLNode* node = window->FirstChild(); node != NULL ; node = node->NextSibling()){

		tinyxml::XMLElement* elem = node->ToElement();

		if (0 == strcmp(node->Value(), "titulo")){

			titulo = elem->GetText();
		}
 		else if (0 == strcmp(node->Value(),"fundo")){

			elem->QueryFloatAttribute("corR",&fundoCorR);

			elem->QueryFloatAttribute("corG",&fundoCorG);

			elem->QueryFloatAttribute("corB",&fundoCorB);

		}
		else if (0 == strcmp(node->Value(),"largura")){

			largWindow = atof(elem->GetText());

		}
		else if (0 == strcmp(node->Value(),"altura")){

			altWindow = atof(elem->GetText());

		}
	}
}

// calcula a distancia entre o centro do circulo e o clique do mouse
float dist2Points(float centroX, float centroY, int x, int y){

	return (sqrt((centroX - x)*(centroX - x) + (centroY - y)*(centroY - y)));
}

// verifica se o clique esta dentro do circulo
bool isInsideCircle (int x,int y){

	difX = abs(circulo.centroX - x);

	difY = abs(circulo.centroY - y);
	
	float dist = dist2Points(circulo.centroX, circulo.centroY, x,y);

	return (dist <= circulo.raio) ? true : false;
	
}

// desenha um circulo 
void desenhaCirculo(){

	int i, lineAmount = 100;

	GLfloat twicePi = 2.0f * PI;

	for (i = 0; i <= lineAmount;i++) { 
	
		glVertex2f( 	circulo.centroX + (circulo.raio * cos(i *  twicePi / lineAmount)), 
				circulo.centroY + (circulo.raio * sin(i * twicePi / lineAmount))
		);
	}

	// glEnd precisa ser aqui ao inves de ser no display pq eh chamada apos o desenho do objeto
	glEnd();
}

// callback dos eventos do mouse 
void mouse(int button,int state,int x,int y){

	// mudança de coordenadas
	y = altWindow - y;

	// esta dentro do circulo ?
	bool inside = isInsideCircle(x,y);

	if (button == GLUT_LEFT_BUTTON && estados == insercao){

			circulo.centroX = x;
			circulo.centroY = y;

			glutPostRedisplay();
			glutSwapBuffers();

			estados = alteracao;

	} else if (button == GLUT_LEFT_BUTTON && inside && estados == alteracao && state == GLUT_DOWN){

		    botaoEsquerdo = true;

		    difX = (circulo.centroX - x);

		    difY = (circulo.centroY - y);            

   	} else if (button == GLUT_RIGHT_BUTTON && inside && estados == alteracao && state == GLUT_DOWN ){

		botaoDireito = true;

		raioAux = dist2Points(circulo.centroX, circulo.centroY, x, y);	

	} else {

		botaoDireito = false;
		botaoEsquerdo = false;
	}
}

// callback dos movimentos do mouse
void motion(int x,int y){

	// mudança de coordenadas
	y = altWindow - y;

	if (botaoEsquerdo){ 

		circulo.centroX = x + difX;
		circulo.centroY = y + difY;

	}
	else if (botaoDireito){
		
		float difRaio = 0.0;
		
		difRaio = (dist2Points(circulo.centroX, circulo.centroY, x, y) - raioAux);

		if ((circulo.raio + difRaio) <= raioMinimo){
	
			circulo.raio = raioMinimo;
		}
		else {

			circulo.raio += difRaio;		
		}

		raioAux += difRaio;
	}

	glutPostRedisplay();
}

// callback que plota a janela e o circulo
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(circulo.corR,circulo.corG,circulo.corB);

	if (estados != insercao){

		glBegin(GL_POLYGON);		
		desenhaCirculo();
	}

	glutSwapBuffers();
}

// inicializa o rgb, camera, etc.
void inicializaGlut(int& largWindow, int& altWindow){

	glClearColor(fundoCorR,fundoCorG,fundoCorB,0); 
	glMatrixMode(GL_PROJECTION);

	// posiciona a camera com os respectivos min e max (min x, max x, min y, max y, min z, max z)
	glOrtho(0.0,largWindow,0.0,altWindow,-1.0,1.0);

}

int main(int argc, char** argv) {

	if (!argv[1]){

		std::cout << "Argumentos invalidos\n";
		exit(1);
	}

	tinyxml::XMLDocument* doc = criaDoc (argc, std::string(argv[1]));

	if (!doc){
		exit(1);
	}

	setaParametros(doc);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);                       
	glutInitWindowSize(largWindow, altWindow);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(titulo.c_str()); 
	inicializaGlut(largWindow, altWindow);
	glutDisplayFunc(display); 
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutMainLoop();      
     
	return 0;
}
