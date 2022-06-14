#ifndef CLIENT_TEACHERPAGE_H
#define CLIENT_TEACHERPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "clock.h"
class teacherClassInfoWidget : public QWidget{
Q_OBJECT
private:
    int margin = 5, spacing = 5;
    QWidget* infoWidget;
    QLabel* descLabel;
    QLabel* detailLabel;
    bigIconButton* teacherClassType;
    QString id;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
    QVector<QString> info;
    QVector<QString> downloadInfo;
public:
    teacherClassInfoWidget(QVector<QString> info, QWidget* parent = nullptr);

    QVector<QString> getInfo(){return info;}
    QVector<QString> getDownloadInfo(){return downloadInfo;}
    QString getId(){return id;}
    void modify(QVector<QString> info);
signals:
    void clicked();
};
class teacherClassWidget : public QWidget {
Q_OBJECT
private:
    QWidget* bgWidget;
    teacherClassInfoWidget* infoWidget;
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
    teacherClassWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info) {
        infoWidget->modify(info);
    }
    QVector<QString> getInfo() {
        return infoWidget->getInfo();
    }
    teacherClassInfoWidget* getInfoWidget() {
        return infoWidget;
    }
signals:
    void clicked();
};
class teacherClassDetailWidget: public QWidget{
Q_OBJECT
private:
    teacherClassWidget* currentActivity;
    textInputItem* title;
    textInputItem* description;
    textInputItem* place;
    textInputItem* time;
    textInputItem* qq;
    QLabel* examTime;
    textInputItem* week;
    textInputItem* day;
    textInputItem* begin;
    textInputItem* end;
    textInputItem* examPlace;
    textInputItem* frequency;
    bool isPersonal = true;
    bool alarm = true;
    QVector<QString> collectMsg();
//    ScrollAreaCustom* materialList;
//    ScrollAreaCustom* homeworkList;
public:
    teacherClassDetailWidget(QWidget* parent);
    void showDetail(QVector<QString> info);
    QVector<QString> getLines() {
        QVector<QString> lines;
        lines.append(title->value());
        lines.append(description->value());
        lines.append(place->value());
        lines.append(time->value());
        return lines;
    }
    QString id;
    void setActivity(teacherClassWidget* activity) {
        currentActivity = activity;
        id = activity->getInfoWidget()->getId();
        if(id == "") {
            id = "0";
        }
    }
    teacherClassWidget* getActivity() {
        return currentActivity;
    }
signals:
    void deliver(QVector<QString> msg);
    void modify(teacherClassWidget* activity);
};

class teacherClassFileDeliver : public QWidget{
Q_OBJECT
private:
    teacherClassWidget* currentActivity;
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
    teacherClassFileDeliver(QWidget* parent);
    void setActivity(teacherClassWidget* activity) {
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
class teacherClassHomeworkWidget;
class TeacherPage : public QWidget{
Q_OBJECT
private:
    QWidget* itemWidget = nullptr;
    QWidget* itemInfo = nullptr;
    QWidget* searchBar = nullptr;
    SlidePage* addteacherClass;
    ScrollAreaCustom* itemList = nullptr;
    QVector<SlidePage*> pageList;
    teacherClassInfoWidget* teacherClassInfo = nullptr;
    teacherClassDetailWidget* activityDtl = nullptr;
    teacherClassFileDeliver* fileDlvr = nullptr;
    teacherClassHomeworkWidget* homework = nullptr;
    ClassSearch* search = nullptr;

    int cornerRadius = 12;
    void resizeEvent(QResizeEvent*);
public:
    TeacherPage(QWidget* parent = nullptr);
signals:
    void checkHomework(int studentId, int teacherClassId);

};


class teacherQuizAddPage : public SlidePage{
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
    teacherQuizAddPage(int radius, int type, int width, int height, QString name, QWidget *parent = nullptr, int posy = 0);
signals:
    void deliver(QVector<QString> msg);
    void modify(QVector<QString> msg);
};
class teacherClassListWidget : public QWidget {
Q_OBJECT
private:
    QWidget* slideParent;
    QWidget* titleWidget;
    QLabel* nameLabel;
    QVector<teacherQuizAddPage*> pageList;
    QVector<bigIconButton*> extraIcons;
    ScrollAreaCustom* container;
    QVector<QWidget*> itemList;

    int overlap = 5, margin = 10, titleHeight = 40, spacing = 3;
    void resizeEvent(QResizeEvent*);
public:
    teacherClassListWidget(QString name, QVector<bigIconButton*> icons, QWidget* p, teacherClassDetailWidget *detailWidget, QWidget* parent = nullptr);
    void addContent(QWidget* p){
        container->addWidget(p, true);
        itemList.push_back(p);
    }

    void cleanContent(){
        for(int i = 0; i < itemList.size(); i++){
            container->removeWidget(itemList[i]);
        }
    }
signals:
    void clicked(int id);
    void addPage(teacherQuizAddPage*);
    void addReceived(QVector<QString>);
    void showDetail(teacherClassWidget*);
    void newActivity(teacherClassWidget*);
};

class teacherClassHomeworkInfoWidget : public QWidget{
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
    teacherClassHomeworkInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    QVector<QString> getInfo(){return info;}
    bool getFinished(){return finished;}
signals:
    void clicked(int homeworkId);

};

class teacherHomeworkWidget : public QWidget {
    //homework in the page of delivering
Q_OBJECT
private:
    QWidget* bgWidget;
    teacherClassHomeworkInfoWidget* infoWidget;
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
    teacherHomeworkWidget(QVector<QString> info, QWidget* parent = nullptr);
    teacherClassHomeworkInfoWidget* getInfoWidget() {
        return infoWidget;
    }
signals:
    void clicked(int homeworkId);
};

class teacherClassHomeworkWidget : public QWidget{
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
    int teacherClassId;
    void setteacherClassId(teacherClassWidget* activity) {
        teacherClassId = activity->getInfoWidget()->getId().toInt();
    }
    teacherClassHomeworkWidget(QWidget* parent = nullptr);
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


#endif //CLIENT_teacherClassPAGE_H
