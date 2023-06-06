#ifndef HOMESCREEN_H_INCLUDED
#define HOMESCREEN_H_INCLUDED

#include <stdio.h>
#include <list>
#include "gl_canvas2d.h"
#include "math_utils.h"
#include "Botao.h"

#define START_GAME 1
#define EXIT_GAME 2

using namespace std;

class HomeScreen {
    float screenWidth;
    float screenHeight;

    int backgroundColor;
    bool visible;

    char* title;
    Botao* start;
    Botao* exit;

public:
    HomeScreen(float screenWidth, float screenHeight, int backgroundColor, char* title) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        this->title = title;
        visible = true;
        start = new Botao(screenWidth / 2, screenHeight / 2, 100, 50, "Start", INDEX14);
        exit = new Botao(screenWidth / 2, screenHeight / 2 - 100, 100, 50, "Exit", INDEX14);
        this->backgroundColor = backgroundColor;
    }
        

    void render() {
        if (visible) {
            CV::color(backgroundColor);
            CV::rectFill(0, 0, screenWidth, screenHeight);

            CV::color(INDEX14);
            CV::text(screenWidth / 2 - 50, screenHeight / 2 + 100, title);

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