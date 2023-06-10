#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "Rectangle.h"
#define BULLET_UP -1
#define BULLET_DOWN 1

using namespace std;

class Bullet : public Rectangle {
    float speed;
    float speed_up;
    bool isFired;
    int yDirection;

public: 
    Bullet(float width, float height, float speed) : Rectangle(0, 0) {
        this->width = width;
        this->height = height;
        this->speed = speed;
        isFired = false;
        yDirection = BULLET_UP;
    }

    void render() override {
        if (isFired) {
            CV::translate(0, 0);
            if (colorScale == RGBA) CV::color(r,g,b);
            else if (colorScale == INDEX14)  CV::color(indexColor);

            CV::polygonFill(vx.data(), vy.data(), 4);
            translateBy(0, speed * yDirection);
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

    float getBulletX() {
        return vx[0];
    }

    void setSpeed(float speed) {
        this->speed = speed;
    }
    
    float getSpeed() {
        return speed;
    }

    void setYDirection(int direction) {
        yDirection = direction;
    }

    vector<float> getVx() {
        return vx;
    }

    vector<float> getVy() {
        return vy;
    }
};

#endif //BULLET_H_INCLUDED