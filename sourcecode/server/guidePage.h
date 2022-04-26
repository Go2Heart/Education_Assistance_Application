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
#include <QVector>
class GuidePage:public QWidget {
    Q_OBJECT
private:
    SlidePage *createNewPage = nullptr;
    SlidePage *defaultSettingsPage = nullptr;
    SlidePage *curSettingsPage = nullptr;
    QVector<MyCanvas*> canvasList;
    MyCanvas* curCanvas = nullptr;
    SlidePage *layersPage = nullptr;
    singleSelectGroup *layerSel = nullptr;
    QWidget *defaultPage;
    QWidget *displayWidget;
    QVector<SlidePage*> pageList;
    int cornerRadius = 12;

    QLineEdit *canvasTitle = nullptr;
    QLineEdit *canvasDesc = nullptr;
    customIcon *settingsIcon = nullptr;
    QVBoxLayout *displayLayout;
    MyCanvas *loadCanvas(const QString &path);

    void resizeEvent(QResizeEvent*);
public:
    GuidePage(QWidget *parent = nullptr);
signals:
    void addPage(SlidePage *page);
};


#endif //GUIDEPAGE_H
