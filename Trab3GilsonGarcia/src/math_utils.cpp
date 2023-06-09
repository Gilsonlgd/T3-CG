#include "math_utils.h"

using namespace std;

vector<vector<float>> multiplyMatrices(const vector<vector<float>>& matrix1, const vector<vector<float>>& matrix2) {
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();

    vector<vector<float>> result(rows1, vector<float>(cols2, 0.0f));

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

float dotProduct(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

void projectPolygon(const vector<float>& vertices, float axisX, float axisY, float& min, float& max) {
    min = dotProduct(vertices[0], vertices[1], axisX, axisY);
    max = min;

    for (size_t i = 2; i < vertices.size(); i += 2) {
        float projection = dotProduct(vertices[i], vertices[i + 1], axisX, axisY);
        if (projection < min) {
            min = projection;
        } else if (projection > max) {
            max = projection;
        }
    }
}

float evaluateBSpline(float p1, float p2, float p3, float p4, float t) {
    return (pow( (1 - t), 3.0) / 6) * p1 + ((3 * pow(t, 3.0) - 6 * pow(t, 2.0) + 4) /6) * p2 + ((-3 * pow(t, 3.0) + 3 * pow(t,2.0) + 3 * t + 1) / 6) * p3 + ( pow(t, 3.0 ) / 6 ) * p4;
}

float randomFloat(float min, float max) {
    static mt19937 rng(time(nullptr));
    uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
