#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customWidgets.h"
#include "customScrollContainer.h"
#include "slidepage.h"
#include "activitypage.h"
#include "guidepage.h"
#include "clock.h"

class clockAddPage : public SlidePage {
    Q_OBJECT
private:
    textInputItem* description;
    textInputItem* place;
    QWidget* timeBar;
    ComboBox*  hour;
    ComboBox* minute;
    QWidget* clockBar;
    ComboBox* frequency;
    bool alarm = true;
    bool created = false;
    QVector<QString> collectMsg();
public:
    clockAddPage(int radius, int type, int width, int height, QString name, QWidget *parent = nullptr, int posy = 0);
signals:
    void deliver(QVector<QString> msg);
    void modify(QVector<QString> msg);
};

class clockInfoWidget : public QWidget {
    Q_OBJECT
private:
    int margin = 5, spacing = 5;
    QWidget* infoWidget;
    QLabel* descLabel;
    QLabel* detailLabel;
    bigIconButton* alarm;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);
public:
    clockInfoWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info);
signals:
    void clicked();
};

class clockWidget : public QWidget {
    Q_OBJECT
private:
    QWidget* bgWidget;
    clockInfoWidget* infoWidget;
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
    clockWidget(QVector<QString> info, QWidget* parent = nullptr);
    void modify(QVector<QString> info) {
        infoWidget->modify(info);
    }
signals:
    void clicked();
};

class clockfoldWidget : public foldWidget {
    Q_OBJECT
private:
    QWidget* slideParent;
    QVector<clockAddPage*> pageList;
public:
    clockfoldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* p, QWidget* parent = nullptr);
signals:
    void addPage(clockAddPage*);
};

class mainPage : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout* mainLayout = nullptr;
    QWidget* toolbar = nullptr;
    QWidget* displayWidget = nullptr;
    QTimer* clickpressTimer;
    QTimer* clickreleaseTimer;
    SlidePage* userInfo;
    ActivityPage* activityPage;
    Clock* clock = nullptr;
    ScrollAreaCustom* infoContainer = nullptr;
    QVector<SlidePage*> slidePageList;
    QVector<clockAddPage*> clockPageList;

    GuidePage* guidePage  = nullptr;
    int cornerRadius = 12;
    bool stateChanged = false;
    int currentPage = 0;
    void hideCurrentPage();
    void showNewPage(QWidget* w);
    void resizeEvent(QResizeEvent*);
public:
    enum PageType {
        MAIN = 0, CLASS = 1, ACTIVITY = 2, GUIDE = 3
    };
    mainPage(QWidget* parent = nullptr);
    void raisePage() {
        displayWidget->raise();
        clickpressTimer->start(1000);
    }
};

extern int type;
extern int studentId;
extern int teacherId;

#endif // MAINPAGE_H
