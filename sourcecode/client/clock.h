#ifndef CLOCK_H
#define CLOCK_H

#include "basicClass.h"
#include <pthread.h>
#include <unistd.h>
extern bool ClockStatus;
class Clock {
private:
    pthread_t thread;
    double ratio;
    Timer timer;
public:
    Clock();
    void restart();
    void stop();
    Timer NowTimer() { return timer; }
    void addTimer();
    void ChgSpd(double x) { if(x - 0 > 1e-5) ratio = x; }
    double Ratio() { return ratio; }
    void Print() { timer.Print(); }
};

#endif