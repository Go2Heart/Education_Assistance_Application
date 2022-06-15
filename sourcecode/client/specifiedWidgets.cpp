#include "specifiedWidgets.h"
#include "basicClass.h"

timeWidget::timeWidget(QWidget* parent) :
    QWidget(parent)
{
    setStyleSheet("background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(3);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    for(int i = 0; i <= 1; i++) {
        ComboBox* box = new ComboBox(this);
        box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        box->setFixedSize(45, 25);
        box->addItem("");
        boxs.push_back(box);
    }
    textItem* split = new textItem(":", "Corbel Light", 13, this);
    boxs[0]->setNumberRange(0, 24);
    boxs[1]->setFixedSize(0, 60);
    layout->addWidget(boxs[0]);
    layout->addWidget(split);
    layout->addWidget(boxs[1]);
    setFixedSize(150, 30);
}

Timer timeWidget::GetTimer() const {
    return Timer(atoi(boxs[0]->currentText().toLatin1().data()), atoi(boxs[1]->currentText().toLatin1().data()));
}

StudentSmallWidget::StudentSmallWidget(Student student, QWidget* parent) :
    student(student), QWidget(parent)
{
    bgWidget = new QWidget(this);
    bgWidget->lower();
    bgWidget->show();
    bgWidget->setStyleSheet("background-color:#22ffa000;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color:transparent;");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(5);
        name = new textItem(student.name, "Corbel Light", 13, infoWidget);
        QWidget* space = new QWidget(infoWidget);
        space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        space->setFixedHeight(10);
        id = new textItem("[学号]" + student.studentNumber, "Corbel Light", 10, infoWidget, "color: black");
        infoLayout->addWidget(name);
        infoLayout->addWidget(space);
        infoLayout->addWidget(id);
    setFixedHeight(100);
}

void StudentSmallWidget::mousePressEvent(QMouseEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#66ffa000;");
    }
    mousePressed = true;
}

void StudentSmallWidget::mouseReleaseEvent(QMouseEvent*) {
    mousePressed = false;
    emit clicked();
}

void StudentSmallWidget::enterEvent(QEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#44ffa000;");
    }
}

void StudentSmallWidget::leaveEvent(QEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#22ffa000;");
    }
}

void StudentSmallWidget::resizeEvent(QResizeEvent*) {
    infoWidget->resize(this->width() - 2 * margin, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    bgWidget->resize(this->size());
}

bool StudentSmallWidget::Selected() {
    if(selected) {
        bgWidget->setStyleSheet("background-color:#22ffa000;");
        selected = false;
    } else {
        selected = true;
        bgWidget->setStyleSheet("background-color:#66ffa000;");
    }
    return selected;
}

StudentSelectWidget::StudentSelectWidget(QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
        container = new ScrollAreaCustom(false, this);
        container->setFixedWidth(350);
        layout->addWidget(container);
}

void StudentSelectWidget::reloadInfo() {
    selectedWidgets.clear();
    container->clear();
    StudentGroupQuery* nowQuery = new StudentGroupQuery();
    connect(nowQuery, &StudentGroupQuery::receive, this, [=](QVariant varValue) {
        QVector<Student> result = varValue.value<QVector<Student>>();
        for(int i = 0; i < result.size(); i++) {
            StudentSmallWidget* widget = new StudentSmallWidget(result[i], this);
            connect(widget, &StudentSmallWidget::clicked, widget, [=] {
                if(widget->Selected()) {
                    selectedWidgets.push_back(widget);
                } else {
                    selectedWidgets.remove(selectedWidgets.indexOf(widget));
                }
            });
            container->addWidget(widget, true);
        }
    });
}
QVector<Student> StudentSelectWidget::GetStudents() {
    QVector<Student> result;
    for(int i = 0; i < selectedWidgets.size(); i++) result.push_back(selectedWidgets[i]->student);
    return result;
}
