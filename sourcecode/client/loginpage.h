#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "customWidgets.h"

class loginPage : public QWidget
{
    Q_OBJECT
private:
    QColor splitColor = QColor(255, 145, 0);
    QTimer* unsetTimer;
    int splitWidth = 4;
    int margin = 6;
    void enterEvent(QEvent*) {
        //unsetTimer->stop();
        //unsetTimer->start(20);
    }
    void paintEvent(QPaintEvent* event);
    //void resizeEvent(QResizeEvent* event);
public:
    loginPage(QWidget *parent = nullptr);
signals:
    void logined();
};

#endif // LOGINPAGE_H
