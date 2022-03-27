#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "specifiedWidgets.h"
#include "clock.h"

class mainPage : public QWidget {
    Q_OBJECT
private:
    QWidget* toolbar = nullptr;
    QWidget* displayWidget = nullptr;
    QTimer* clickpressTimer;
    QTimer* clickreleaseTimer;
    SlidePage* userInfo;
    Clock* clock = nullptr;
    ScrollAreaCustom* infoContainer = nullptr;
    QVector<SlidePage*> slidePageList;
    QVector<clockAddPage*> clockPageList;
    int cornerRadius = 12;
    bool stateChanged = false;
    void resizeEvent(QResizeEvent*);
public:
    mainPage(QWidget* parent = nullptr);
    void raisePage() {
        displayWidget->raise();
        clickpressTimer->start(1000);
    }
};

#endif // MAINPAGE_H
