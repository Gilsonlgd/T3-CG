#ifndef HOMESCREEN_H_INCLUDED
#define HOMESCREEN_H_INCLUDED

#include <stdio.h>
#include <list>
#include "gl_canvas2d.h"
#include "math_utils.h"
#include "Botao.h"

#define START_GAME 1
#define EXIT_GAME 2
#define BUTTON_WIDTH 300.0
#define BUTTON_HEIGHT 50.0
#define TITLE_COLOR 251.0/255, 127.0/255, 87.0/255, 1
#define BACKGROUND_COLOR 19.0/255, 5.0/255, 33.0/255, 1
#define BUTTONS_COLLOR 185, 11,130, 1

using namespace std;

/*
##### HOMESCREEN #####
Implementa a tela inicial, 
com os botões de start e exit
######################
*/


class HomeScreen {
    float screenWidth;
    float screenHeight;

    bool visible;

    char* title;
    Botao* start;
    Botao* exit;

public:
    HomeScreen(float screenWidth, float screenHeight, char* title) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        this->title = title;
        visible = true;
        start = new Botao(screenWidth / 2 - BUTTON_WIDTH / 2, screenHeight / 2 - BUTTON_HEIGHT*2, BUTTON_WIDTH, BUTTON_HEIGHT, "Start", RGBA);
        exit = new Botao(screenWidth / 2 - BUTTON_WIDTH / 2, screenHeight / 2, BUTTON_WIDTH, BUTTON_HEIGHT, "Exit", RGBA);

        start->setColor(BUTTONS_COLLOR);
        exit->setColor(BUTTONS_COLLOR);
    }
        

    void render() {
        if (visible) {
            CV::color(BACKGROUND_COLOR);
            CV::rectFill(0, 0, screenWidth, screenHeight);

            CV::color(TITLE_COLOR);
            CV::text(screenWidth / 2 - 65, 200, title);

            start->render();
            exit->render();
        }
    }

    int checkButtonsClick(float x, float y) {
        if (start->hasCollided(x, y)) {
            visible = false;
            return START_GAME;
        } else if (exit->hasCollided(x, y)) {
            return EXIT_GAME;
        }

        return 0;
    }

    
};

#endif // HOMESCREEN_H_INCLUDED