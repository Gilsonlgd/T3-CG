#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "Polygon.h"
#include "constants.h"

#define LEFT_SIDE 0
#define RIGHT_SIDE 1
#define TOP_SIDE 2
#define BOTTOM_SIDE 3

/*
##### RETÂNGULO #####
Este retângulo foi implementado utilizando polígono.
Todos os métodos de translação e rotação são orientados a isso
######################
*/

using namespace std;

class Rectangle : public Polygon {
protected:
    float width;
    float height;

    // atualiza as corrdenadas do retângulo de acordo com a origem, largura e altura
    void attPointsCoord() {
        float origemX = vx[0];
        float origemY = vy[0];

        vx[1] = origemX + width;
        vy[1] = origemY;

        vx[2] = origemX + width;
        vy[2] = origemY + height;

        vx[3] = origemX;
        vy[3] = origemY + height;
    }

public:
    using Polygon::setVisible;

    Rectangle(float x, float y) : Polygon(4) {
        ID = RECT_ID;
        width = 5;
        height = 10;
        vx[0] = x;
        vy[0] = y;
        attPointsCoord();
    }

    void render() override {
        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(r,g,b);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), 4);
    }

};


#endif // RECTANGLE_H_INCLUDED
