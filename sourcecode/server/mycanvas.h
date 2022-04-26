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

class MyCanvas : public QWidget {
    Q_OBJECT
private:
    //For display
    MyGraphicsView *view;
    QHBoxLayout *mainLayout;
    QWidget *infoWidget;
    QLabel *pageName;
    //for guidance
    /*QString startName="";
    QString endName="";*/
    MyGraphicsVexItem* start;
    MyGraphicsVexItem* end;
    ALGraph *g;
    void Init();

public:
    MyCanvas(QTextStream &ts, int radius, QWidget *parent = nullptr);

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
