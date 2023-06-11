#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include <math.h>
#include <random>
#include <ctime>
#include "constants.h"
struct Point {
    float x;
    float y;
};

struct Math_Vector {
    float x;
    float y;
};

using namespace std;

/*
##### math_utils #####
Algumas funções matemáticas
necessárias.
######################
*/


vector<vector<float>> multiplyMatrices(const vector<vector<float>>& matrix1, const vector<vector<float>>& matrix2);

float dotProduct(float x1, float y1, float x2, float y2);

void projectPolygon(const vector<float>& vertices, float axisX, float axisY, float& min, float& max);

float evaluateBSpline(float p1, float p2, float p3, float p4, float t);

float randomFloat(float min, float max);

float dist(float x1, float y1, float x2, float y2);

float angleDEG(float x1, float y1, float x2, float y2);

void rotatePoint(float& x1, float& y1, float pivotX, float pivotY, float angle);

void rotatePoints(float* vx, float* vy, int nPoints, float pivotX, float pivotY, float angle);

float calculateMagnitude(float x, float y);

#endif  // MATH_UTILS_H
