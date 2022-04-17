#ifndef ACTIVITYPAGE_H
#define ACTIVITYPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "clock.h"
class activityInfoWidget : public QWidget{
    Q_OBJECT
private:
    int margin = 5, spacing = 5;
    QWidget* infoWidget;
    QLabel* descLabel;
    QLabel* detailLabel;
    bigIconButton* activityType;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
public:
    activityInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info);
signals:
    void clicked();
};

class activityAddPage : public SlidePage {
    Q_OBJECT
private:
    textInputItem* title;
    textInputItem* description;
    textInputItem* place;
    textInputItem* time;
    QWidget* activityBar;
    textInputItem* frequency;
    bool isPersonal = true;
    bool alarm = true;
    bool created = false;
    QVector<QString> collectMsg();
public:
    activityAddPage(int radius, int type, int width, int height, QString name, QWidget *parent = nullptr, int posy = 0);
signals:
    void deliver(QVector<QString> msg);
    void modify(QVector<QString> msg);
};

class ActivityPage : public QWidget{
    Q_OBJECT
private:
    QWidget* itemWidget = nullptr;
    QWidget* itemInfo = nullptr;
    SlidePage* addActivity;
    ScrollAreaCustom* itemList = nullptr;
    QVector<SlidePage*> pageList;
    activityInfoWidget* activityInfo = nullptr;
    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    ActivityPage(QWidget* parent = nullptr);
};

class activityWidget : public QWidget {
    Q_OBJECT
private:
    QWidget* bgWidget;
    activityInfoWidget* infoWidget;
    QString defaultColor = "#0a0078d4";
    QString hoverColor = "#1a0078d4";
    QString pressedColor = "#2a0078d4";
    void enterEvent(QEvent*) {
        bgWidget->setStyleSheet("background-color:" + hoverColor + ";border-radius:12px;");
    }
    void leaveEvent(QEvent*) {
        bgWidget->setStyleSheet("background-color:" + defaultColor + ";border-radius:12px;");
    }
    void resizeEvent(QResizeEvent*);
public:
    activityWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info) {
        infoWidget->modify(info);
    }
signals:
    void clicked();
};

class activityfoldWidget : public foldWidget {
    Q_OBJECT
private:
    QWidget* slideParent;
    QVector<activityAddPage*> pageList;
public:
    activityfoldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* p, QWidget* parent = nullptr);
signals:
    void addPage(activityAddPage*);
};

#endif // ACTIVITYPAGE_H
