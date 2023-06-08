#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

class FPSControl {
    int maxFrameRate;
    double actualFrameRate;
    chrono::milliseconds frameDelay;
    chrono::steady_clock::time_point priorTime;
    chrono::steady_clock::time_point presentTime;
public:
    FPSControl(int maxFrameRate_, chrono::steady_clock::time_point priorTime_) {
        maxFrameRate = maxFrameRate_;
        priorTime = priorTime_;
        frameDelay = chrono::milliseconds(1000 / maxFrameRate);
    }

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
        actualFrameRate = 1.0f / elapsedSeconds;
    }

    float getActualFrameRate() {
        return actualFrameRate;
    }
};