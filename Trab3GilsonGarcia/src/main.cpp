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
#include <random>
#include <string>

#include "gl_canvas2d.h"

#include "Keyboard.h"
#include "Spaceship.h"
#include "Polygon.h"
#include "EnemiesController.h"
#include "Map.h"
#include "HomeScreen.h"
#include "FPSControl.h"

#include "constants.h"

using namespace std;

#define STARTING_GAME 0
#define RUNNING_GAME 1
#define GAME_OVER 2

Keyboard *kbd = NULL;
Spaceship *spaceship = NULL;
Map *map = NULL;
FPSControl* fpsControl = NULL;
HomeScreen* homeScreen = NULL;
EnemiesController* enemiesController = NULL;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1200, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY;           //variaveis globais do mouse para poder exibir dentro da render().
int gameState;


void gameOver() {
   printf("\nVocê perdeu!\n");
   //exit(0);
}

void handleMapMovement() {
   map->move(spaceship->getSpeed());
   enemiesController->move(spaceship->getSpeed());
}

void handlePlayerShotsCollision() {
   list<Bullet*> shots = spaceship->getShots();

   for (Bullet* bullet : shots) {
      if (enemiesController->checkBulletCollision(bullet)) {
         spaceship->removeShot(bullet);
      }
   }
}

void handleEnemiesShotsCollision() {
   list<Bullet*> shots = enemiesController->getShots();

   for (Bullet* shot : shots) {
      if (spaceship->checkBulletCollision(shot)) {
         enemiesController->removeShot(shot);
         gameOver();
         //lida com a colisão
      }
   }
}

void handleEnemiesSpawn() {
   chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
   chrono::steady_clock::time_point lastWaveTime = enemiesController->getLastWaveTime();
   chrono::milliseconds elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastWaveTime);
   chrono::milliseconds waveInterval = enemiesController->getWaveInterval();

   if (elapsedTime.count() > waveInterval.count()) {
      float xStart = map->getEnemiesIntervalMinX();
      float xEnd = screenWidth - xStart;
      enemiesController->spawnEnemiesWave(xStart, xEnd, currentTime);
   }
}

void handleRunningGame() {
   handleMapMovement();
   handlePlayerShotsCollision();
   handleEnemiesShotsCollision();
   handleEnemiesSpawn();

   map->render();
   spaceship->render();
   enemiesController->render();
}

void printFrameRate() {
   int FPS = (int)(fpsControl->getActualFrameRate()); 
   string frameRate = "FPS: " + to_string(FPS);

   CV::color(1, 1, 1);
   CV::text(screenWidth - 100, 20, frameRate.c_str());
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   if (gameState == STARTING_GAME) {
      homeScreen->render();
   }

   if (gameState == RUNNING_GAME) {
      handleRunningGame();
   }

   if (map->checkSpaceshipCollision(spaceship)) {
      gameOver();
      //lida com a colisão
   }

   fpsControl->limitRefreshRate();
   printFrameRate();
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

   if(state == 1) {}

   if( state == 0 ) //clicou
   {
      int state = homeScreen->checkButtonsClick(mouseX, mouseY);

      if (state == START_GAME) {
         gameState = RUNNING_GAME;
      } else if (state == EXIT_GAME) {
         exit(0);
      }
   }

}

int main(void)
{
   kbd = new Keyboard();
   spaceship = new Spaceship((float)screenWidth / 2, screenHeight - 100, 35, 70, 2.5, 1);
   map = new Map(screenWidth, screenHeight);
   homeScreen = new HomeScreen(screenWidth, screenHeight, "Spaceship Wars");
   enemiesController = new EnemiesController(screenHeight);
   fpsControl = new FPSControl(60, chrono::steady_clock::now());

   map->setPlayerCollisionInterval(spaceship->getCenterY() - spaceship->getHeight()/2, spaceship->getCenterY() + spaceship->getHeight()/2);
   map->setEnemiesCollisionInterval(enemiesController->getYAxisSpawn(), enemiesController->getYAxisSpawn() + enemiesController->getEnemiesHeight());

   gameState = STARTING_GAME;
   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
