#ifndef SPECIFIEDWIDGETS_H
#define SPECIFIEDWIDGETS_H

#include "customWidgets.h"
#include "connect.h"

class Timer;
class timeWidget : public QWidget { // 时间搜索栏类，可以根据当前输入返回时间戳类
    Q_OBJECT
private:
    QVector<ComboBox*> boxs;
    QVector<textItem*> splits;
public:
    timeWidget(QWidget* parent = nullptr);
    Timer GetTimer() const;
    void clear();
};
class StudentSmallWidget : public QWidget {
    Q_OBJECT
private:
    int margin = 5;
    QWidget* bgWidget = nullptr;
    QWidget* infoWidget = nullptr;
    textItem* name;
    textItem* id;
    bool selected = false;
    bool mousePressed = false;
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void resizeEvent(QResizeEvent*);
public:
    Student student;
    StudentSmallWidget(Student student, QWidget* parent);
    bool Selected();
signals:
    void clicked();
};

class StudentSelectWidget : public QWidget {
    Q_OBJECT
private:
    ScrollAreaCustom* container = nullptr;
    QVector<StudentSmallWidget*> selectedWidgets;
public:
    StudentSelectWidget(QWidget *parent = nullptr);
    void reloadInfo();
    QVector<Student> GetStudents();
};

#endif // SPECIFIEDWIDGETS_H
