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
#include "EndScreen.h"
#include "FPSControl.h"

#include "constants.h"

using namespace std;

#define STARTING_GAME 0
#define RUNNING_GAME 1
#define GAME_OVER 2
#define SCORE_PER_SHOT 10

Keyboard *kbd = NULL;
Spaceship *spaceship = NULL;
Map *map = NULL;
FPSControl* fpsControl = NULL;
HomeScreen* homeScreen = NULL;
EndScreen* endScreen = NULL;
EnemiesController* enemiesController = NULL;

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1200, screenHeight = 700; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY;           //variaveis globais do mouse para poder exibir dentro da render().
int gameState;
float deltaTime;
float score;

void printFrameRate() {
   int FPS = (int)(fpsControl->getActualFrameRate()); 
   string frameRate = "FPS: " + to_string(FPS);

   CV::color(1, 1, 1);
   CV::text(screenWidth - 100, 80, frameRate.c_str());
}

void printScore() {
   string scoreText = "Score: " + to_string((int)score);

   CV::color(1, 1, 1);
   CV::text(screenWidth - 180, 45, scoreText.c_str());
   CV::rect(screenWidth - 200, 20, screenWidth - 20, 60);
}

void printPlayerLifes() {
   string lifesText = "Lifes: " + to_string(spaceship->getNLifes());

   CV::color(1, 1, 1);
   CV::text(40, 45, lifesText.c_str());
   CV::rect(20, 20, 200, 60);
}

void handleMapMovement() {
   map->move(spaceship->getSpeed() * deltaTime);
   enemiesController->move(spaceship->getSpeed(), deltaTime);
   spaceship->move(deltaTime);
}

void handleEnemiesShooting() {
   enemiesController->handleEnemiesShooting(spaceship->getCenterX(), spaceship->getCenterY());
   enemiesController->refreshShotsInterval(score);
}

void handlePlayerShotsCollision() {
   list<Bullet*> shots = spaceship->getShots();

   for (Bullet* bullet : shots) {
      if (enemiesController->checkBulletCollision(bullet)) {
         score += SCORE_PER_SHOT;
         spaceship->removeShot(bullet);
      }
   }
}

void handleEnemiesShotsCollision() {
   list<Bullet*> shots = enemiesController->getShots();

   for (Bullet* shot : shots) {
      if (spaceship->checkBulletCollision(shot)) {
         if (!spaceship->isInvulnerable()) {
            enemiesController->removeShot(shot);
            spaceship->decNLifes();
            spaceship->setInvulnerable();

            if (spaceship->getNLifes() == 0) {
               gameState = GAME_OVER;
               endScreen->setFinalScore(score);
            }
         }
      }
   }
}

void handleEnemiesPlayerCollision() {
   list<Enemy*> enemies = enemiesController->getEnemies();

   for (Enemy* enemy : enemies) {
      if (enemy->isAlive()) {
         if (spaceship->checkEnemyCollision(enemy)) {
            if (!spaceship->isInvulnerable()) {
               spaceship->decNLifes();
               spaceship->setInvulnerable();

               if (spaceship->getNLifes() == 0) {
                  gameState = GAME_OVER;
                  endScreen->setFinalScore(score);
               }
            }
         }
      } 
   }
}

void handlePlayerMapCollision() {   
   if (map->checkSpaceshipCollision(spaceship)) {
      if (!spaceship->isInvulnerable()) {
         spaceship->decNLifes();
         spaceship->setInvulnerable();
            
         if (spaceship->getNLifes() == 0) {
            gameState = GAME_OVER;
            endScreen->setFinalScore(score);
         }
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

   enemiesController->refreshSpawnInterval(score);
}

void handleRunningGame() {
   handleMapMovement();
   handlePlayerMapCollision();
   handleEnemiesShooting();
   handlePlayerShotsCollision();
   handleEnemiesShotsCollision();
   handleEnemiesPlayerCollision();
   handleEnemiesSpawn();

   map->render();
   spaceship->render();
   enemiesController->render();

   printScore();
   printPlayerLifes();
   printFrameRate();
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

   if (gameState == GAME_OVER) {
      endScreen->render();
   }

   fpsControl->limitRefreshRate();
   deltaTime = fpsControl->getDeltaTime();
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
      int newGameState = -1;

      if (gameState == STARTING_GAME) newGameState = homeScreen->checkButtonsClick(x, y);
      else if (gameState == GAME_OVER) newGameState = endScreen->checkButtonsClick(x, y);

      if (newGameState == START_GAME) {
         gameState = RUNNING_GAME;
      } else if (newGameState == EXIT_GAME) {
         exit(0);
      }
   }

}

int main(void)
{
   kbd = new Keyboard();
   spaceship = new Spaceship((float)screenWidth / 2, screenHeight - 100, 35, 70, 4, 2);
   map = new Map(screenWidth, screenHeight);
   homeScreen = new HomeScreen(screenWidth, screenHeight, "Spaceship Wars");
   endScreen = new EndScreen(screenWidth, screenHeight, "Game Over");
   enemiesController = new EnemiesController(screenHeight);
   fpsControl = new FPSControl(60, chrono::steady_clock::now());

   map->setPlayerCollisionInterval(spaceship->getCenterY() - spaceship->getHeight()/2, spaceship->getCenterY() + spaceship->getHeight()/2);
   map->setEnemiesCollisionInterval(enemiesController->getYAxisSpawn(), enemiesController->getYAxisSpawn() + enemiesController->getEnemiesHeight());

   gameState = STARTING_GAME;
   score = 0;
   CV::init(&screenWidth, &screenHeight, "");
   CV::run();
}
