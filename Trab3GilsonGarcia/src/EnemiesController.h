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

#define INITIAL_WAVE_INTERVAL 7000
#define INITIAL_SHOTS_INTERVAL 3000

#define ENEMIES_WIDTH 30.0
#define ENEMIES_HEIGHT 65.0
#define ENEMIES_SPACING 50
#define ENEMIES_COLOR 212.0/255, 234.0/255, 5.0/255
#define ENEMIES_BULLET_COLOR 0.0/255, 249.0/255, 26.0/255
#define SPAWN_OFFSET 40

/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class EnemiesController{
    list<Enemy*> enemies;
    float windowHeight; 
    int maxEnemiesPerWave;
    chrono::milliseconds waveInterval;
    chrono::steady_clock::time_point lastWaveTime;

    chrono::milliseconds shotsInterval;
    chrono::steady_clock::time_point lastShotTime;

public:
    EnemiesController(float windowHeight) {
        enemies = list<Enemy*>();
        this->windowHeight = windowHeight;
        maxEnemiesPerWave = 4;

        waveInterval = chrono::milliseconds(INITIAL_WAVE_INTERVAL);
        shotsInterval = chrono::milliseconds(INITIAL_SHOTS_INTERVAL);

        lastShotTime = chrono::steady_clock::now();
        lastWaveTime = chrono::steady_clock::now();
    }

    void render() {
        for (Enemy* enemy : enemies) {
            enemy->render();
        }
    }

    void spawnEnemiesWave(float xStart, float xEnd, chrono::steady_clock::time_point currentTime) {
        float intervalLen = xEnd - xStart;
        int nEnemies = (int)(intervalLen / (ENEMIES_WIDTH + ENEMIES_SPACING));
        
        if (nEnemies > maxEnemiesPerWave) {
            nEnemies = maxEnemiesPerWave;
        }

        float totalEnemiesWidth = nEnemies * (ENEMIES_WIDTH + ENEMIES_SPACING);
        float xRange = intervalLen - totalEnemiesWidth;
        float xRangeStart = randomFloat(xStart, xStart + xRange);
        float curXTranslation = xRangeStart - xStart;


        for (int i = 0; i < nEnemies; i++) {
            Enemy* enemy = new Enemy(xRangeStart + SPAWN_OFFSET + i * (ENEMIES_WIDTH + ENEMIES_SPACING), Y_AXIS_SPAWN, ENEMIES_WIDTH, ENEMIES_HEIGHT, xRange);
            enemy->setCurrentXTranslation(curXTranslation);
            enemy->setColor(ENEMIES_COLOR);
            enemy->setBulletColor(ENEMIES_BULLET_COLOR);
            enemies.push_back(enemy);
            lastWaveTime = currentTime;
        }
    }

    void move(float speed, float deltaTime) {
        for (Enemy* enemy : enemies) {
            enemy->move(speed, deltaTime);
            if (enemy->getY() > windowHeight + 100) {
                enemies.remove(enemy);
                delete enemy;
            }
        }
    }

    bool checkBulletCollision(Bullet* bullet) {
        for (Enemy* enemy : enemies) {
            if (enemy->isAlive()) {
                bool collided = enemy->checkBulletCollision(bullet);
                if (collided) {
                    enemy->setAlive(false);
                    return true;
                }
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

    void handleEnemiesShooting(float x, float y) {
        chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
        if (currentTime - lastShotTime > shotsInterval) {
            for (Enemy* enemy : enemies) {
                if (enemy->isAlive()) enemy->shotTo(x, y);
            }
            lastShotTime = chrono::steady_clock::now();
        }
    }

    list<Bullet*> getShots() {
        list<Bullet*> shots = list<Bullet*>();

        for (Enemy* enemy : enemies) {
            list<Bullet*> enemyShots = enemy->getFiredBullets();
            shots.insert(shots.end(), enemyShots.begin(), enemyShots.end());
        }
        return shots;
    }

    void removeShot(Bullet* bullet) {
        for (Enemy* enemy : enemies) {
            enemy->removeBullet(bullet);
        }
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
