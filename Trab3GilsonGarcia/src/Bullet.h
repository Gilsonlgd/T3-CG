#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "Rectangle.h"
#define BULLET_UP -1
#define BULLET_DOWN 1

using namespace std;

/*
##### BULLET #####
Representa uma bala que pode ser 
atirada. Usada pelo player e pelos
inimigos.
######################
*/


class Bullet : public Rectangle {
    float speed;
    float speed_up;
    bool isFired;
    float yDirection; // direção y na qual a bala se move
    float xDirection; // direção x na qual a bala se move


public: 
    Bullet(float width, float height, float speed) : Rectangle(0, 0) {
        this->width = width;
        this->height = height;
        this->speed = speed;
        isFired = false;
        yDirection = BULLET_UP;
        xDirection = 0;
    }

    void render() override {
        if (isFired) {
            CV::translate(0, 0);
            if (colorScale == RGBA) CV::color(r,g,b);
            else if (colorScale == INDEX14)  CV::color(indexColor);

            CV::polygonFill(vx.data(), vy.data(), 4);
        }
    }

    void move(float deltaTime) {
        translateBy(speed * xDirection * deltaTime, speed * yDirection * deltaTime);
    }

    // utlizado para setar a posição da bala no momento do tiro
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

    void setYDirection(float direction) {
        yDirection = direction;
    }

    void setXDirection(float direction) {
        xDirection = direction;
    }

    void rotateBullet(float angle) {
        rotatePoints(vx.data(), vy.data(), nPoints, vx[3], vy[3], angle);
        this->angle = angle;
    }

    vector<float> getVx() {
        return vx;
    }

    vector<float> getVy() {
        return vy;
    }
};

#endif //BULLET_H_INCLUDED