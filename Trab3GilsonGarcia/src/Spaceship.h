#ifndef SPACESHIP_H_INCLUDED
#define SPACESHIP_H_INCLUDED

#include <list>
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

class Spaceship : public Polygon{
    float height;
    float base;

    float speed;
    float speed_up;

    float maxSpeed;
    float minSpeed;

    int accelerating;
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

    void moveBullets(float deltaTime) {
        if (firedBullets.size()) {
            for (auto bullet : firedBullets) {
                bullet->move(deltaTime);

                if (bullet->getBulletY() < -10) {
                    firedBullets.remove(bullet);
                    delete bullet;
                }
            }
        }
    }

    void handleSpeedUp() {
        if (accelerating != 0) {
            if (speed < maxSpeed && accelerating == 1) {
                speed += SPEED_UP_INCREASE;
            } else if (speed > minSpeed && accelerating == -1) {
                speed -= SPEED_UP_INCREASE;
            }

            if (speed > maxSpeed) speed = maxSpeed;
            else if (speed < minSpeed) speed = minSpeed;
        }
    }
public:
    Spaceship(float x, float y, float height, float base, float maxSpeed, float minSpeed) : Polygon(3){
        vx[0] = x - base/2;
        vy[0] = y + height/2;

        vx[1] = x;
        vy[1] = y - height/2;

        vx[2] = x + base/2;
        vy[2] = y + height/2;
        
        this->height = height;
        this->base = base;
        speed = minSpeed;
        speed_up = 0;
        this->maxSpeed = maxSpeed;
        this->minSpeed = minSpeed;
        accelerating = 0;
        colorScale = RGBA;
        isMoving = false;
        xDirection = 0;
        yDirection = 0;
        ammunitionSize = 90;
        reloadAmmunition();
    }

    virtual void render() override {
        renderBullets();

        CV::translate(0, 0);
        if (colorScale == RGBA) CV::color(233.0/265, 15.0/265, 213.0/265, 1);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), nPoints);

    }

    void move(float deltaTime) {
        handleSpeedUp();
        moveBullets(deltaTime);
        if (isMoving) {
            translateBy(xDirection * 4 * deltaTime, 0);
        }
    }

    void startMove(int direction) {
        switch (direction) {
            case DOWN:
                accelerating = -1;
            break;
            case UP:
                accelerating = 1;
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
                accelerating = 0;
            break;
            case UP:
                accelerating = 0;
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

    bool checkBulletCollision(Bullet* bullet) {
        float bulletHeight = bullet->getHeight();
        float bulletWidth = bullet->getWidth();

        if (bullet->getBulletY() + bulletHeight < vy[1]) {
            return false;
        }

        if (bullet->getBulletX() + bulletWidth < vx[0]) {
            return false;
        }

        if (bullet->getBulletX() > vx[2]) {
            return false;
        }

        return hasPolygonCollided(bullet->getVx(), bullet->getVy());
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

    list<Bullet*> getShots() {
        return firedBullets;
    }

    void removeShot(Bullet* bullet) {
        firedBullets.remove(bullet);
        delete bullet;
    }
};

#endif // SPACESHIP_H_INCLUDED