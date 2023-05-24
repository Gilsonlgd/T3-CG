/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Gilson Garcia
//         06/2023
//
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "gl_canvas2d.h"

#include "Keyboard.h"
#include "Spaceship.h"
#include "Polygon.h"
#include "Map.h"
#include "Botao.h"
//#include "Export.h"
//#include "Import.h"

#include "constants.h"

using namespace std;

Keyboard *kbd = NULL;
Spaceship *spaceship = NULL;
Map *map = NULL;
//Export *exportData = NULL;
//Import *importData = NULL;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1200, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY;           //variaveis globais do mouse para poder exibir dentro da render().
bool isDragging = false;      // estah arrastando?
bool isResizing = false;      // estah redimensionando uma figura? só pode uma por vez
bool isCTRLdown = false;      // estah segurando o ctrl?
bool isSHIFTdown = false;     // estah segurando o shift?


bool isMouseInsideDrawBounds(float x, float y) {
   if (x > 0 && x < screenWidth) {
      if (y > 0 && y < screenHeight) {
         return true;
      }
   };
   return false;
}

void DrawShapes()
{
 
}

void handleMapMovement() {
   map->move(0.5);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   handleMapMovement();

   map->render();
   spaceship->render();
}

void handleStarshipMovement() {
   if (kbd->isControlKeyPressed()) {
      int direction = kbd->getControlKeyPressed();
      spaceship->startMove(direction);
   }
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   if( key < 200 )
   {
      opcao = key;
   }

   kbd->startPressKey(key);

   handleStarshipMovement();
}

void handleStopStarshipMovement(int key) {
   if (key == UP || key == DOWN || key == LEFT || key == RIGHT) {
      spaceship->stopMove(key);
   }

   if (kbd->isControlKeyPressed()) {
      int direction = kbd->getControlKeyPressed();
      spaceship->startMove(direction);
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   kbd->stopPressKey(key);

   handleStopStarshipMovement(key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   if(state == 1) {
      isDragging = false;
   };

   if( state == 0 ) //clicou
   { }

}

int main(void)
{
   kbd = new Keyboard();
   spaceship = new Spaceship(screenWidth / 2, screenHeight - 100, 50, 100);
   map = new Map(screenWidth, screenHeight, 2);

   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
