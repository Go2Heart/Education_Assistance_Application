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
        toolbar->setStyleSheet("background-color:#97a9ff;border-radius:0px;");
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
        displayWidget->setLayout(displayLayout);
            clock = new Clock(displayWidget);
            displayLayout->addWidget(clock);
            QWidget* infoWidget = new QWidget(displayWidget);
            infoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            QHBoxLayout* infoLayout = new QHBoxLayout(infoWidget);
            infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            infoLayout->setSpacing(15);
            infoLayout->setContentsMargins(0, 0, 0, 0);
                QWidget* eventWidget = new QWidget(infoWidget);
                eventWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
                eventWidget->setFixedWidth(400);
                QVBoxLayout* eventLayout = new QVBoxLayout(eventWidget);
                eventLayout->setAlignment(Qt::AlignCenter);
                eventLayout->setSpacing(15);
                eventLayout->setContentsMargins(0, 0, 0, 0);
                    infoContainer = new ScrollAreaCustom(false, eventWidget);
                    infoContainer->setFixedWidth(400);
                    eventLayout->addWidget(infoContainer);

                    QVector<bigIconButton*> iconVec;
                    iconVec.push_back(new bigIconButton(1, ":/icons/icons/add.svg"));
                    clockfoldWidget* clockWidget = new clockfoldWidget("clock", 500, iconVec, displayWidget, eventWidget);
                    connect(clockWidget, &clockfoldWidget::addPage, this, [=](clockAddPage* page) {
                        clockPageList.push_back(page);
                    });
                    eventLayout->addWidget(clockWidget);

                infoLayout->addWidget(eventWidget);
                QWidget* classTable = new QWidget(displayWidget);
                classTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                classTable->setStyleSheet("border:1px solid gray;background-color:green");
                infoLayout->addWidget(classTable);
            displayLayout->addWidget(infoWidget);
        mainLayout->addWidget(displayWidget);

        userInfo = new SlidePage(10, SlidePage::WIDTH_FIXED, 250, 100, "", displayWidget, 0);
        slidePageList.push_back(userInfo);
        connect(userBtn, &bigIconButton::clicked, userInfo, &SlidePage::slideIn);
}

void mainPage::resizeEvent(QResizeEvent*) {
    for(int i = 0; i < slidePageList.size(); i++) {
        slidePageList[i]->resize(slidePageList[i]->width() - 1, (slidePageList[i]->Type() & SlidePage::HEIGHT_FIXED) ? slidePageList[i]->height() : displayWidget->height());
        slidePageList[i]->resize(slidePageList[i]->width() + 1, slidePageList[i]->height());
    }
    for(int i = 0; i < clockPageList.size(); i++) {
        clockPageList[i]->resize(clockPageList[i]->width() - 1, (clockPageList[i]->Type() & SlidePage::HEIGHT_FIXED) ? clockPageList[i]->height() : displayWidget->height());
        clockPageList[i]->resize(clockPageList[i]->width() + 1, clockPageList[i]->height());
    }
}
