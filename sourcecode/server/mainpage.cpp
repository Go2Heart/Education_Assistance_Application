#include "mainpage.h"

int type = -1;
int studentId = 255;
int teacherId = 255;

clockAddPage::clockAddPage(int radius, int type, int width, int height, QString name, QWidget* parent, int posy) :
    SlidePage(radius, type, width, height, name, parent, posy)
{
    description = new textInputItem("描述：", this);
    place = new textInputItem("地点：", this);
    timeBar = new QWidget(this);
    timeBar->setStyleSheet("background-color:transparent;");
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* timeLayout = new QHBoxLayout(timeBar);
    timeLayout->setContentsMargins(0, 0, 0, 0);
    timeLayout->setSpacing(3);
    timeLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        hour = new ComboBox(timeBar);
        hour->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        hour->setFixedSize(45, 25);
        hour->setNumberRange(0, 24);
        timeLayout->addWidget(hour);
        QFont font = QFont("Corbel Light", 13);
        QFontMetrics fm(font);
        font.setStyleStrategy(QFont::PreferAntialias);
        QLabel* split = new QLabel(":", timeBar);
        split->setFont(font);
        split->setFixedHeight(fm.lineSpacing());
        split->setStyleSheet("color: black");
        timeLayout->addWidget(split);
        minute = new ComboBox(timeBar);
        minute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        minute->setFixedSize(45, 25);
        minute->setNumberRange(0, 60);
        timeLayout->addWidget(minute);
    timeBar->setFixedHeight(30);
    customWidget* timeWidget = new customWidget("时间：", timeBar, this);

    clockBar = new QWidget(this);
    QHBoxLayout* clockLayout = new QHBoxLayout(clockBar);
    clockLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    clockLayout->setSpacing(10);
    clockLayout->setContentsMargins(0, 0, 0, 0);
        bigIconButton* alarmBtn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", 0, clockBar);
        alarmBtn->setFixedSize(30, 30);
        clockLayout->addWidget(alarmBtn);
        frequency = new ComboBox(clockBar);
        frequency->addItem("仅一次");
        frequency->addItem("每周");
        frequency->addItem("每月");
        frequency->setFixedSize(100, 25);
        customWidget* freWidget = new customWidget("频率：", frequency, this);
        clockLayout->addWidget(freWidget);
    clockBar->setFixedHeight(40);

    connect(alarmBtn, &bigIconButton::clicked, this, [=] {
        if(alarm){
            alarmBtn->setPixmap(":/icons/icons/alarm_off.svg");
            alarm = false;
        } else {
            alarmBtn->setPixmap(":/icons/icons/alarm_on.svg");
            alarm = true;
        }
    });

    textButton* createBtn = new textButton("Create!", this);
    connect(createBtn, &textButton::clicked, this, [=] {
        slideOut();
        createBtn->setText("Modify!");
        if(created)
            emit modify(collectMsg());
        else {
            created = true;
            emit deliver(collectMsg());
        }
    });
    AddContent(createBtn);
    AddContent(clockBar);
    AddContent(timeWidget);
    AddContent(place);
    AddContent(description);
}

QVector<QString> clockAddPage::collectMsg() {
    QVector<QString> tmp;
    tmp.push_back(description->value());
    tmp.push_back(place->value());
    tmp.push_back(hour->currentText() + ":" + minute->currentText());
    tmp.push_back(alarm ? "true" : "alse");
    tmp.push_back(frequency->currentText());
    return tmp;
}

