#ifndef SPACESHIP_H_INCLUDED
#define SPACESHIP_H_INCLUDED

#include <list>
#include "Polygon.h"
#include "Keyboard.h"
#include "Bullet.h"

using namespace std;


/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class Spaceship : public Polygon{
    float height;
    float base;

    float speed;
    float speed_up;

    float maxSpeed;
    float minSpeed;

    bool isMoving;
    int xDirection;
    int yDirection;

    int ammunitionSize;
    list<Bullet*> ammunition;
    list<Bullet*> firedBullets;

    void reloadAmmunition() {
        for (int i = 0; i < ammunitionSize; i++) {
            ammunition.push_back(new Bullet(5, 10, 10));
        }
    }
public:
    Spaceship(float x, float y, float height, float base) : Polygon(3){
        vx[0] = x - base/2;
        vy[0] = y + height/2;

        vx[1] = x;
        vy[1] = y - height/2;

        vx[2] = x + base/2;
        vy[2] = y + height/2;
        
        this->height = height;
        this->base = base;
        speed = 0;
        speed_up = 0;
        maxSpeed = 10;
        minSpeed = 5;
        colorScale = RGBA;
        isMoving = false;
        xDirection = 0;
        yDirection = 0;
        ammunitionSize = 90;
        reloadAmmunition();
    }

    virtual void render() override {
        if (firedBullets.size()) {
            for (auto bullet : firedBullets) {
                bullet->render();
            }
        }

        if (isMoving) {
            translateBy(xDirection * 10, yDirection * 10);
        }

        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(233.0/265, 15.0/265, 213.0/265, 1);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), nPoints);

    }

    void startMove(int direction) {
        switch (direction) {
            case DOWN:
                yDirection = 1;
            break;
            case UP:
                yDirection = -1;
            break;
            case RIGHT:
                xDirection = 1;
            break;
            case LEFT:
                xDirection = -1;
            break;
        }

        isMoving = true;
    }

    void stopMove(int direction) {
        switch (direction) {
            case DOWN:
                yDirection = 0;
            break;
            case UP:
                yDirection = 0;
            break;
            case RIGHT:
                xDirection = 0;
            break;
            case LEFT:
                xDirection = 0;
            break;
        }

        isMoving = false;
    }

    void shot() {
        if (ammunition.size()) {
            Bullet* bullet = ammunition.front();
            ammunition.pop_front();

            bullet->fireBullet(vx[1], vy[1]);
            firedBullets.push_back(bullet);
        } else {
            reloadAmmunition();
        }
    }

    float getHeight() {
        return height;
    }

    void setHeight(float height) {
        this->height = height;
    }

    float getBase() {
        return base;
    }

    void setBase(float base) {
        this->base = base;
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
};

#endif // SPACESHIP_H_INCLUDED