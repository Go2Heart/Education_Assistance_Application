#ifndef CLOCK_H
#define CLOCK_H

#include "basicClass.h"
#include <pthread.h>
#include <unistd.h>

class Clock {
private:
    pthread_t thread;
    double ratio;
    Timer timer;
public:
    Clock();
    Timer NowTimer() { return timer; }
    void addTimer() {
        timer = timer + Timer(0, 1);
        if(timer.Hour() == 24) timer = Timer(0, 0, timer.Day() == 7 ? 1 : timer.Day() + 1, timer.Day() == 7 ? timer.Week() + 1 : timer.Week());
        if(timer.Week() == 21) timer = Timer(timer.Hour(), timer.Min(), timer.Day(), 1);
    }
    void ChgSpd(double x) { if(x - 0 > 1e-5) ratio = x; }
    double Ratio() { return ratio; }
    void Print() { timer.Print(); }
};

#endif