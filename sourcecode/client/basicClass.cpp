#include "basicClass.h"

Timer UnzipTimer(int x) {
    int week, day, hour, minute;
    minute = x % 256;
    x >>= 8;
    hour = x % 256;
    x >>= 8;
    day = x % 256;
    x >>= 8;
    week = x % 256;
    return Timer(hour, minute, day, week);
}
