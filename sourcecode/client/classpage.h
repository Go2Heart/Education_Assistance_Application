//
// Created by Go2Heart on 2022/4/27.
//

#ifndef CLIENT_CLASSPAGE_H
#define CLIENT_CLASSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "clock.h"
class classInfoWidget : public QWidget{
Q_OBJECT
private:
    int margin = 5, spacing = 5;
    QWidget* infoWidget;
    QLabel* descLabel;
    QLabel* detailLabel;
    bigIconButton* classType;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
public:
    classInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info);
signals:
    void clicked();
};


class ClassPage : public QWidget{
Q_OBJECT
private:
    QWidget* itemWidget = nullptr;
    QWidget* itemInfo = nullptr;
    QWidget* searchBar = nullptr;
    SlidePage* addclass;
    ScrollAreaCustom* itemList = nullptr;
    QVector<SlidePage*> pageList;
    classInfoWidget* classInfo = nullptr;
    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    ClassPage(QWidget* parent = nullptr);
};

class classWidget : public QWidget {
Q_OBJECT
private:
    QWidget* bgWidget;
    classInfoWidget* infoWidget;
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
    classWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info) {
        infoWidget->modify(info);
    }
signals:
    void clicked();
};

class classListWidget : public QWidget {
Q_OBJECT
private:
    QWidget* slideParent;
    QWidget* titleWidget;
    QLabel* nameLabel;
    QVector<bigIconButton*> extraIcons;
    ScrollAreaCustom* container;

    int overlap = 5, margin = 10, titleHeight = 40, maxHeight, spacing = 3;
public:
    classListWidget(QString name, int h, QWidget* p, QWidget* parent = nullptr);
    void addContent(QWidget* p){
        container->addWidget(p, true);
    }
    void resizeEvent(QResizeEvent*);
signals:
    void clicked(int id);
};


#endif //CLIENT_CLASSPAGE_H
