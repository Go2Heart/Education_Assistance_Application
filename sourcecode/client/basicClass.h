#ifndef BASICCLASS_H
#define BASICCLASS_H

#include <QDebug>

class Timer {
public:
    int week = 0, day = 0, hour = 0, minute = 0;
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
    int Zip() {
        return week * (1 << 24) + day * (1 << 16) + hour * (1 << 8) + minute;
    }
    QString ToString() { return QString::asprintf("%02d: %02d", hour, minute); }
    void Print() {
        qDebug() << week << day << hour << minute;
    }

};

extern Timer UnzipTimer(int x);

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
