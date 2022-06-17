#include "classaddpage.h"

ClassAddPage::ClassAddPage(QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
        textItem* title = new textItem("添加课程", "华文行楷", 30, this);
        QWidget* spliter = new QWidget(this);
        spliter->setFixedHeight(2);
        spliter->setStyleSheet("background-color:gray;");
        QWidget* nowWidget = new QWidget(this);
        QHBoxLayout* nowLayout = new QHBoxLayout(nowWidget);
        nowLayout->setContentsMargins(0, 10, 0, 30);
        nowLayout->setAlignment(Qt::AlignCenter);
        nowLayout->setSpacing(10);
            selectContainer = new StudentSelectWidget(this);
            nowLayout->addWidget(selectContainer);

            QWidget* createWidget = new QWidget(this);
            createWidget->setStyleSheet("border-radius:10px");
            createWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QVBoxLayout* createLayout = new QVBoxLayout(createWidget);
            createLayout->setContentsMargins(50, 50, 50, 50);
            createLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
            createLayout->setSpacing(10);

                textInputItem* classPlace = new textInputItem("课程地点", createWidget, 1);
                textInputItem* className = new textInputItem("课程名", createWidget, 1);
                textInputItem* QQNumber =  new textInputItem("QQ群号", createWidget, 1);
                    QWidget* info1 = new QWidget(this);
                    QHBoxLayout* layout1 = new QHBoxLayout(info1);
                    layout1->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
                    layout1->setContentsMargins(0, 0, 0, 0);
                    layout1->setSpacing(1);
                        textItem* text1 = new textItem("第", "微软雅黑", 15, info1);
                        ComboBox* box1 = new ComboBox(info1);
                        box1->setFixedSize(40, 25);
                        box1->setNumberRange(1, 21);
                        textItem* text2 = new textItem("周  到第", "微软雅黑", 15, info1);
                        ComboBox* box2 = new ComboBox(info1);
                        box2->setFixedSize(40, 25);
                        box2->setNumberRange(1, 21);
                        textItem* text3 = new textItem("周  周", "微软雅黑", 15, info1);
                        ComboBox* box3 = new ComboBox(info1);
                        box3->setFixedSize(40, 25);
                        box3->setNumberRange(1, 8);
                        layout1->addWidget(text1);
                        layout1->addWidget(box1);
                        layout1->addWidget(text2);
                        layout1->addWidget(box2);
                        layout1->addWidget(text3);
                        layout1->addWidget(box3);
                    info1->setFixedHeight(40);
                customWidget* line1 = new customWidget("上课时间 ", info1, createWidget, 1);
                    QWidget* info2 = new QWidget(this);
                    QHBoxLayout* layout2 = new QHBoxLayout(info2);
                    layout2->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
                    layout2->setContentsMargins(0, 0, 0, 0);
                    layout2->setSpacing(1);
                        textItem* text4 = new textItem("第", "微软雅黑", 15, info2);
                        ComboBox* box4 = new ComboBox(info2);
                        box4->setFixedSize(40, 25);
                        box4->setNumberRange(1, 15);
                        textItem* text5 = new textItem("节 到 第", "微软雅黑", 15, info2);
                        ComboBox* box5 = new ComboBox(info2);
                        box5->setNumberRange(1, 15);
                        box5->setFixedSize(40, 25);
                        textItem* text6 = new textItem("节", "微软雅黑", 15, info2);
                        layout2->addWidget(text4);
                        layout2->addWidget(box4);
                        layout2->addWidget(text5);
                        layout2->addWidget(box5);
                        layout2->addWidget(text6);
                    info2->setFixedHeight(40);
                customWidget* line2 = new customWidget("         ", info2, createWidget, 1);


                textButton* createBtn = new textButton("创建课程!", createWidget);
                connect(createBtn, &textButton::clicked, this, [=] {
                    ClassAddQuery* nowAdd = new ClassAddQuery(
                        classPlace->value(),
                        className->value(),
                        QQNumber->value(),
                        box1->currentText().toInt(),
                        box2->currentText().toInt(),
                        box3->currentText().toInt(),
                        box4->currentText().toInt(),
                        box5->currentText().toInt(),
                        selectContainer->GetStudents());
                });
                createLayout->addWidget(classPlace);
                createLayout->addWidget(className);
                createLayout->addWidget(QQNumber);
                createLayout->addWidget(line1);
                createLayout->addWidget(line2);
                createLayout->addWidget(createBtn);
            nowLayout->addWidget(createWidget);
        layout->addWidget(title);
        layout->addWidget(spliter);
        layout->addWidget(nowWidget);
}

void ClassAddPage::reloadInfo() {
    selectContainer->reloadInfo();
}
