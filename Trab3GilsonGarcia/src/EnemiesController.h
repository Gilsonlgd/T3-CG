#ifndef ENEMIESCONTROLER_H_INCLUDED
#define ENEMIESCONTROLER_H_INCLUDED

#include <list>
#include "stdio.h"
#include "Enemy.h"
#include <chrono>
#include <thread>

using namespace std;

#define Y_AXIS_SPAWN -100.0
#define SPEED_UP_INCREASE 0.04
#define INITIAL_WAVE_INTERVAL 5000
#define ENEMIES_WIDTH 30.0
#define ENEMIES_HEIGHT 65.0
#define ENEMIES_SPACING 50

/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class EnemiesController{
    list<Enemy*> enemies;
    float windowHeight; 
    chrono::milliseconds waveInterval;
    chrono::steady_clock::time_point lastWaveTime;

public:
    EnemiesController(int nEnemies, float windowHeight) {
        enemies = list<Enemy*>();
        this->windowHeight = windowHeight;
        waveInterval = chrono::milliseconds(INITIAL_WAVE_INTERVAL);
        lastWaveTime = chrono::steady_clock::now();

        spawnEnemiesWave(10, 200, lastWaveTime);
    }

    void render() {
        for (Enemy* enemy : enemies) {
            enemy->render();
        }
    }

    void spawnEnemiesWave(int nEnemies, float xStart, chrono::steady_clock::time_point currentTime) {
        for (int i = 0; i < nEnemies; i++) {
            enemies.push_back(new Enemy(xStart + i * (ENEMIES_WIDTH + ENEMIES_SPACING), Y_AXIS_SPAWN, ENEMIES_WIDTH, ENEMIES_HEIGHT, 2));
            lastWaveTime = currentTime;
        }
    }

    void move(float speed) {
        for (Enemy* enemy : enemies) {
            enemy->move(speed);
            if (enemy->getY() > windowHeight + 100) {
                enemies.remove(enemy);
                delete enemy;
            }
        }
    }

    bool checkBulletCollision(Bullet* bullet) {
        for (Enemy* enemy : enemies) {
            bool collided = enemy->checkBulletCollision(bullet);
            if (collided) {
                enemies.remove(enemy);
                delete enemy;
                return true;
            }
        }
    }

    float getEnemiesHeight() {
        return ENEMIES_HEIGHT;
    }

    float getEnemiesWidth() {
        return ENEMIES_WIDTH;
    }

    float getEnemiesSpacing() {
        return ENEMIES_SPACING;
    }

    float getYAxisSpawn() {
        return Y_AXIS_SPAWN;
    }

    chrono::steady_clock::time_point getLastWaveTime() {
        return lastWaveTime;
    }

    chrono::milliseconds getWaveInterval() {
        return waveInterval;
    }

    /*void setWaveInterval(float milliseconds) {
        this->waveInterval = chrono::milliseconds(milliseconds);
    }*/
};

#endif // ENEMIESCONTROLER_H_INCLUDED
