#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
#include <math.h>
struct Point {
    float x;
    float y;
};

using namespace std;

vector<vector<float>> multiplyMatrices(const vector<vector<float>>& matrix1, const vector<vector<float>>& matrix2);

float dotProduct(float x1, float y1, float x2, float y2);

void projectPolygon(const vector<float>& vertices, float axisX, float axisY, float& min, float& max);

float evaluateBSpline(float p1, float p2, float p3, float p4, float t);

#endif  // MATH_UTILS_H
