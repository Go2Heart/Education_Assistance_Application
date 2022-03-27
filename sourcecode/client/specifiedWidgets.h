#ifndef SPECIFIEDWIDGETS_H
#define SPECIFIEDWIDGETS_H

#include "customWidgets.h"
#include "slidepage.h"

class clockAddPage : public SlidePage {
    Q_OBJECT
private:
    textInputItem* description;
    textInputItem* place;
    textInputItem* time;
    QWidget* clockBar;
    textInputItem* frequency;
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
    bigIconButton* alerm;
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

#endif // SPECIFIEDWIDGETS_H
