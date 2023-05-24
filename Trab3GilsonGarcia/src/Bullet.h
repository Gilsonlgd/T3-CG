#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "Rectangle.h"

using namespace std;

class Bullet : public Rectangle {
    float speed;
    float speed_up;
    bool isFired;

public: 
    Bullet(float width, float height, float speed) : Rectangle(0, 0) {
        this->width = width;
        this->height = height;
        this->speed = speed;
        isFired = false;
    }

    void render() override {
        if (isFired) {
            CV::translate(0, 0);
            if (colorScale == RGBA) CV::color(r,g,b);
            else if (colorScale == INDEX14)  CV::color(indexColor);

            CV::polygonFill(vx.data(), vy.data(), 4);
            translateBy(0, -speed);
        }
    }

    void fireBullet(float x, float y) {
        float offsetX = width/2;
        translateTo(x - offsetX, y);
        isFired = true;
    }

    float getBulletY() {
        return vy[0];
    }
};

#endif //BULLET_H_INCLUDED