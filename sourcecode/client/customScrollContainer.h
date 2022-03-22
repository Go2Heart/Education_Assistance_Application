#ifndef CUSTOMSCROLLCONTAINER_H
#define CUSTOMSCROLLCONTAINER_H

#include <QWidget>
#include <QColor>
#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>
#include <QtDebug>
#include <QStyleOption>

#define MAXSPEED 70

class ScrollAreaCustom;
class ScrollIndicator;
class ScrollListContainer;

class ScrollAreaCustom : public QWidget
{
    Q_OBJECT
private:
    //high level design:
    //我们希望对轮滑和鼠标创建惯性效果，那么轮滑实际上我们的加速度是恒定的，而且可以设置MAXSPEED，每次wheelevent计算即可
    //但是鼠标的速度是完全随机的，所以我们很难获得加速度，我们需要设计一个计时器来进行一个采样来更新速度
    //这个更新不需要在结束时刻触发，可以在整个鼠标移动过程中触发
    //此外，有了初速度还需要一个减速的过程，这个就是rfrshView计时器的作用
    //这是两个完全独立于整体设计的系统
    //其中rfrshView本质上只跟scrollcontainer槽，release和wheel事件相关
    //getcord只与move相关，同时与updatespd相关
    QTimer *rfrshView;
    QTimer *getCord;

    QPropertyAnimation *bounce;
    //QWidget *tempindicator;

    bool pressed = false;
    bool scrollDown = true;
    bool outofEdge = false;
    bool ignoreMaxSpeed = false;
    bool indicatorEnabled = false;

    int lastY;
    int strtY;
    int bfEdgeY;
    int damp = 1;
    int curSpd = 0;
    int moveStored = 0;
    int nextMove = 1;
    int margin = 10;

    ScrollIndicator *indicator;
    ScrollListContainer *container;

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void bounceback();
public:
    ScrollAreaCustom(bool b, QWidget *parent = nullptr);
    void addWidget(QWidget *widget, bool setAnimation);
    void addWidgets(QVector<QWidget*> widgets) {
        for(int i = 0; i < widgets.size(); i++) addWidget(widgets[i], false);
    }
    void removeWidget(QWidget *widget = nullptr);
    void ScrollToTop();
    void updateHeight();
private slots:
    void scrollContainer();
    void updateSpd();
    void scrollIndicator(int dp);
};

class ScrollListContainer : public QWidget
{
    Q_OBJECT
public:
    ScrollListContainer(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void AddWidget(QWidget *widget, bool setAnimation);
    void AddWidgets(QVector<QWidget*> widgets) {
        for(int i = 0; i < widgets.size(); i++) AddWidget(widgets[i], false);
    }
    void RemoveWidget(QWidget *widget = nullptr);
    void updateHeight();
    void clear();
private:
    int spacing = 3;
    QVector<QWidget*> widgets;
    int size = 0;
    QVector<int> ys;
};

class ScrollIndicator : public QWidget
{
    Q_OBJECT
private:
    QColor curColor;
    QColor defaultColor = QColor(100, 100, 100, 130);
    QColor hoverColor = QColor(70, 70, 70, 150);
    QColor pressColor = QColor(50, 50, 50, 170);

    bool pressed = false;
    int lastY;
public:
    ScrollIndicator(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void scrollPage(int);
};

#endif // CUSTOMSCROLLCONTAINER_H
