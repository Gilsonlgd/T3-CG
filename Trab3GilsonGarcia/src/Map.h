#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdio.h>
#include <vector>
#include <random>
#include <algorithm>
#include "gl_canvas2d.h"
#include "math_utils.h"
#include <ctime>
#include "Spaceship.h"

#define STAR_SIZE 2
#define STARS_PER_SEGMENT 150
#define BSPLINE_X_RANGE 400
#define BSPLINE_N_CONTROL_POINTS 10 

using namespace std;

/*
##### MAP #####
Implementa o mapa do jogo, 
e suas estrelas. Movimentação
e colisão com as curvas 
acontem aqui.
######################
*/


class Map {
    vector<float> vx, vy;

    vector<float> curStarsX;
    vector<float> curStarsY;

    vector<float> controlPointsX;
    vector<float> controlPointsY;

    vector<float> bSplineLeftX;
    vector<float> bSplineRightX;

    vector<float> bSplineY;

    // guardo um bitmap de colisão para cada curva
    // no intervalo da nave do jogador para melhor performance.
    vector<Point*> playerCollisionBitMapL; 
    vector<Point*> playerCollisionBitMapR;
    float playerCollisionIntervalStart;
    float playerCollisionIntervalEnd;

    // guardo um bitmap de colisão para cada curva
    // no intervalo dos inimigos para melhor performance.
    vector<Point*> enemiesCollisionBitMapL;
    vector<Point*> enemiesCollisionBitMapR;
    float enemiesCollisionIntervalStart;
    float enemiesCollisionIntervalEnd;

    float baseWidth;
    float baseHeight;

    void initiateCoordinates() {
        for (int i = 0; i < 4; i++) {
            vx.push_back(0);
            vy.push_back(0);
        }

        vx[0] = 0;
        vy[0] = 0;

        vx[1] = baseWidth;
        vy[1] = 0;

        vx[2] = baseWidth;
        vy[2] = baseHeight;

        vx[3] = 0;
        vy[3] = baseHeight;
    }

    void seedStars(vector<float> *starsX, vector<float> *starsY) {
        starsX->clear();
        starsY->clear();

        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            starsX->push_back( randomFloat(0, baseWidth) );
            starsY->push_back( randomFloat(0, baseHeight) );
        }
    }

    void reseedStar(float* x, float* y) {
        float minHeight = -(baseHeight / 4.0);
        
        *x = randomFloat(0, baseWidth);
        *y = randomFloat(minHeight, 0);
    }

    // gera os pontos de controle iniciais das curvas.
    // y incrementado de 250 em 250. x aleatório.
    void seedControlPoints() {
        random_device rd;
        mt19937 rng(rd());
        uniform_real_distribution<float> distX(0.0f, BSPLINE_X_RANGE);
        uniform_real_distribution<float> distY(0.0f, (int)baseHeight);

        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS; i++) {
            float x = distX(rng);

            bSplineLeftX.push_back(x);
            bSplineRightX.push_back(baseWidth - x);            
        }

        float initialCordinate = -1000;
        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS; i++) {
            bSplineY.push_back(initialCordinate);
            initialCordinate += 250;
        }

        sort(bSplineY.begin(), bSplineY.end());
    }

    void renderStars() {
        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            CV::translate(0, 0);
            CV::color(1,1,1);
            float sx = curStarsX[i];
            float sy = curStarsY[i];
            CV::rectFill(sx, sy, sx + STAR_SIZE, sy + STAR_SIZE);
        }
    }

    void renderBSpline() {
        playerCollisionBitMapL.clear();
        playerCollisionBitMapR.clear();

        enemiesCollisionBitMapL.clear();
        enemiesCollisionBitMapR.clear();

        for (int i = 0; i < bSplineY.size() - 3; i++) {
            for (float t = 0; t < 1; t += 0.001) {
                float xLeft = evaluateBSpline(bSplineLeftX[i], bSplineLeftX[i + 1], bSplineLeftX[i + 2], bSplineLeftX[i + 3], t);
                float xRight = evaluateBSpline(bSplineRightX[i], bSplineRightX[i + 1], bSplineRightX[i + 2], bSplineRightX[i + 3], t);

                float y = evaluateBSpline(bSplineY[i], bSplineY[i + 1], bSplineY[i + 2], bSplineY[i + 3], t);

                CV::translate(0, 0);
                CV::color(1,1,1);
                CV::point(xLeft, y);
                CV::point(xRight, y);

                if (y >= playerCollisionIntervalStart && y <= playerCollisionIntervalEnd) {
                    Point* pLeft = new Point();
                    pLeft->x = xLeft;
                    pLeft->y = y;
                    
                    Point* pRight = new Point();
                    pRight->x = xRight;
                    pRight->y = y;

                    playerCollisionBitMapL.push_back(pLeft);
                    playerCollisionBitMapR.push_back(pRight);
                }

                if (y >= enemiesCollisionIntervalStart && y <= enemiesCollisionIntervalEnd) {
                    Point* pLeft = new Point();
                    pLeft->x = xLeft;
                    pLeft->y = y;
                    
                    Point* pRight = new Point();
                    pRight->x = xRight;
                    pRight->y = y;

                    enemiesCollisionBitMapL.push_back(pLeft);
                    enemiesCollisionBitMapR.push_back(pRight);
                }
            }
        }  
    }

