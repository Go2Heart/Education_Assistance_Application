#include "basicClass.h"

Timer ToTimer(int x) {
    return Timer(x / 60, x % 60);
}

int ToInt(Timer x) {
    return x.Hour() * 60 + x.Min();
}

bool Duration::cross(Duration duration) {
    if (
        begin.day == duration.begin.day &&
        !(end.week < duration.begin.week || duration.end.week < begin.week) &&
        !(end.HMLess(duration.begin) || duration.end.HMLess(begin))
    ) return true;
    else return false;
}
