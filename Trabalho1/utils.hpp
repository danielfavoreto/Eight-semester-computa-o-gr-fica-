
Circulo circulo;

Estado estados = insercao;

// parametros da janela
int largWindow, altWindow;
float fundoCorR, fundoCorG, fundoCorB;
std::string titulo;

// distancia do clique ate a posicao atual
float difX = 0.0;
float difY = 0.0;

float raioAux = 0.0;

float raioMinimo = 5.0;

bool botaoEsquerdo = false;
bool botaoDireito = false;

bool isInsideCircle(int x,int y);

void drawCirculo();

void mouse(int button,int state,int x,int y);

void motion(int x,int y);

void display();
void inicializaGlut(int& largWindow, int& altWindow);

// funcoes de parse do xml
tinyxml::XMLDocument* criaDoc (int argc, std::string argv);
void setaParametros(tinyxml::XMLDocument* doc);

#define PI 3.1415926535897932384626433832795