clockInfoWidget::clockInfoWidget(QVector<QString> info, QWidget* parent) :
    QWidget(parent),
    alarm(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg", "", 0, this))
{
    setStyleSheet("background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    infoWidget = new QWidget(this);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(15);
        QFont descFont = QFont("Corbel Light", 13);
        QFontMetrics descm(descFont);
        descFont.setStyleStrategy(QFont::PreferAntialias);
        descLabel = new QLabel("#内容#" + info[0], infoWidget);
        descLabel->setFont(descFont);
        descLabel->setFixedHeight(descm.lineSpacing());
        descLabel->setStyleSheet("color: black");
        infoLayout->addWidget(descLabel);

        QFont detailFont = QFont("Corbel Light", 10);
        QFontMetrics detailm(detailFont);
        detailFont.setStyleStrategy(QFont::PreferAntialias);
        detailLabel = new QLabel("#地点#" + info[1] + "       #时间#" + info[2], infoWidget);
        detailLabel->setFont(detailFont);
        detailLabel->setFixedHeight(detailm.lineSpacing());
        detailLabel->setStyleSheet("color: gray");
        infoLayout->addWidget(detailLabel);
    alarm->setFixedSize(30, 30);
}

void clockInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void clockInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked();
}

void clockInfoWidget::resizeEvent(QResizeEvent*) {
    infoWidget->resize(this->width() - alarm->width() - 2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    alarm->move(this->width() - alarm->width() - margin, this->height() / 2 - alarm->height() / 2);
}

void clockInfoWidget::modify(QVector<QString> info) {
    descLabel->setText("#内容#" + info[0]);
    detailLabel->setText("#地点#" + info[1] + "       #时间#" + info[2]);
    alarm->setPixmap(info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg");
}

clockWidget::clockWidget(QVector<QString> info, QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(100);
    bgWidget = new QWidget(this);
    bgWidget->resize(this->size());
    bgWidget->setStyleSheet("background-color:" + defaultColor + ";border-radius:12px;");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    infoWidget = new clockInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &clockInfoWidget::clicked, this, [=] {emit clicked();});
}

void clockWidget::resizeEvent(QResizeEvent *) {
    bgWidget->resize(this->size());
}

clockfoldWidget::clockfoldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* p, QWidget* parent) :
    foldWidget(name, h, icons, parent),
    slideParent(p)
{
    connect(icons[0], &bigIconButton::clicked, this, [=] {
        clockAddPage* newPage = new clockAddPage(12, 1, 300, 0, "创建新待办", slideParent);
        emit addPage(newPage);
        connect(newPage, &clockAddPage::deliver, this, [=](QVector<QString> s) {
            clockWidget* newWidget = new clockWidget(s, this);
            addContent(newWidget);
            connect(newWidget, &clockWidget::clicked, newPage, &SlidePage::slideIn);
            connect(newPage, &clockAddPage::modify, newWidget, [=](QVector<QString> s) {
                newWidget->modify(s);
            });
            pageList.push_back(newPage);
        });
        newPage->slideIn();
    });
}

mainPage::mainPage(QWidget* parent) :
    QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
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
                    iconVec.push_back(new bigIconButton(9, ":/icons/icons/add.svg"));
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
        currentPage = MAIN;

        activityPage = new ActivityPage(this);
        activityPage->hide();

        userInfo = new SlidePage(10, SlidePage::FIXED, 250, 100, "", displayWidget, 0);
        slidePageList.push_back(userInfo);
        connect(userBtn, &bigIconButton::clicked, userInfo, &SlidePage::slideIn);
        connect(activityBtn, &bigIconButton::clicked, this, [=] {
            hideCurrentPage();
            if(currentPage == ACTIVITY){
                showNewPage(displayWidget);
                currentPage = MAIN;
            } else {
                showNewPage(activityPage);
                currentPage = ACTIVITY;
            }
        });

        guidePage = new GuidePage(this);
        guidePage->hide();
        connect(guideBtn, &bigIconButton::clicked, [=] {
            hideCurrentPage();
            if (currentPage == GUIDE){
                showNewPage(displayWidget);
                currentPage = MAIN;
            } else {
                showNewPage(guidePage);
                currentPage = GUIDE;
            }
        });
        connect(guidePage, &GuidePage::addPage, this, [=](SlidePage* page){
            if(page){
                //qDebug()<<page->objectName();
                //pageList.push_back(page);
            }
        });
}

void mainPage::hideCurrentPage() {
    switch (currentPage) {
        case MAIN:
            mainLayout->removeWidget(displayWidget);
            displayWidget->hide();
            break;
        case CLASS:
            break;
        case ACTIVITY:
            mainLayout->removeWidget(activityPage);
            activityPage->hide();
            break;
        case GUIDE:
            mainLayout->removeWidget(guidePage);
            guidePage->hide();
            break;
    }
}

void mainPage::showNewPage(QWidget* w) {
    mainLayout->addWidget(w);
    w->show();
    userInfo->changeParent(w);
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
