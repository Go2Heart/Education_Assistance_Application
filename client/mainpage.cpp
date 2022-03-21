#include "mainpage.h"

mainPage::mainPage(QWidget* parent) :
    QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
        toolbar = new QWidget(this);
        toolbar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        toolbar->setStyleSheet("background-color:#97a9ff;border-radius:0px");
        toolbar->setFixedWidth(56);
        QVBoxLayout* toolLayout = new QVBoxLayout(toolbar);
        toolbar->setLayout(toolLayout);
        toolLayout->setContentsMargins(8, 30, 8, 30);
        toolLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        toolLayout->setSpacing(20);
            bigIconButton* userBtn = new bigIconButton(1, ":/icons/icons/user.svg", "", cornerRadius, toolbar);
            userBtn->setFixedSize(40, 40);
            QWidget* spacing = new QWidget(toolbar);
            spacing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            spacing->setFixedHeight(20);
            bigIconButton* classBtn = new bigIconButton(2, "", "课程", cornerRadius, toolbar);
            classBtn->setFixedSize(40, 40);
            bigIconButton* activityBtn = new bigIconButton(2, "", "活动", cornerRadius, toolbar);
            activityBtn->setFixedSize(40, 40);
            bigIconButton* guideBtn = new bigIconButton(2, "", "导航", cornerRadius, toolbar);
            guideBtn->setFixedSize(40, 40);
            toolLayout->addWidget(userBtn);
            toolLayout->addWidget(spacing);
            toolLayout->addWidget(classBtn);
            toolLayout->addWidget(activityBtn);
            toolLayout->addWidget(guideBtn);
        mainLayout->addWidget(toolbar);

        displayWidget = new QWidget(this);
        displayWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QVBoxLayout* displayLayout = new QVBoxLayout(displayWidget);
        displayLayout->setContentsMargins(30, 30, 30, 30);
        displayLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        displayLayout->setSpacing(10);
            clock = new Clock(displayWidget);
            displayLayout->addWidget(clock);
            QWidget* infoWidget = new QWidget(displayWidget);
            infoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QHBoxLayout* infoLayout = new QHBoxLayout(infoWidget);
            infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            infoLayout->setSpacing(15);
            infoLayout->setContentsMargins(0, 0, 0, 0);
                infoContainer = new ScrollAreaCustom(false, infoWidget);
                infoContainer->setFixedWidth(250);
                infoLayout->addWidget(infoContainer);
                    QVector<QWidget*> qaq;
                    for(int i = 0; i < 50; i++){
                        textInputItem *version = new textInputItem("version", infoContainer);
                        version->setValue("beta");
                        version->setEnabled();
                        qaq.push_back(version);
                    }
                    infoContainer->addWidgets(qaq);
                QWidget* classTable = new QWidget(displayWidget);
                classTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                classTable->setStyleSheet("border:1px solid gray;background-color:green");
                infoLayout->addWidget(classTable);
            displayLayout->addWidget(infoWidget);
        mainLayout->addWidget(displayWidget);
}
