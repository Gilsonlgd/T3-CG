#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <list>
#include "stdio.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "Bullet.h"

using namespace std;

#define SPEED_UP_INCREASE 0.04

/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class Enemy : public Polygon{
    float height;
    float width;

    float speed;
    float speed_up;

    int accelerating;
    bool isMoving;
    int xDirection;
    int yDirection;
    int indexColor;

    float maxXrange;

    list<Bullet*> firedBullets;

    void renderBullets() {
        if (firedBullets.size()) {
            for (auto bullet : firedBullets) {
                bullet->render();

                if (bullet->getBulletY() < -10) {
                    firedBullets.remove(bullet);
                    delete bullet;
                }
            }
        }
    }

    void shot() {
        Bullet* bullet = new Bullet(5, 10, 10);
        bullet->fireBullet(vx[0], vy[0] + height);
        firedBullets.push_back(bullet);
    }

public:
    Enemy(float x, float y, float width, float height, float maxXrange) : Polygon(5){
        vx[0] = x;
        vy[0] = y;

        vx[1] = x + width/2;
        vy[1] = y + height * 3.0/4.0;    ;

        vx[2] = x + width/3;
        vy[2] = y + height;

        vx[3] = x - width/3;
        vy[3] = y + height;

        vx[4] = x - width/2;
        vy[4] = y + height * 3.0/4.0;
        
        this->height = height;
        this->width = width;
        this->maxXrange = maxXrange;

        colorScale = RGBA;
        indexColor = 0;
        xDirection = 0;
        yDirection = 1;
    }

    virtual void render() override {
        renderBullets();
        translateBy(xDirection * speed, 0);

        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(233.0/265, 15.0/265, 213.0/265, 1);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), nPoints);

    }
    
    float getX() {
        return vx[0];
    }

    float getY() {
        return vy[0];
    }

    float getHeight() {
        return height;
    }

    void setHeight(float height) {
        this->height = height;
    }

    float getWidth() {
        return width;
    }

    void setWidth(float width) {
        this->width = width;
    }

    float getSpeed() {
        return speed;
    }

    void setSpeed(float speed) {
        this->speed = speed;
    }

    float getSpeedUp() {
        return speed_up;
    }

    void setSpeedUp(float speed_up) {
        this->speed_up = speed_up;
    }

    void move(float speed) {
        translateBy(0, speed);
    }

    bool checkBulletCollision(Bullet* bullet) {
        if (bullet->getBulletY() > vy[0] + height) {
            return false;
        }

        if (bullet->getBulletX() > vx[0] + width/2) {
            return false;
        }

        if (bullet->getBulletX() < vx[0] - width/2) {
            return false;
        }

        return hasPolygonCollided(bullet->getVx(), bullet->getVy());
    }
};

#endif // ENEMY_H_INCLUDED