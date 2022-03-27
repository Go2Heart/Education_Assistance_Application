#include "clock.h"

Clock::Clock(QWidget *parent) :
    QWidget(parent),
    nowTime(Timer(8, 0, 1, 1))
{
    //setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout* clockLayout = new QHBoxLayout(this);
    setLayout(clockLayout);
    clockLayout->setContentsMargins(0, 0, 0, 0);
    clockLayout->setSpacing(30);
    clockLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QFont textFont = QFont("Corbel", 20);
    QFontMetrics fm(textFont);
    clock = new QLabel(this);
    clock->setFont(textFont);
    clock->setWordWrap(true);
    clock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clock->setFixedSize(QSize(250, fm.lineSpacing()));
    clock->setAlignment(Qt::AlignCenter);
    clock->setText(QString::asprintf("Week:%d,Day:%d,%02d:%02d", nowTime.Week(), nowTime.Day(), nowTime.Hour(), nowTime.Min()));
    clockLayout->addWidget(clock);

    horizontalValueAdjuster* radioAdjuster = new horizontalValueAdjuster("clock speed", 0.1, 10, 0.1, this);
    radioAdjuster->setValue(1.0);
    connect(radioAdjuster,
        &horizontalValueAdjuster::valueChanged,
        this,
        [=](qreal ratio) {
            qDebug() << ratio;
            ChgRatio(ratio);
    });
    clockLayout->addWidget(radioAdjuster);

    addTimer = new QTimer;
    addTimer->setSingleShot(true);
    addTimer->start((1.0 / ratio) * defaultSpeed);
    connect(addTimer,
        &QTimer::timeout,this, [=] { Add(); addTimer->start((1.0 / ratio) * defaultSpeed); });
}

void Clock::Add() {
    nowTime = nowTime + Timer(0, 1);
    //qDebug()<<nowTime.Week()<<"    "<<nowTime.Day()<<"    "<<nowTime.Hour()<<"   "<<nowTime.Minute();
    if(nowTime.Hour() >= 18 && nowTime.Min() > 10)
        nowTime = Timer(8, 0, nowTime.Week() + (nowTime.Day() == 7), nowTime.Day() == 7 ? 1 : nowTime.Day() + 1);
    clock->setText(QString::asprintf("Week:%d,Day:%d,%02d:%02d", nowTime.Week(), nowTime.Day(), nowTime.Hour(), nowTime.Min()));
}

void Clock::ChgRatio(qreal ratio) {
    this->ratio = ratio;
}