public:
    Map(float windowWidth, float windowHeight) {
        this->baseWidth = windowWidth;
        this->baseHeight = windowHeight;
        playerCollisionIntervalStart = 0;
        playerCollisionIntervalEnd = 0;

        initiateCoordinates();
        seedStars(&curStarsX, &curStarsY);
        seedControlPoints();
    }

    void render() {
        CV::translate(0, 0);
        CV::color(0,0,0);
        CV::polygonFill(vx.data(), vy.data(), 4);

        renderStars();
        renderBSpline();
    }

    void move(float speed) {
        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            curStarsY[i] += speed / 20;
            if (curStarsY[i] > baseHeight) {
                reseedStar(&curStarsX[i], &curStarsY[i]);
            }
        }

        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS; i++) {
            bSplineY[i] += speed;
        }

        float *prevLastY = &bSplineY[bSplineY.size() - 5];
        if (*prevLastY > baseHeight + 200) {
            if (bSplineY[0] <= 0) {
                bSplineY[BSPLINE_N_CONTROL_POINTS - 1] = bSplineY[0] - 250;
            } else {
                bSplineY[0] = -250;
                for (int i = 1; i < BSPLINE_N_CONTROL_POINTS - 1; i++) {
                    bSplineY[i] = bSplineY[i - 1] + 250;
                }
            }
            sort(bSplineY.begin(), bSplineY.end());

            float xLeft = randomFloat(0, BSPLINE_X_RANGE);
            float xRight = baseWidth - xLeft;

            bSplineLeftX.pop_back();
            bSplineRightX.pop_back();

            bSplineLeftX.insert(bSplineLeftX.begin(), xLeft);
            bSplineRightX.insert(bSplineRightX.begin(), xRight);
        }
    }

    // seta os intervalos de colisão para armazenar os bitmaps de colisão
    void setPlayerCollisionInterval(float y1, float y2) {
        playerCollisionIntervalStart = y1;
        playerCollisionIntervalEnd = y2;
    }

    void setEnemiesCollisionInterval(float y1, float y2) {
        enemiesCollisionIntervalStart = y1;
        enemiesCollisionIntervalEnd = y2;
    }

    bool checkSpaceshipCollision(Spaceship* spaceship) {
        
        for (int i = 0; i < playerCollisionBitMapL.size(); i++) {
            
            if (spaceship->hasPointCollided(playerCollisionBitMapL[i]->x, playerCollisionBitMapL[i]->y)) {
                spaceship->setCanMoveToLeft(false);
                spaceship->translateBackwardsFromCollisionPointL(playerCollisionBitMapL[i]->x);
                return true;
            }

            if (spaceship->hasPointCollided(playerCollisionBitMapR[i]->x, playerCollisionBitMapR[i]->y)) {
                spaceship->setCanMoveToRight(false);
                spaceship->translateBackwardsFromCollisionPointR(playerCollisionBitMapR[i]->x);
                return true;
            }
        }
        
        spaceship->setCanMoveToLeft(true);
        spaceship->setCanMoveToRight(true);

        return false;
    }

    // retorna o ponto do intervalo mais próximo das naves
    // dos inimigos. usado para definir a posição de spawn 
    float getEnemiesIntervalMinX() {
        float maxX = -1000000;
        for (int i = 0; i < enemiesCollisionBitMapL.size(); i++) {
            if (enemiesCollisionBitMapL[i]->x > maxX) {
                maxX = enemiesCollisionBitMapL[i]->x;
            }
        }

        return maxX;
    }
};

#endif // MAP_H_INCLUDED