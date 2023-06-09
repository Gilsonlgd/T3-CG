#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <list>
#include "stdio.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "Bullet.h"

using namespace std;

#define SPEED_UP_INCREASE 0.04
#define INITIAL_SPEED_X 1.5

/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class Enemy : public Polygon{
    float height;
    float width;

    float ySpeed;
    float xSpeed;

    float speed_up;

    int accelerating;
    bool isMoving;
    int xDirection;
    int yDirection;
    int indexColor;

    float maxXrange;
    float curXTranslation;

    list<Bullet*> firedBullets;

    void renderBullets() {
        if (firedBullets.size()) {
            for (auto bullet : firedBullets) {
                bullet->render();

                if (bullet->getBulletY() > 900) {
                    firedBullets.remove(bullet);
                    delete bullet;
                }
            }
        }
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
        xDirection = 1;
        yDirection = 1;
        curXTranslation = 0;
        xSpeed = INITIAL_SPEED_X;
    }

    virtual void render() override {
        renderBullets();

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

    float getSpeedY() {
        return ySpeed;
    }

    void setSpeedY(float ySpeed) {
        this->ySpeed = ySpeed;
    }

    float getSpeedX() {
        return xSpeed;
    }

    void setSpeedX(float xSpeed) {
        this->xSpeed = xSpeed;
    }

    float getSpeedUp() {
        return speed_up;
    }

    void setSpeedUp(float speed_up) {
        this->speed_up = speed_up;
    }

    void setCurrentXTranslation(float curXTranslation) {
        this->curXTranslation = curXTranslation;
    }

    void move(float ySpeed) {
        if (curXTranslation <= 0) {
            xDirection = 1;
        } else if (curXTranslation >= maxXrange) {
            xDirection = -1;
        }

        if (xDirection == 1) {
            curXTranslation += xSpeed;
        } else {
            curXTranslation -= xSpeed;
        }
        
        translateBy(xSpeed * xDirection, ySpeed);
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

    void shot() {
        Bullet* bullet = new Bullet(5, 10, 10);
        bullet->setYDirection(BULLET_DOWN);
        bullet->fireBullet(vx[0], vy[0] + height);
        firedBullets.push_back(bullet);
    }
};

#endif // ENEMY_H_INCLUDED