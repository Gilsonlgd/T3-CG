#ifndef ENDSCREEN_H_INCLUDED
#define ENDSCREEN_H_INCLUDED

#include <stdio.h>
#include <list>
#include "gl_canvas2d.h"
#include "math_utils.h"
#include "Botao.h"
#include <string>

#define EXIT_GAME 2
#define BUTTON_WIDTH 300.0
#define BUTTON_HEIGHT 50.0
#define TITLE_COLOR 251.0/255, 127.0/255, 87.0/255, 1
#define BACKGROUND_COLOR 19.0/255, 5.0/255, 33.0/255, 1
#define BUTTONS_COLLOR 185, 11,130, 1

using namespace std;

class EndScreen {
    float screenWidth;
    float screenHeight;
    float finalScore;

    bool visible;

    char* title;
    Botao* exit;

public:
    EndScreen(float screenWidth, float screenHeight, char* title) {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        finalScore = 0;

        this->title = title;
        visible = true;
        exit = new Botao(screenWidth / 2 - BUTTON_WIDTH / 2, screenHeight / 2 - BUTTON_HEIGHT*1.5, BUTTON_WIDTH, BUTTON_HEIGHT, "Exit", RGBA);

        exit->setColor(BUTTONS_COLLOR);
    }
        
    void render() {
        if (visible) {
            CV::color(BACKGROUND_COLOR);
            CV::rectFill(0, 0, screenWidth, screenHeight);

            CV::color(TITLE_COLOR);
            CV::text(screenWidth / 2 - 45, 200, title);

            string finalScoreText = "Your Final Score: " + to_string((int)finalScore);
            CV::text(screenWidth / 2 - 90, 250, finalScoreText.c_str());

            exit->render();
        }
    }

    int checkButtonsClick(float x, float y) {
        if (exit->hasCollided(x, y)) {
            return EXIT_GAME;
        }

        return 0;
    }

    void setFinalScore(float finalScore) {
        this->finalScore = finalScore;
    }
};

#endif // ENDSCREEN_H_INCLUDED