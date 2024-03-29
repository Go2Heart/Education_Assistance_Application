#ifndef CLOCK_H
#define CLOCK_H

#include "basicClass.h"
#include "customWidgets.h"
#include "connect.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>

class Clock : public QWidget
{
    Q_OBJECT
private:
    QLabel* clock;
    QTimer* addTimer;

    qreal defaultSpeed = 1000 / 6.0, ratio = 1;
    Timer nowTime;
public:
    Clock(QWidget *parent = nullptr);
    int Hour() {return nowTime.hour; }
    int Min() {return nowTime.minute; }
    int Day() {return nowTime.day; }
signals:
    void checkAlarm();
public slots:
    void Blocked() { addTimer->stop(); }
    void Released() { addTimer->start(defaultSpeed * ratio); }
};


#endif // CLOCK_H
