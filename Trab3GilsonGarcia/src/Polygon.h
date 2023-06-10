#ifndef FIGURA_H_INCLUDED
#define FIGURA_H_INCLUDED

#include <vector>
#include <cmath>
#include <iostream>
#include <cfloat>
#include "constants.h"
#include "math_utils.h"
#include "gl_canvas2d.h"

using namespace std;

class Polygon {
protected:
    int ID;
    bool visible, selected, resizing, rotating;

    int nPoints;
    vector<float> vx, vy;
    float angle;
    
    float offsetX, offsetY;

    float r, g, b;
    int indexColor;
    int colorScale;

    virtual void translateBy(float xIncrease, float yIncrease) {
        // Criando a matriz de translação
        vector<vector<float>> translationMatrix = {
            {1.0f, 0.0f, xIncrease},
            {0.0f, 1.0f, yIncrease},
            {0.0f, 0.0f, 1.0f}
        };

        // Aplicando a translação a cada ponto do polígono
        for (int i = 0; i < nPoints; ++i) {
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

    virtual void translateTo(float x, float y) {
        // Calculating the translation offsets
        float offsetX = x - vx[0];
        float offsetY = y - vy[0];

        translateBy(offsetX, offsetY);
    }
       
public:
    Polygon(int nPoints) {
        r = 1;
        g = 0;
        b = 0;
        visible = false;
        indexColor = 2;
        angle = 0;
        colorScale = INDEX14;
        
        this->nPoints = nPoints;
        for (int i = 0; i < nPoints; i++) {
            vx.push_back(0);
            vy.push_back(0);
        }
    }

    virtual ~Polygon() {}

    virtual void render() {
        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(r,g,b);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), nPoints);
    }

    void setColor(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        colorScale = RGBA;
    }

    void setColor(int index) {
        indexColor = index;
        colorScale = INDEX14;
    }

    float getColorR() {
        return r;
    }

    float getColorG() {
        return g;
    }

    float getColorB() {
        return b;
    }

    int getIndexColor() {
        return indexColor;
    }

    int getColorScale() {
        return colorScale;
    }

    float getAngle() {
        return angle;
    }

    int getType() {
        return ID;
    }

    vector<float> getVx() {
        return vx;
    }

    vector<float> getVy() {
        return vy;
    }

    void setVisible() {
        visible = true;
    }
    void setUnvisible() {
        visible = false;
    }

    bool isVisible() {
        return visible;
    }

    void setRotating(bool value) {
        rotating = value;
    }
    bool isRotating() {
        return rotating;
    }

    // Algorimo Ray Casting de detecção de colisão.
    virtual bool hasPointCollided(int x, int y) {
        int n = nPoints;
        int count = 0;

        for (int i = 0; i < n; i++) {
            float x1 = vx[i];
            float y1 = vy[i];
            float x2 = vx[(i + 1) % n];
            float y2 = vy[(i + 1) % n];
            
            if ((y1 > y) != (y2 > y) && (x < (x2 - x1) * (y - y1) / (y2 - y1) + x1)) {
                count++;
            }    
        }
        return count % 2 == 1;
    }

    // Algoritmo SAT de detecção de colisão
     bool hasPolygonCollided(const vector<float>& vx1, const vector<float>& vy1) {
        // Concatenate the vertices of both polygons
        vector<float> combinedVx = vx;
        vector<float> combinedVy = vy;
        combinedVx.insert(combinedVx.end(), vx1.begin(), vx1.end());
        combinedVy.insert(combinedVy.end(), vy1.begin(), vy1.end());

        // Perform the SAT check
        for (int i = 0; i < nPoints; i++) {
            int nextIndex = (i + 1) % nPoints;

            // Get the edge vector
            float edgeX = vx[i] - vx[nextIndex];
            float edgeY = vy[i] - vy[nextIndex];

            // Calculate the perpendicular vector
            float perpendicularX = -edgeY;
            float perpendicularY = edgeX;

            // Normalize the perpendicular vector
            float magnitude = sqrt(perpendicularX * perpendicularX + perpendicularY * perpendicularY);
            perpendicularX /= magnitude;
            perpendicularY /= magnitude;

            // Project the vertices onto the perpendicular vector
            float minProjectionA = FLT_MAX;
            float maxProjectionA = -FLT_MAX;
            float minProjectionB = FLT_MAX;
            float maxProjectionB = -FLT_MAX;

            for (int j = 0; j < combinedVx.size(); j++) {
                float projection = combinedVx[j] * perpendicularX + combinedVy[j] * perpendicularY;

                if (j < nPoints) {
                    minProjectionA = min(minProjectionA, projection);
                    maxProjectionA = max(maxProjectionA, projection);
                } else {
                    minProjectionB = min(minProjectionB, projection);
                    maxProjectionB = max(maxProjectionB, projection);
                }
            }

            // Check for overlap
            if (!(maxProjectionB >= minProjectionA && maxProjectionA >= minProjectionB)) {
                // The projections do not overlap, so the polygons are not colliding
                return false;
            }
        }

        // All projections overlap, so the polygons are colliding
        return true;
    }

    virtual float getCenterX() {
        float sum = 0;
        for (int i = 0; i < nPoints; i++) {
           sum += vx[i]; 
        }

        return sum / nPoints;
    }

    virtual float getCenterY() {
        float sum = 0;
        for (int i = 0; i < nPoints; i++) {
           sum += vy[i]; 
        }

        return sum / nPoints;
    }

    // algumas funções virtuais que são inerentes a todas as instâncias da classe mas
    // que serão sobrescritas
    virtual void rotate(float mx, float my) {}
    virtual void setVisible(float x, float y) {}
    virtual void setOffset(float x, float y) {}
};

#endif // FIGURA_H_INCLUDED
