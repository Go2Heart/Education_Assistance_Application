#ifndef BASICCLASS_H
#define BASICCLASS_H

class Timer {
private:
    int week = 0, day = 0, hour = 0, minute = 0;
public:
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
        return (hour < t.hour || (hour == t.hour && minute < t.minute));
    }
    bool operator <= (const Timer &t) {
        return (hour < t.hour || (hour == t.hour && minute <= t.minute));
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
};

extern Timer ToTimer(int x);
extern int ToInt(Timer x);

class Duration {
private:
    Timer begin, end;
public:
    Duration(Timer begin, Timer end) :
        begin(begin),
        end(end)
    {}
    Duration() :
        begin(Timer(0, 0)),
        end(Timer(0, 0))
    {}
    bool cross(Duration duration);
    Timer Begin() { return begin; }
    Timer End() { return end; }
};
#endif // BASICCLASS_H
