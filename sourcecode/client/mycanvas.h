//
// Created by Go2Heart on 2022/3/24.
//

#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QWidget>
#include <QGridLayout>
#include <QFileDialog>
#include "slidepage.h"
#include "graph_view.h"
#include "graph_implement.h"
#include "connect.h"

class LogWidget : public QWidget {
    Q_OBJECT
private:
    ScrollAreaCustom* container = nullptr;
public:
    LogWidget(QWidget* parent = nullptr);
    void clear();
    void AddContent(QWidget* w) { container->addWidget(w, true); }
};

class MyCanvas : public QWidget {
    Q_OBJECT
private:
    MyGraphicsView *view;
    QHBoxLayout *mainLayout;
    QWidget *infoWidget;
    LogWidget* logWidget;
    QLabel *pageName;
    ComboBox* modeBox;

    MyGraphicsVexItem* start;
    MyGraphicsVexItem* end;
    QVector <MyGraphicsVexItem*> pathVector;
    ALGraph *g;
    void Init();

public:
    MyCanvas(QTextStream &ts, int radius, QWidget *parent = nullptr);
    void LoadInfo();
    void changeEndVex(int x);
signals:
    void setDel(MyCanvas* target);
    void startChanged();
    void endChanged();
    void modeBtnClicked();
    void passBtnClicked();

private slots:
    void addVex(MyGraphicsVexItem*);
    void addArc(MyGraphicsLineItem*);
};



#endif //MYCANVAS_H
