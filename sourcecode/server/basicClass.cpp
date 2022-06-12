#include "basicClass.h"

Timer ToTimer(int x) {
    return Timer(x / 60, x % 60);
}

int ToInt(Timer x) {
    return x.hour * 60 + x.minute;
}

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

bool Duration::cross(Duration duration) {
    if (
        begin.day == duration.begin.day &&
        !(end.week < duration.begin.week || duration.end.week < begin.week) &&
        !(end.HMLess(duration.begin) || duration.end.HMLess(begin))
    ) return true;
    else return false;
}
