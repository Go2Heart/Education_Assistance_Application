#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QTimer>
#include "loginpage.h"
#include "mainpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTimer* unsetTimer;
    bool forceState = false;

    int cornerRadius = 10, verticalMargin = 30, horizontalMargin = 15;
    QWidget *border = nullptr;
    QGraphicsDropShadowEffect *windowShadow;
    QColor mainBackGround = QColor(251, 251, 251);

    topButton* adjBtn = nullptr;
    topButton* closeBtn = nullptr;
    topButton* miniBtn = nullptr;
    loginPage* loginpage = nullptr;
    mainPage* mainpage = nullptr;
    void Init();

    enum {
        AT_LEFT = 1, AT_TOP = 2, AT_RIGHT = 4, AT_BOTTOM = 8,
        AT_TOP_LEFT = 3, AT_TOP_RIGHT = 6, AT_BOTTOM_LEFT = 9, AT_BOTTOM_RIGHT = 12,
    };
    bool mousePressed = false;
    int mouseState = 0;
    QPoint lastPos;
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event) {
        mousePressed = false;
        forceState = false;
        if(event->globalPos().y() < 2) controlWindowScale();
    }
    void resizeEvent(QResizeEvent*);

    bool maximized = false;
    QRect lastGeometry;
    void controlWindowScale();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
