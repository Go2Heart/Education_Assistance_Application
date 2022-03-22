#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "clock.h"

class mainPage : public QWidget {
    Q_OBJECT
private:
    QWidget* toolbar = nullptr;
    QWidget* displayWidget = nullptr;
    SlidePage* userInfo;
    Clock* clock = nullptr;
    ScrollAreaCustom* infoContainer = nullptr;
    QVector<SlidePage*> pageList;
    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    mainPage(QWidget* parent = nullptr);
};

#endif // MAINPAGE_H
