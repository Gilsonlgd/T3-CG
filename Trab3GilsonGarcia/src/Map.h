#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <vector>
#include <random>
#include "gl_canvas2d.h"
#include "matrix_utils.h"

#define STAR_SIZE 2
#define STARS_PER_SEGMENT 500

using namespace std;

class Map {
    vector<float> vx, vy;

    vector<int> curStarsX;
    vector<int> curStarsY;

    vector<int> nextStarsX;
    vector<int> nextStarsY;

    float baseWidth;

    float baseHeight;
    float segmentSize;

    int currentSegment;
    int segmentsPerTime;

    void translate(float xIncrease, float yIncrease) {
        // Criando a matriz de translação
        vector<vector<float>> translationMatrix = {
            {1.0f, 0.0f, xIncrease},
            {0.0f, 1.0f, yIncrease},
            {0.0f, 0.0f, 1.0f}
        };

        // Aplicando a translação a cada ponto do polígono
        for (int i = 0; i < 4; ++i) {
            // Adicionando as coordenadas do ponto como uma coluna
            vector<vector<float>> pointMatrix = {
                {vx[i]},
                {vy[i]},
                {1.0f}
            };

            // Multiplicando a matriz de translação pelo ponto
            vector<vector<float>> transformedPoint = multiplyMatrices(translationMatrix, pointMatrix);

            // Atualizando as coordenadas do ponto no polígono
            vx[i] = transformedPoint[0][0];
            vy[i] = transformedPoint[1][0];
        }
    }

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

    void seedStars(vector<int> *starsX, vector<int> *starsY) {
        starsX->clear();
        starsY->clear();

        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> distX(0, (int)baseWidth);
        uniform_int_distribution<int> distY((int)(-baseHeight * ( segmentsPerTime-1)), (int)segmentSize);

        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            starsX->push_back( distX(rng));
            starsY->push_back( distY(rng));
        }
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

public:
    Map(float windowWidth, float windowHeight, int segmentsPerTime) {
        this->baseWidth = windowWidth;
        this->baseHeight = windowHeight;
        this->segmentsPerTime = segmentsPerTime;
        this->segmentSize = windowHeight * segmentsPerTime;

        currentSegment = 0;
        initiateCoordinates();
        seedStars(&curStarsX, &curStarsY);
    }

    void render() {
        CV::translate(0, 0);
        CV::color(0,0,0);
        CV::polygonFill(vx.data(), vy.data(), 4);

        //renderStars();
    }

    void move(float speed) {
        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            curStarsY[i] += speed;
        }
    }
};

#endif // MAP_H_INCLUDED
