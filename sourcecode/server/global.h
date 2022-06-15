#ifndef GLOBAL_H
#define GLOBAL_H
#include <cstdio>
extern class Students studentGroup;
extern class Teachers teacherGroup;
extern class Activities activityGroup;
extern class Alarms alarmGroup;
extern class Lessons lessonGroup;
extern class Graph graph;
extern class Encoder encodeSolver;
extern class Decoder decodeSolver;
extern class Clock timeTracker;
extern class TimeHeap timeHeap;
extern FILE* logFile;
class Timer;
extern Timer beg[20];
extern Timer fin[20];
int ToBegNum(Timer x);
int ToEndNum(Timer x);
int GenerateAlarmId();
#endif