#include "matrix_utils.h"

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
