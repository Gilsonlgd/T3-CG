#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <vector>

using namespace std;

vector<vector<float>> multiplyMatrices(const vector<vector<float>>& matrix1, const vector<vector<float>>& matrix2);

float dotProduct(float x1, float y1, float x2, float y2);

void projectPolygon(const vector<float>& vertices, float axisX, float axisY, float& min, float& max);

#endif  // MATRIX_UTILS_H
