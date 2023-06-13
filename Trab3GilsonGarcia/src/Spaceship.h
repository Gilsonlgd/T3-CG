#ifndef SPACESHIP_H_INCLUDED
#define SPACESHIP_H_INCLUDED

#include <list>
#include <chrono>
#include "Polygon.h"
#include "Keyboard.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

#define TIME_INVULNERABLE 3000
#define SPEED_UP_INCREASE 0.04
#define SPACESHIP_COLOR 233.0/265, 15.0/265, 213.0/265

/*
##### Teclado #####
Implementa a nave do jogador.
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

    int nLifes;
    bool invulnerable;
    chrono::steady_clock::time_point lastHitTime;

    bool canMoveToLeft;
    bool canMoveToRight;

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
        nLifes = 5;
        invulnerable = false;
        canMoveToLeft = true;
        canMoveToRight = true;
        reloadAmmunition();
    }

    virtual void render() override {
        renderBullets();

        CV::translate(0, 0);

        if (invulnerable) CV::color(1, 1, 1, 1);
        else if (colorScale == RGBA) CV::color(233.0/265, 15.0/265, 0.0/265, 1);
        else if (colorScale == INDEX14)  CV::color(indexColor);

        CV::polygonFill(vx.data(), vy.data(), nPoints);

    }

    void move(float deltaTime) {
        handleSpeedUp();
        moveBullets(deltaTime);

        if (isMoving) {
            if (xDirection > 0  && canMoveToRight) {
                translateBy(xDirection * 6 * deltaTime, 0);
            } else if (xDirection < 0 && canMoveToLeft) {
                translateBy(xDirection * 6 * deltaTime, 0);
            }
        }

        if (invulnerable) {
            chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
            chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(currentTime - lastHitTime);
            if (time_span.count() * 1000 > TIME_INVULNERABLE) {
                invulnerable = false;
            }
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

    bool checkEnemyCollision(Enemy* enemy) {
        float enemyHeight = enemy->getHeight();
        float enemyWidth = enemy->getWidth();
        
        if (enemy->getY() + enemyHeight < vy[1]) {
            return false;
        }

        if (enemy->getX() + enemyWidth < vx[0]) {
            return false;
        }

        if (enemy->getX() - enemyWidth > vx[2]) {
            return false;
        }

        return hasPolygonCollided(enemy->getVx(), enemy->getVy());
    }

    void setInvulnerable() {
        this->invulnerable = true;
        lastHitTime = chrono::steady_clock::now();
    }

    bool isInvulnerable() {
        return invulnerable;
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

    int getNLifes() {
        return nLifes;
    }

    void decNLifes() {
        nLifes--;
    }

    void incNLifes() {
        nLifes++;
    }

    void setCanMoveToLeft(bool canMoveToLeft) {
        this->canMoveToLeft = canMoveToLeft;
    }

    void setCanMoveToRight(bool canMoveToRight) {
        this->canMoveToRight = canMoveToRight;
    }

    list<Bullet*> getShots() {
        return firedBullets;
    }

    void removeShot(Bullet* bullet) {
        firedBullets.remove(bullet);
        delete bullet;
    }

    void translateBackwardsFromCollisionPointL(float collisionPointX) {
        translateBy(collisionPointX - vx[0], 0);
    }

    void translateBackwardsFromCollisionPointR(float collisionPointX) {
        translateBy(collisionPointX - vx[2], 0);
    }
};

#endif // SPACESHIP_H_INCLUDED