#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <vector>
#include <random>
#include <algorithm>
#include "gl_canvas2d.h"
#include "math_utils.h"
#include <ctime>

#define STAR_SIZE 2
#define STARS_PER_SEGMENT 250
#define BSPLINE_X_RANGE 200
#define BSPLINE_N_CONTROL_POINTS 10

using namespace std;

class Map {
    vector<float> vx, vy;

    vector<float> curStarsX;
    vector<float> curStarsY;

    vector<float> controlPointsX;
    vector<float> controlPointsY;

    vector<float> bSplineLeftX;
    vector<float> bSplineRightX;

    vector<float> bSplineY;

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

        random_device rd;
        mt19937 rng(rd());
        uniform_int_distribution<int> distX(0, (int)baseWidth);
        uniform_int_distribution<int> distY(0, (int)baseHeight);

        for (int i = 0; i < STARS_PER_SEGMENT; i++) {
            starsX->push_back( (float)distX(rng) );
            starsY->push_back( (float)distY(rng) );
        }
    }

    void reseedStar(float* x, float* y) {
        float minHeight = -(baseHeight / 4.0);
        static std::mt19937 rng1(time(nullptr));
        uniform_real_distribution<float> distX(0.0f, (float)baseWidth);
        uniform_real_distribution<float> distY(minHeight, 0.0f);

        *x = distX(rng1);
        *y = distY(rng1);
    }

    void seedControlPoints() {
        controlPointsY.push_back(-100);
        controlPointsY.push_back(baseHeight + 100);

        random_device rd;
        mt19937 rng(rd());
        uniform_real_distribution<float> distX(0.0f, BSPLINE_X_RANGE);
        uniform_real_distribution<float> distY(0.0f, (int)baseHeight);

        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS; i++) {
            float x = distX(rng);
            float y = distY(rng);

            bSplineLeftX.push_back(x);
            bSplineRightX.push_back(baseWidth - x);
            
            bSplineY.push_back(y);
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
        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS - 3; i++) {
            for (float t = 0; t < 1; t += 0.001) {
                float xLeft = evaluateBSpline(bSplineLeftX[i], bSplineLeftX[i + 1], bSplineLeftX[i + 2], bSplineLeftX[i + 3], t);
                float xRight = evaluateBSpline(bSplineRightX[i], bSplineRightX[i + 1], bSplineRightX[i + 2], bSplineRightX[i + 3], t);

                float y = evaluateBSpline(bSplineY[i], bSplineY[i + 1], bSplineY[i + 2], bSplineY[i + 3], t);

                CV::translate(0, 0);
                CV::color(1,1,1);
                CV::point(xLeft, y);
                CV::point(xRight, y);
            }
        }
        
    }

public:
    Map(float windowWidth, float windowHeight) {
        this->baseWidth = windowWidth;
        this->baseHeight = windowHeight;

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
            curStarsY[i] += speed;
            if (curStarsY[i] > baseHeight) {
                reseedStar(&curStarsX[i], &curStarsY[i]);
            }
        }

        for (int i = 0; i < BSPLINE_N_CONTROL_POINTS; i++) {
            bSplineY[i] += speed;
        }

        float *prevLastY = &bSplineY[BSPLINE_N_CONTROL_POINTS - 2];
        if (*prevLastY > baseHeight) {
            bSplineY[BSPLINE_N_CONTROL_POINTS - 1] = -100;
            sort(bSplineY.begin(), bSplineY.end());
        }
    }
};

#endif // MAP_H_INCLUDED
