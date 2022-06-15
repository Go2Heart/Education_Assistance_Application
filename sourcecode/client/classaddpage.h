#ifndef CLASSADDPAGE_H
#define CLASSADDPAGE_H

#include "customWidgets.h"
#include "slidepage.h"
#include "connect.h"
#include "specifiedWidgets.h"


class ClassAddPage : public QWidget
{
    Q_OBJECT
private:
    StudentSelectWidget* selectContainer = nullptr;
public:
    ClassAddPage(QWidget *parent = nullptr);
    void reloadInfo();
signals:
};

#endif // CLASSADDPAGE_H
