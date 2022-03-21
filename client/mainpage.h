#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "clock.h"

class mainPage : public QWidget {
    Q_OBJECT
private:
    QWidget* toolbar = nullptr;
    QWidget* displayWidget = nullptr;
    Clock* clock = nullptr;
    ScrollAreaCustom* infoContainer = nullptr;
    int cornerRadius = 12;
public:
    mainPage(QWidget* parent = nullptr);
};

#endif // MAINPAGE_H
