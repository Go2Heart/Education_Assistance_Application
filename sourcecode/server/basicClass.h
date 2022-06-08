#ifndef BASICCLASS_H
#define BASICCLASS_H

#include <cstdio>

class Timer {
public:
    int week = 0, day = 0, hour = 0, minute = 0;
    /*enum {
        Mon = 1, Tue = 2, Wed = 3, Thu = 4, Fri = 5, Sat = 6, Sun = 7
    };*/
    Timer(int hour, int minute, int day = 0, int week = 0) :
        week(week),
        day(day),
        hour(hour),
        minute(minute)
    {}
    Timer() {}
    bool operator < (const Timer &t) {
        return (week < t.week ||
            (week == t.week && day < t.day) ||
            (week == t.week && day == t.day && hour < t.hour) ||
            (week == t.week && day == t.day && hour == t.hour && minute < t.minute));
    }
    bool operator <= (const Timer &t) {
        return (week < t.week ||
            (week == t.week && day < t.day) ||
            (week == t.week && day == t.day && hour < t.hour) ||
            (week == t.week && day == t.day && hour == t.hour && minute <= t.minute));
    }
    bool HMLess(const Timer &t) {
        return hour < t.hour || (hour == t.hour && minute < t.minute);
    }
    bool HMLessEqual(const Timer &t) {
        return hour < t.hour || (hour == t.hour && minute <= t.minute);
    }
    Timer operator + (const Timer &t) {
        return Timer(hour + t.hour + (minute + t.minute >= 60), (minute + t.minute) % 60, day + t.day, week + t.week);
    }
    friend Timer operator - (Timer a, Timer b) {
        return Timer(a.hour - b.hour - (a.minute < b.minute), (a.minute + 60 - b.minute) % 60);
    }
    int Week() { return week; }
    int Day() { return day; }
    int Hour() { return hour; }
    int Min() { return minute; }
    int Zip() {
        return week * (1 << 24) + day * (1 << 16) + hour * (1 << 8) + minute;  
    }
    void Print() { printf("Week: %d Day: %d Hour : %d Minute : %d\n", week, day, hour, minute); }
};

extern Timer ToTimer(int x);
extern int ToInt(Timer x);

class Duration {
public:
    Timer begin, end;
    Duration(Timer begin, Timer end) :
        begin(begin),
        end(end)
    {}
    Duration() :
        begin(Timer(0, 0)),
        end(Timer(0, 0))
    {}
    bool cross(Duration duration);
};
#endif // BASICCLASS_H
