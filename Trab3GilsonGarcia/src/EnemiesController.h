#ifndef ENEMIESCONTROLER_H_INCLUDED
#define ENEMIESCONTROLER_H_INCLUDED

#include <list>
#include "stdio.h"
#include "Enemy.h"

using namespace std;

#define SPEED_UP_INCREASE 0.04

/*
##### Teclado #####
Implementa controle do teclado
######################
*/

class EnemiesController{
    list<Enemy*> enemies;

public:
    EnemiesController(int nEnemies) {
        enemies = list<Enemy*>();

        for (int i = 0; i < nEnemies; i++) {
            enemies.push_back(new Enemy(200 + i * 50, -100, 35, 75, 2));
        }
    }

    void render() {
        for (Enemy* enemy : enemies) {
            enemy->render();
        }
    }

    void move(float speed) {
        for (Enemy* enemy : enemies) {
            enemy->move(speed);
        }
    }

    bool checkBulletCollision(Bullet* bullet) {
        for (Enemy* enemy : enemies) {
            bool collided = enemy->checkBulletCollision(bullet);
            if (collided) {
                enemies.remove(enemy);
                delete enemy;
                return true;
            }
        }
    }
};

#endif // ENEMIESCONTROLER_H_INCLUDED