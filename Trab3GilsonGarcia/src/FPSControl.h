#include <iostream>
#include <chrono>
#include <thread>

#define TIME_BETWEEN_FPS_CAPTURE 250

using namespace std;

/*
##### FPSCONTROL #####
Implementa o controle de FPS,
como limitar FPS max, calcular 
FPS atual e calcular deltaTime
######################
*/


class FPSControl {
    int maxFrameRate;
    double actualFrameRate;

    chrono::milliseconds frameDelay;
    chrono::milliseconds deltaTime;
    chrono::steady_clock::time_point priorTime;
    chrono::steady_clock::time_point presentTime;
    chrono::steady_clock::time_point lastFrameRateCapture;
public:
    FPSControl(int maxFrameRate_, chrono::steady_clock::time_point priorTime_) {
        maxFrameRate = maxFrameRate_;
        priorTime = priorTime_;
        frameDelay = chrono::milliseconds(1000 / maxFrameRate);
    }

    // limita o FPS maximo
    void limitRefreshRate() {
        presentTime = chrono::steady_clock::now();
        chrono::milliseconds elapsedTime = chrono::duration_cast<chrono::milliseconds>(presentTime - priorTime);

        if (elapsedTime < frameDelay) {
            this_thread::sleep_for(frameDelay - elapsedTime);
            presentTime = chrono::steady_clock::now();  
            elapsedTime = chrono::duration_cast<chrono::milliseconds>(presentTime - priorTime); 
        }

        priorTime = presentTime; 

        float elapsedSeconds = elapsedTime.count() / 1000.0f;
        deltaTime = elapsedTime;

        chrono::milliseconds elapsedTimeBetweenFPSCapture = chrono::duration_cast<chrono::milliseconds>(presentTime - lastFrameRateCapture);
        chrono::milliseconds timeBetweenFPSCapture = chrono::milliseconds(TIME_BETWEEN_FPS_CAPTURE);
        
        if (elapsedTimeBetweenFPSCapture > timeBetweenFPSCapture) {
            actualFrameRate = 1.0f / elapsedSeconds;
            lastFrameRateCapture = presentTime;
        }
    }

    float getActualFrameRate() {
        return actualFrameRate;
    }

    // retorna o deltaTime, valor usado para calcular a velocidade dos objetos
    // em relação ao fps
    float getDeltaTime() {
        return (float)deltaTime.count() / 16.0f;
    }
    
};