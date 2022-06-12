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
    QVector<QString> downloadInfo;
public:
    classInfoWidget(QVector<QString> info, QWidget* parent = nullptr);

    QVector<QString> getInfo(){return info;}
    QVector<QString> getDownloadInfo(){return downloadInfo;}
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
    textInputItem* qq;
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
    textButton* downloadButton=nullptr;
    ScrollAreaCustom* fileList=nullptr;
    ScrollAreaCustom* downloadList=nullptr;
    QVector<QString> fileNames;
    QVector<std::string> filesToSubmit;
    QVector<QString> fileToDownload;
    QWidget * downloadElement;
    QString id;
    FileUpload* fileUploader;
    FileDownload* fileDownloader;
public:
    classFileDeliver(QWidget* parent);
    void setActivity(classWidget* activity) {
        currentActivity = activity;
        id = activity->getInfoWidget()->getId();
        if(id == "") {
            id = "0";
        }
    }
    void setDownloadInfo(QVector<QString> info) {
        fileToDownload = info;
    }
    QString getId() {
        return id;
    }
signals:
    void deliver(QVector<std::string> msg);
    void download();
};
class classHomeworkWidget;
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
    classHomeworkWidget* homework = nullptr;
    ClassSearch* search = nullptr;

    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    ClassPage(QWidget* parent = nullptr);
signals:
    void checkHomework(int studentId, int classId);

};



class classListWidget : public QWidget {
Q_OBJECT
private:
    QWidget* slideParent;
    QWidget* titleWidget;
    QLabel* nameLabel;
    QVector<bigIconButton*> extraIcons;
    ScrollAreaCustom* container;
    QVector<QWidget*> itemList;

    int overlap = 5, margin = 10, titleHeight = 40, maxHeight, spacing = 3;
public:
    classListWidget(QString name, int h, QWidget* p, QWidget* parent = nullptr);
    void addContent(QWidget* p){
        container->addWidget(p, true);
        itemList.push_back(p);
    }
     void cleanContent(){
        for(int i = 0; i < itemList.size(); i++){
            container->removeWidget(itemList[i]);

        }
        itemList.clear();
    }
    void resizeEvent(QResizeEvent*);
signals:
    void clicked(int id);
};

class classHomeworkInfoWidget : public QWidget{
    //homework info the widget inside the scroll container
Q_OBJECT
private:
    int margin = 5, spacing = 5;
    QWidget* infoWidget;
    QLabel* titleLabel;
    QLabel* descriptionLabel;
    QVector<QString> info;
    int id;
    bool finished = false;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
public:
    classHomeworkInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    QVector<QString> getInfo(){return info;}
    bool getFinished(){return finished;}
signals:
    void clicked(int homeworkId);

};

class homeworkWidget : public QWidget {
    //homework in the page of delivering
Q_OBJECT
private:
    QWidget* bgWidget;
    classHomeworkInfoWidget* infoWidget;
    QString defaultColor = "#0a0078d4";
    QString hoverColor = "#1a0078d4";
    QString pressedColor = "#2a0078d4";
    //SlidePage* deliverPage = nullptr;
    void enterEvent(QEvent*) {
        bgWidget->setStyleSheet("background-color:" + hoverColor + ";border-radius:12px;");
    }
    void leaveEvent(QEvent*) {
        bgWidget->setStyleSheet("background-color:" + defaultColor + ";border-radius:12px;");
    }
    void resizeEvent(QResizeEvent*);
public:
    homeworkWidget(QVector<QString> info, QWidget* parent = nullptr);
    classHomeworkInfoWidget* getInfoWidget() {
        return infoWidget;
    }
signals:
    void clicked(int homeworkId);
};

class classHomeworkWidget : public QWidget{
Q_OBJECT
private:
    QWidget* searchBar;
    ScrollAreaCustom* container;
    QVector<QWidget*> itemList;
    ScrollAreaCustom* tempHomework;

    QVector<QString> fileNames;
    QVector<std::string> filesToSubmit;
    QVector<QString> fileToDownload;
    HomeworkUpload* homeworkUploader;
    //void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    //void resizeEvent(QResizeEvent*);

public:
    HomeworkQuery* homeworkQuery = nullptr;
    HomeworkPost* homeworkPost = nullptr;
    bool received = false;
    int chooseId;
    int classId;
    void setClassId(classWidget* activity) {
        classId = activity->getInfoWidget()->getId().toInt();
    }
    classHomeworkWidget(QWidget* parent = nullptr);
    void addContent(QWidget* p){
        container->addWidget(p, true);
        itemList.push_back(p);
    }
    void cleanContent(){
        for(int i = 0; i < itemList.size(); i++){
            container->removeWidget(itemList[i]);
        }
        itemList.clear();
    }

signals:
    void clicked();


};


#endif //CLIENT_CLASSPAGE_H
