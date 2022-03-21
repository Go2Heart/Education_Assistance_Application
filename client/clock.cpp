#include "clock.h"

Clock::Clock(QWidget *parent) :
    QLabel(parent),
    nowTime(Timer(8, 0, 1, 1))
{
    //qDebug()<<nowTime.Week()<<"    "<<nowTime.Day()<<"    "<<nowTime.Hour()<<"   "<<nowTime.Minute();
    QFont textFont = QFont("Corbel", 20);
    QFontMetrics fm(textFont);
    setFont(textFont);
    setWordWrap(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedSize(QSize(250, fm.lineSpacing()));
    setAlignment(Qt::AlignCenter);
    addTimer = new QTimer;
    addTimer->setSingleShot(true);
    addTimer->start(ratio * defaultSpeed);
    setText(QString::asprintf("Week:%d,Day:%d,%02d:%02d", nowTime.Week(), nowTime.Day(), nowTime.Hour(), nowTime.Min()));
    connect(addTimer,
        &QTimer::timeout,this, [=] { Add(); addTimer->start(ratio * defaultSpeed); });
}

void Clock::Add() {
    nowTime = nowTime + Timer(0, 1);
    //qDebug()<<nowTime.Week()<<"    "<<nowTime.Day()<<"    "<<nowTime.Hour()<<"   "<<nowTime.Minute();
    if(nowTime.Hour() >= 18 && nowTime.Min() > 10)
        nowTime = Timer(8, 0, nowTime.Week() + (nowTime.Day() == 7), nowTime.Day() == 7 ? 1 : nowTime.Day() + 1);
    setText(QString::asprintf("Week:%d,Day:%d,%02d:%02d", nowTime.Week(), nowTime.Day(), nowTime.Hour(), nowTime.Min()));
}

void Clock::ChgRatio(qreal ratio) {
    this->ratio = ratio;
}
