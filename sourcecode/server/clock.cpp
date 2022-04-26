#include "clock.h"
#include "graph.h"
#include "global.h"

void* func(void* p) {
    while(1) {
        usleep(100000 * (1 / timeTracker.Ratio()));
        timeTracker.addTimer();
        graph.UpdGraph(timeTracker.NowTimer());
        //timeTracker.Print();
    }
}

Clock::Clock() {
    timer = Timer(8, 0, 1, 1);
    ratio = 1;
    pthread_create(&thread, NULL, func, NULL);
}

