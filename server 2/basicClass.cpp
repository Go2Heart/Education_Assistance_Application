#include "basicClass.h"

Timer ToTimer(int x) {
    return Timer(x / 60, x % 60);
}

int ToInt(Timer x) {
    return x.Hour() * 60 + x.Min();
}

bool Duration::cross(Duration duration) {
    if (
        begin.Day() == duration.Begin().Day() &&
        !(end.Week() < duration.Begin().Week() || duration.End().Week() < begin.Week()) &&
        !(end < duration.Begin() || duration.End() < begin)
    ) return true;
    else return false;
}
