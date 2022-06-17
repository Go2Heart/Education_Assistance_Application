//
// Created by Go2Heart on 2022/3/24.
//

#ifndef GUIDEPAGE_H
#define GUIDEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "customWidgets.h"
#include "slidepage.h"
#include "mycanvas.h"
#include "connect.h"
#include "specifiedWidgets.h"
#include <QVector>
class GuidePage : public QWidget {
    Q_OBJECT
private:
    MyCanvas *nowCanvas;
    QWidget *displayWidget;
    QVector<SlidePage*> pageList;
    int cornerRadius = 12;
    QVBoxLayout *displayLayout;

    ComboBox* classSelectBox;
    MyCanvas *loadCanvas(const QString &path);

    void resizeEvent(QResizeEvent*);
public:
    GuidePage(QWidget *parent = nullptr);
    void LoadClassData();
    void LoadInfo();
signals:
    void addPage(SlidePage *page);
};


#endif //GUIDEPAGE_H
