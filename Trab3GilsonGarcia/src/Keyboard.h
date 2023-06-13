#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <list>
#include <algorithm>


using namespace std;

#define DEL 127
#define CTRL 214
#define SHIFT 212
#define ESC 27
#define UP 201
#define DOWN 203
#define LEFT 200
#define RIGHT 202
#define SPACE 32


/*
##### Teclado #####
Implementa controle do teclado,
util no controle do jogador, usando
teclas simultaneas.
######################
*/

class Keyboard {
    list<int> pressedKeys;
    list<int> controlPressedKeys; // são as teclas de controle do jogador
    
public:
    Keyboard() {}

    void startPressKey(int key) {
        if (isPressed(key)) return;
        
        pressedKeys.push_back(key);
        if (key == UP || key == DOWN || key == LEFT || key == RIGHT) {
            controlPressedKeys.push_back(key);
        }
    }

    void stopPressKey(int key) {
        if (!isPressed(key)) return;
        
        pressedKeys.remove(key);
        if (key == UP || key == DOWN || key == LEFT || key == RIGHT) {
            controlPressedKeys.remove(key);
        }
    }

    bool isPressed(int key) {
        if (pressedKeys.empty()) return false;

        auto it = find(pressedKeys.begin(), pressedKeys.end(), key);
        if (it != pressedKeys.end()) {
            return true;
        } 

        return false;
    }

    bool isControlKeyPressed() {
        if (controlPressedKeys.empty()) return false;
        return true;
    }

    int getControlKeyPressed() {
        if (controlPressedKeys.empty()) return -1;
        return controlPressedKeys.back();
    }
};

#endif // KEYBOARD_H_INCLUDED