#include <chrono>
#include <thread>

using namespace std;

class FPSControl {
    int maxFrameRate;
    double actualFrameRate;
    chrono::milliseconds waitingTime;
    chrono::steady_clock::time_point priorTime;
    chrono::steady_clock::time_point presentTime;
public:
    FPSControl(int maxFrameRate_, chrono::steady_clock::time_point priorTime_) {
        maxFrameRate = maxFrameRate_;
        priorTime = priorTime_;
        waitingTime = chrono::milliseconds(1000 / maxFrameRate);
    }

    void limitRefreshRate() {
        presentTime = chrono::steady_clock::now();
        chrono::milliseconds elapsedTime = chrono::duration_cast<chrono::milliseconds>(presentTime - priorTime);

        if (elapsedTime < waitingTime) {
            this_thread::sleep_for(waitingTime - elapsedTime);
        }

        priorTime = chrono::steady_clock::now();

        double elapsedSeconds = waitingTime.count() / 1000.0;
        actualFrameRate = 1.0 / elapsedSeconds; 
    }

    int getActualFrameRate() {
        return (int)actualFrameRate;
    }
};