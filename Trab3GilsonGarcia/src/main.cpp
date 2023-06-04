/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Gilson Garcia
//         06/2023
//
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <chrono>

#include "gl_canvas2d.h"

#include "Keyboard.h"
#include "Spaceship.h"
#include "Polygon.h"
#include "Map.h"
#include "FPSControl.h"
//#include "Export.h"
//#include "Import.h"

#include "constants.h"

using namespace std;

Keyboard *kbd = NULL;
Spaceship *spaceship = NULL;
Map *map = NULL;
FPSControl* fpsControl = NULL;
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

/*
float radIni = 0;

void drawWireframe(float r, float interval) {
   float radInterval = interval * PI / 180;
   float rad = 0;
   CV::color(0, 0, 0);

   for (rad = 0; rad < 2*PI; rad += 2*radInterval) {
      float x1 = r * cos(rad + radIni);
      float y1 = r * sin(rad + radIni );

      float x2 = r * cos(rad + radInterval + radIni);
      float y2 = r * sin(rad + radInterval + radIni);

      CV::line(x1, y1, x2, y2);

      float ri = r - 10; 
      for (float i = rad + radInterval; i < rad + 2*radInterval; i+= 0.0001) {
         float xi1 = ri * cos(i + radIni);
         float yi1 = ri * sin(i + radIni);

         float xi2 = ri * cos(i + 0.001 + radIni);
         float yi2 = ri * sin(i + 0.001 + radIni);

         CV::line(xi1, yi1, xi2, yi2);
      }

      CV::line(x1, y1, ri * cos(rad + radIni), ri * sin(rad + radIni));
      CV::line(x2, y2, ri * cos(rad + radInterval + radIni), ri * sin(rad + radInterval+ radIni));
   }

   radIni += 0.01;
}

*/
/*
float radIni = 0;

void drawSpin() {
   CV::color(0, 0, 0);
   CV::translate(200,200);
   float r = 0;

   for (float ang = 0; ang < 5*PI; ang+=0.001) {
      float x = r * cos(ang + radIni);
      float y = r * sin(ang + radIni);
      CV::point(x, y);
      r+=0.01;
   }
   radIni += 0.001;
} 
*/
//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   handleMapMovement();

   map->render();
   spaceship->render();

   fpsControl->limitRefreshRate();
   printf("fps: %d\n", fpsControl->getActualFrameRate());

   //drawSpin();
   //CV::translate(200, 200);
   //drawWireframe(100, 20);

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
   if (kbd->isPressed(SPACE)) spaceship->shot();
   
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
   spaceship = new Spaceship((float)screenWidth / 2, screenHeight - 100, 50, 100);
   map = new Map(screenWidth, screenHeight, 2);
   fpsControl = new FPSControl(60, chrono::steady_clock::now());

   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
