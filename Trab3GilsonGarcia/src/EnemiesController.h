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
#define INITIAL_SHOTS_INTERVAL 2500
#define MIN_SHOTS_INTERVAL 1000
#define MIN_SPAWN_INTERVAL 3000 
#define MIN_ENEMIES_PER_WAVE 3

#define ENEMIES_WIDTH 30.0
#define ENEMIES_HEIGHT 65.0
#define ENEMIES_SPACING 50
#define ENEMIES_COLOR 212.0/255, 234.0/255, 5.0/255
#define ENEMIES_BULLET_COLOR 0.0/255, 249.0/255, 26.0/255
#define SPAWN_OFFSET 40

/*
##### ENEMIES CONTROLLER #####
Implementa controle dos inimigos,
como spawn, movimentação, colisão ]
e tiros.
##############################
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
        maxEnemiesPerWave = MIN_ENEMIES_PER_WAVE;

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

    void refreshSpawnInterval(float score) {
        if ( (int)score % 100 != 0) return;
        if (waveInterval <= chrono::milliseconds(MIN_SPAWN_INTERVAL)) return;

        int decrease = (int)(score / 100) * 1000;
        waveInterval = chrono::milliseconds(INITIAL_WAVE_INTERVAL - decrease);
    }

    void refreshMaxEnemiesPerWave(float score) {
        if ( (int)score % 300 != 0) return;
        if (maxEnemiesPerWave >= 10) return;

        maxEnemiesPerWave = MIN_ENEMIES_PER_WAVE + (int)(score / 300);
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

    list<Enemy*> getEnemies() {
        return enemies;
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

    void refreshShotsInterval(float score) {
        if ( (int)score % 100 != 0) return;
        if (shotsInterval <= chrono::milliseconds(MIN_SHOTS_INTERVAL)) return;

        int decrease = (int)(score / 100) * 50;
        shotsInterval = chrono::milliseconds(INITIAL_SHOTS_INTERVAL - decrease);
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
};

#endif // ENEMIESCONTROLER_H_INCLUDED
