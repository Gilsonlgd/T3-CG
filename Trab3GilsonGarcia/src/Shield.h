#ifndef _SHIELD_H_
#define _SHIELD_H_

#include "gl_canvas2d.h"
#include "bullet.h"
#include "math_utils.h"
#include "Enemy.h"
#include <stdio.h>
#include <vector>
#include <chrono>

#define SHIELD_COLOR 0.0, 0.0, 1.0
#define SHIELD_TIME 3000
#define TIME_TO_READY 13000

using namespace std;
/*
##### SHIELD #####
Implementa o escudo do jogador.
######################
*/

class Shield {
    vector<float> controlX;
    vector<float> controlY;

    vector<float> vx;
    vector<float> vy;

    bool isActive;
    bool ready;
    chrono::steady_clock::time_point lastActivationTime;
    chrono::steady_clock::time_point lastTimeCapture;
    chrono::milliseconds timeToReady;

    float spaceshipHeight;
    float spaceshipWidth;
    float radius;

public:
    Shield(float x, float y, float spaceshipWidth, float spaceshipHeight, float radius) {
        controlX.push_back(x - spaceshipWidth/2 - radius);
        controlY.push_back(y + spaceshipHeight/2);

        controlX.push_back(x);
        controlY.push_back(y - spaceshipHeight/2 - radius);

        controlX.push_back(x + spaceshipWidth/2 + radius);
        controlY.push_back(y + spaceshipHeight/2);    

        this->spaceshipHeight = spaceshipHeight;
        this->spaceshipWidth = spaceshipWidth;
        this->radius = radius;
        isActive = false;
        ready = true;
        timeToReady = chrono::milliseconds(0);
        lastTimeCapture = chrono::steady_clock::now();
    }

    void render() {
        if (isActive) {
            vx.clear();
            vy.clear();

            for (float t = 0; t < 1; t += 0.001) {
                float x = evaluateBezier2(controlX[0], controlX[1], controlX[2], t);
                float y = evaluateBezier2(controlY[0], controlY[1], controlY[2], t);

                vx.push_back(x);
                vy.push_back(y);

                CV::translate(0, 0);
                CV::color(SHIELD_COLOR, 1);
                CV::point(x, y);
            }

            chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
            chrono::milliseconds elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastActivationTime);

            if (elapsedTime.count() > SHIELD_TIME) {
                isActive = false;
            }  
        }
    }

    // atualiza a posição do escudo de acordo com a posição do jogador
    void refreshPosition(float x, float y) {
        controlX[0] = x - spaceshipWidth/2 - radius;
        controlY[0] = y + spaceshipHeight/2;

        controlX[1] = x;
        controlY[1] = y - spaceshipHeight/2 - radius;

        controlX[2] = (x + spaceshipWidth/2 + radius);
        controlY[2] = (y + spaceshipHeight/2);  
    }

    // atualiza o tempo para o escudo ficar pronto
    void refreshTimeToReady() {
        if (ready) return;

        chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
        chrono::milliseconds elapsedTime = chrono::duration_cast<chrono::milliseconds>(currentTime - lastActivationTime);

        if (elapsedTime.count() >= TIME_TO_READY) {
            timeToReady = chrono::milliseconds(0);
            ready = true;
        } else if (currentTime - lastTimeCapture > chrono::milliseconds(1000)) {
            timeToReady = chrono::milliseconds(timeToReady.count() - 1000);
            lastTimeCapture = currentTime;
        }
    }

    // ativa o escudo
    void activate() {
        if (ready) {
            isActive = true;
            lastActivationTime = chrono::steady_clock::now();
            timeToReady = chrono::milliseconds(TIME_TO_READY);
            ready = false;
        }   
    }

    bool isActiveShield() {
        return isActive;
    }

    int getTimeToReady() {
        return timeToReady.count() / 1000;
    }

    // verifica se o escudo colidiu com um tiro
    bool checkBulletCollision(Bullet* bullet){
        if (bullet->getBulletY() + bullet->getHeight() < controlY[1]) return false;
        if (bullet->getBulletX() + bullet->getWidth() < controlX[0]) return false;
        if (bullet->getBulletX() - bullet->getWidth() > controlX[2]) return false;

        for (int i = 0; i < vx.size(); i++){
            if (bullet->hasPointCollided(vx[i], vy[i])) {
                return true;
            }
        }

        return false;
    }

    // verifica se o escudo colidiu com um inimigo
    bool checkEnemyCollision(Enemy* enemy) {
        float enemyHeight = enemy->getHeight();
        float enemyWidth = enemy->getWidth();

        if (enemy->getY() + enemyHeight < controlY[1]) {
            return false;
        }

        if (enemy->getX() + enemyWidth < controlX[0]) {
            return false;
        }

        if (enemy->getX() - enemyWidth > controlX[2]) {
            return false;
        }

        for (int i = 0; i < vx.size(); i++) {
            if (enemy->hasPointCollided(vx[i], vy[i])) {
                return true;
            }
        }

        return false;
    }

};

#endif