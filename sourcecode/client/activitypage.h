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
    QVector<QString> info;
public:
    activityInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    QVector<QString> getInfo(){return info;}
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
    QVector<QString> getInfo() {
        return infoWidget->getInfo();
    }
signals:
    void clicked();
};

class activityDetailWidget: public QWidget{
Q_OBJECT
private:
    activityWidget* currentActivity;
    textInputItem* title;
    textInputItem* description;
    textInputItem* place;
    textInputItem* time;
    textInputItem* frequency;
    bool isPersonal = true;
    bool alarm = true;
    QVector<QString> collectMsg();
//    ScrollAreaCustom* materialList;
//    ScrollAreaCustom* homeworkList;
public:
    activityDetailWidget(QWidget* parent);
    void showDetail(QVector<QString> info);
    QVector<QString> getLines() {
        QVector<QString> lines;
        lines.append(title->value());
        lines.append(description->value());
        lines.append(place->value());
        lines.append(time->value());
        return lines;
    }
    void setActivity(activityWidget* activity) {
        currentActivity = activity;
    }
    activityWidget* getActivity() {
        return currentActivity;
    }
signals:
    void deliver(QVector<QString> msg);
    void modify(activityWidget* activity);

};

class ActivityPage : public QWidget{
Q_OBJECT
private:
    QWidget* itemWidget = nullptr;
    QWidget* itemInfo = nullptr;
    QWidget* searchBar = nullptr;
    SlidePage* addActivity;
    ScrollAreaCustom* itemList = nullptr;
    QVector<SlidePage*> pageList;
    activityInfoWidget* activityInfo = nullptr;
    activityDetailWidget* activityDtl = nullptr;
    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    ActivityPage(QWidget* parent = nullptr);
};



class activityListWidget : public QWidget {
Q_OBJECT
private:
    QWidget* slideParent;
    QWidget* titleWidget;
    QLabel* nameLabel;
    QVector<activityAddPage*> pageList;
    QVector<bigIconButton*> extraIcons;
    ScrollAreaCustom* container;

    int overlap = 5, margin = 10, titleHeight = 40, spacing = 3;
    void resizeEvent(QResizeEvent*);
public:
    activityListWidget(QString name, QVector<bigIconButton*> icons, QWidget* p, activityDetailWidget* detailWidget,QWidget* parent = nullptr);
    void addContent(QWidget* p){
        container->addWidget(p, true);
    }
signals:
    void clicked(int id);
    void addPage(activityAddPage*);
    void addReceived(QVector<QString>);
    void showDetail(activityWidget*);
    void newActivity(activityWidget*);
};


#endif // ACTIVITYPAGE_H
