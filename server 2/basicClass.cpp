#include "basicClass.h"

bool Duration::cross(Duration duration) {
    if(begin.Day() != duration.Begin().Day()) return false;
    if(end < duration.Begin() || duration.End() < begin) return false;
    return true;
}
