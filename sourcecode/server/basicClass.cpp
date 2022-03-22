#include "basicClass.h"

Timer ToTimer(int x) {
    return Timer(x / 60, x % 60);
}

int ToInt(Timer x) {
    return x.Hour() * 60 + x.Min();
}

bool Duration::cross(Duration duration) {
    if (
        begin.Day() == duration.begin.Day() &&
        !(end.Week() < duration.begin.Week() || duration.end.Week() < begin.Week()) &&
        !(end < duration.begin || duration.end < begin)
    ) return true;
    else return false;
}
