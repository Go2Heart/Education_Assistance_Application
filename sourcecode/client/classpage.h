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
    QString id;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    QVector<QString> info;
public:
    classInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    QVector<QString> getInfo(){return info;}
    QString getId(){return id;}
signals:
    void clicked();
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
    QVector<QString> getInfo() {
        return infoWidget->getInfo();
    }
    classInfoWidget* getInfoWidget() {
        return infoWidget;
    }
signals:
    void clicked();
};
class classDetailWidget: public QWidget{
Q_OBJECT
private:
    classWidget* currentActivity;
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
    classDetailWidget(QWidget* parent);
    void showDetail(QVector<QString> info);
    QVector<QString> getLines() {
        QVector<QString> lines;
        lines.append(title->value());
        lines.append(description->value());
        lines.append(place->value());
        lines.append(time->value());
        return lines;
    }
    void setActivity(classWidget* activity) {
        currentActivity = activity;
    }
    classWidget* getActivity() {
        return currentActivity;
    }
signals:
    void deliver(QVector<QString> msg);

};

class classFileDeliver : public QWidget{
Q_OBJECT
private:
    classWidget* currentActivity;
    textButton* select=nullptr;
    textButton* upload=nullptr;
    ScrollListContainer* fileList=nullptr;
    QVector<QString> fileNames;
    QVector<std::string> filesToSubmit;
    QString id;
    FileUpload* fileUploader;
public:
    classFileDeliver(QWidget* parent);
    void setActivity(classWidget* activity) {
        currentActivity = activity;
        id = activity->getInfoWidget()->getId();
        if(id == "") {
            id = "0";
        }
    }
    QString getId() {
        return id;
    }
signals:
    void deliver(QVector<std::string> msg);
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
    classDetailWidget* activityDtl = nullptr;
    classFileDeliver* fileDlvr = nullptr;
    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    ClassPage(QWidget* parent = nullptr);
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
