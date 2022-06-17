#ifndef BASICCLASS_H
#define BASICCLASS_H

#include <cstdio>

class Timer {
public:
    int week = 0, day = 0, hour = 0, minute = 0;
    /*enum {
        Mon = 1, Tue = 2, Wed = 3, Thu = 4, Fri = 5, Sat = 6, Sun = 7
    };*/
    Timer(int now_hour, int now_min, int now_day = 0, int now_week = 0) :
        week(now_week),
        day(now_day),
        hour(now_hour),
        minute(now_min)
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
        int nowmin = minute + t.minute;
        int nowhour = hour + t.hour;
        int nowday = day + t.day;
        int nowweek = week + t.week;
        if(nowmin >= 60) nowmin -= 60, nowhour++;
        if(nowhour >= 24) nowhour -= 24, nowday++;
        if(nowday > 7) nowday -= 7, nowweek++;
        return Timer(nowhour, nowmin, nowday, nowweek);
    }
    friend Timer operator - (Timer a, Timer b) {
        return Timer(a.hour - b.hour - (a.minute < b.minute), (a.minute + 60 - b.minute) % 60);
    }
    int Zip() {
        return week * (1 << 24) + day * (1 << 16) + hour * (1 << 8) + minute;  
    }
    void Print() { printf("Week: %d Day: %d Hour : %d Minute : %d\n", week, day, hour, minute); }
};

extern Timer ToTimer(int x);
extern Timer UnzipTimer(int x);
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
