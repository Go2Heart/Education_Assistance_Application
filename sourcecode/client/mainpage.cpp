#include "mainpage.h"
#include "global.h"

int type = -1;
Student nowStudent;
Teacher nowTeacher;
int studentId = 255;
int teacherId = 255;

UserInfoWidget::UserInfoWidget(QWidget* parent) :
    QWidget(parent)
{
    setStyleSheet("background-color:#00000000");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(7);
    layout->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    layout->setContentsMargins(10, 10, 0, 0);
        QWidget* line1 = new QWidget(this);
        line1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QHBoxLayout* layout1 = new QHBoxLayout(line1);
        layout1->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
        layout1->setContentsMargins(0, 0, 0, 0);
            textItem* username = new textItem(nowStudent.name, "微软雅黑", 15, line1);
            bigIconButton* identityIcon = new bigIconButton(9, ":/icons/icons/student.svg", "", "", 0, 5, line1);
            identityIcon->setFixedSize(username->height() + 5, username->height() + 5);
            layout1->addWidget(username);
            layout1->addWidget(identityIcon);
        line1->setFixedHeight(40);
        QWidget* split1 = new QWidget(this);
        split1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        split1->setStyleSheet("background-color:#ff666666;");
        split1->setFixedHeight(2);
        layout->addWidget(line1);
        layout->addWidget(split1);

        QWidget* line2 = new QWidget(this);
        line2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QHBoxLayout* layout2 = new QHBoxLayout(line2);
        layout2->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
        layout2->setContentsMargins(0, 0, 0, 0);
            bigIconButton* idIcon = new bigIconButton(9, ":/icons/icons/student-id.svg", "", "", 0, 5, line2);
            textItem* studentId = new textItem(nowStudent.studentNumber, "微软雅黑", 13, line2);
            idIcon->setFixedSize(studentId->height(), studentId->height());
            layout2->addWidget(idIcon);
            layout2->addWidget(studentId);
        line2->setFixedHeight(30);
        QWidget* split2 = new QWidget(this);
        split2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        split2->setStyleSheet("background-color:#ff666666;");
        split2->setFixedHeight(2);
        layout->addWidget(line2);
        layout->addWidget(split2);
        textButton* passwdBtn = new textButton("修改密码", this);
        connect(passwdBtn, &textButton::clicked, this, [=] {
            QString newpasswd = QInputDialog::getText(this,
                "请输入密码", "密码",
                QLineEdit::Normal, "", nullptr, Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint
            );
            PasswordUpd* passwordUpd = new PasswordUpd(newpasswd);
        });
        layout->addWidget(passwdBtn);
    setFixedHeight(300);
    /*QTimer* t = new QTimer(this);
    t->setSingleShot(true);
    t->start(1000);
    connect(t, &QTimer::timeout, this, [=] {
        t->start(1000);
        resize(220, 300);
    });*/
}

TeacherInfoWidget::TeacherInfoWidget(QWidget* parent) :
    QWidget(parent)
{
    setStyleSheet("background-color:#00000000");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(7);
    layout->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    layout->setContentsMargins(10, 10, 0, 0);
        QWidget* line1 = new QWidget(this);
        QHBoxLayout* layout1 = new QHBoxLayout(line1);
        layout1->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
        line1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout1->setContentsMargins(0, 0, 0, 0);
            textItem* username = new textItem(nowTeacher.name, "微软雅黑", 15, line1);
            bigIconButton* identityIcon = new bigIconButton(9, ":/icons/icons/teacher.svg", "", "", 0, 5, line1);
            identityIcon->setFixedSize(username->height() + 5, username->height() + 5);
            layout1->addWidget(username);
            layout1->addWidget(identityIcon);
        line1->setFixedHeight(40);
        QWidget* split1 = new QWidget(this);
        split1->setStyleSheet("background-color:#ff666666;");
        split1->setFixedHeight(2);
        layout->addWidget(line1);
        layout->addWidget(split1);

        QWidget* line2 = new QWidget(this);
        QHBoxLayout* layout2 = new QHBoxLayout(line2);
        line2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout2->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
        layout2->setContentsMargins(0, 0, 0, 0);
            bigIconButton* idIcon = new bigIconButton(9, ":/icons/icons/student-id.svg", "", "", 0, 5, line2);
            textItem* teacherId = new textItem(nowTeacher.teacherNumber, "微软雅黑", 13, line2);
            idIcon->setFixedSize(teacherId->height(), teacherId->height());
            layout2->addWidget(idIcon);
            layout2->addWidget(teacherId);
        line2->setFixedHeight(30);
        QWidget* split2 = new QWidget(this);
        split2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        split2->setStyleSheet("background-color:#ff666666;");
        split2->setFixedHeight(2);
        layout->addWidget(line2);
        layout->addWidget(split2);
        textButton* passwdBtn = new textButton("修改密码", this);
        connect(passwdBtn, &textButton::clicked, this, [=] {
            QString newpasswd = QInputDialog::getText(this,
                "请输入密码", "密码",
                QLineEdit::Normal, "", nullptr, Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint
            );
            PasswordUpd* passwordUpd = new PasswordUpd(newpasswd);
        });
        layout->addWidget(passwdBtn);
    setFixedHeight(300);
}


clockAddPage::clockAddPage(int radius, int type, int width, int height, QString name, QWidget* parent,  int posy) :
    SlidePage(radius, type, width, height, name, parent, posy)
{
    description = new textInputItem("描述：", this);
    place = new textInputItem("地点：", this);
    dayBar = new QWidget(this);
    dayBar->setStyleSheet("background-color: transparent;");
    dayBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* dayLayout = new QHBoxLayout(dayBar);
    dayLayout->setContentsMargins(0, 0, 0, 0);
    dayLayout->setSpacing(3);
    dayLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        week = new ComboBox(dayBar);
        week->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        week->setFixedSize(45,25);
        week->setNumberRange(1,20);
        dayLayout->addWidget(week);
        QLabel* daySplit = new QLabel("周", dayBar);
        daySplit->setStyleSheet("background-color: transparent;");
        dayLayout->addWidget(daySplit);
        day = new ComboBox(dayBar);
        day->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        day->setFixedSize(45,25);
        day->setNumberRange(1,7);
        QLabel* daySplit2 = new QLabel("天", dayBar);
        daySplit2->setStyleSheet("background-color: transparent;");
        dayLayout->addWidget(day);
        dayLayout->addWidget(daySplit2);
    dayBar->setFixedHeight(30);
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
        QLabel* split = new QLabel("：", timeBar);
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

    customWidget* dayWidget = new customWidget("日期：", dayBar, this);

    customWidget* timeWidget = new customWidget("时间：", timeBar, this);
    clockBar = new QWidget(this);
    QHBoxLayout* clockLayout = new QHBoxLayout(clockBar);
    clockLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    clockLayout->setSpacing(10);
    clockLayout->setContentsMargins(0, 0, 0, 0);
        alarmBtn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", "", 0, 0, clockBar);
        alarmBtn->setFixedSize(30, 30);
        clockLayout->addWidget(alarmBtn);
        frequency = new ComboBox(clockBar);
        frequency->addItem("仅一次");
        frequency->addItem("每天");
        frequency->addItem("每周");
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

    createBtn = new textButton("Create!", this);
    connect(createBtn, &textButton::clicked, this, [=] {
        if(description->value().isEmpty() || place->value().isEmpty()){
            QMessageBox::warning(this, "Warning", "请填写完整信息！");
            return;
        }

        if(hour->currentText().isEmpty() || minute->currentText().isEmpty()){
            QMessageBox::warning(this, "Warning", "请填写完整时间！");
            return;
        }

        Timer tempTime = Timer(hour->currentText().toInt(), minute->currentText().toInt(), day->currentText().toInt(), week->currentText().toInt());

        if(created == false) {
            AlarmModify* nowModify = new AlarmModify(nowid, tempTime.Zip(), frequency->currentIndex(), description->value(), place->value(), alarm);
            QMessageBox::information(this, "Success", "修改成功！");
            connect(nowModify, &AlarmModify::receive, this, [=] {
                emit msgDeliver();
            });
        } else {
            AlarmAdd* nowAdd = new AlarmAdd(tempTime.Zip(), frequency->currentIndex(), description->value(), place->value(), alarm);
            QMessageBox::information(this, "Success", "创建成功！");
            connect(nowAdd, &AlarmAdd::receive, this, [=] { //why????
                qDebug() << "receive data!";
                emit msgDeliver();
            });
        }
        slideOut();
    });
    AddContent(createBtn);
    AddContent(clockBar);
    AddContent(timeWidget);
    AddContent(dayWidget);
    AddContent(place);
    AddContent(description);
}

void clockAddPage::clear() {
    description->setValue("");
    place->setValue("");
    week->setEditText("");
    day->setEditText("");
    hour->setEditText("");
    minute->setEditText("");
    frequency->setCurrentIndex(0);
    alarmBtn->setPixmap(":/icons/icons/alarm_on.svg");
    createBtn->setText("Create!");
    alarm = true;
    created = true;
}

void clockAddPage::LoadFromData(int id, QString desc, QString place, Timer x, int frequency, bool alarm) {
    nowid = id;
    description->setValue(desc);
    this->place->setValue(place);
    hour->setEditText(QString::asprintf("%d", x.hour));
    minute->setEditText(QString::asprintf("%d", x.minute));
    this->frequency->setCurrentIndex(frequency);
    this->alarm = alarm;
    if(alarm) alarmBtn->setPixmap(":/icons/icons/alarm_off.svg");
    else alarmBtn->setPixmap(":/icons/icons/alarm_on.svg");
    created = false;
    createBtn->setText("Modify!");
}

clockInfoWidget::clockInfoWidget(QVector<QString> info, QWidget* parent) :
    QWidget(parent),
    alarm(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg", "", "", 0, 0, this))
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
    modifyPage = new clockAddPage(12, 1, 300, 0, "1", slideParent);
    connect(modifyPage, &clockAddPage::msgDeliver, this, [=] {
        reloadInfo();
    });
    connect(icons[0], &bigIconButton::clicked, this, [=] {
        modifyPage->SetName("创建新待办");
        modifyPage->clear();
        modifyPage->slideIn();
    });
}

void clockfoldWidget::reloadInfo() {
    AlarmsQuery* nowQuery = new AlarmsQuery();
    clear();
    connect(nowQuery, &AlarmsQuery::receive, this, [=](QVariant varValue) {
        QVector<Alarm> tmp = varValue.value<QVector<Alarm>>();
        for(int i = 0; i < tmp.size(); i++) {
            QVector<QString> infos;
            infos.push_back(tmp[i].desc);
            infos.push_back(tmp[i].place);
            infos.push_back(tmp[i].t.ToString());
            infos.push_back(tmp[i].enabled ? "true" : "false");
            clockWidget* nowWidget = new clockWidget(infos, this);
            connect(nowWidget, &clockWidget::clicked, this, [=] {
                modifyPage->SetName("修改待办");
                modifyPage->LoadFromData(tmp[i].id, tmp[i].desc, tmp[i].place, tmp[i].t, tmp[i].frequency, tmp[i].enabled);
                modifyPage->slideIn();
            });
            addContent(nowWidget);
        }
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
        toolLayout = new QVBoxLayout(toolbar);
        toolbar->setLayout(toolLayout);
        toolLayout->setContentsMargins(8, 30, 8, 30);
        toolLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
        toolLayout->setSpacing(20);
            userBtn = new bigIconButton(1, ":/icons/icons/user.svg", "", "", 0, cornerRadius, toolbar);
            userBtn->setFixedSize(40, 40);
            QWidget* spacing = new QWidget(toolbar);
            spacing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            spacing->setFixedHeight(20);
            classBtn = new bigIconButton(1, ":/icons/icons/classpage.svg", "", "", 0, cornerRadius, toolbar);
            classBtn->setFixedSize(40, 40);
            classBtn->hide();
            activityBtn = new bigIconButton(1, ":/icons/icons/activitypage.svg", "", "", 0, cornerRadius, toolbar);
            activityBtn->setFixedSize(40, 40);
            activityBtn->hide();
            guideBtn = new bigIconButton(1, ":/icons/icons/guidepage.svg", "", "", 0, cornerRadius, toolbar);
            guideBtn->setFixedSize(40, 40);
            guideBtn->hide();
            classAddBtn = new bigIconButton(1, ":/icons/icons/addclasspage.svg", "", "", 0, cornerRadius, toolbar);
            classAddBtn->setFixedSize(40, 40);
            classAddBtn->hide();
            logoutBtn = new bigIconButton(1, ":/icons/icons/logout.svg", "", "", 0, cornerRadius, toolbar);
            logoutBtn->setFixedSize(40, 40);
            logoutBtn->hide();
            toolLayout->addWidget(userBtn);
            toolLayout->addWidget(spacing);
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
                    clockWidget = new clockfoldWidget("clock", 500, iconVec, displayWidget, eventWidget);
                    clockPageList.push_back(clockWidget->modifyPage);
                    eventLayout->addWidget(clockWidget);

                infoLayout->addWidget(eventWidget);
                ScrollAreaCustom* classArea = new ScrollAreaCustom(true, displayWidget, false);
                classArea->setFixedWidth(800);
                newTable = new ClockTable(classArea);
                classArea->addWidget(newTable, false);
                /*
                QWidget* classTable = new QWidget(displayWidget);
                classTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                classTable->setStyleSheet("border:1px solid gray;background-color:green");
                */
                infoLayout->addWidget(classArea);
            displayLayout->addWidget(infoWidget);
        displayWidget->hide();

        activityPage = new ActivityPage(this);
        activityPage->hide();

        teacherPage = new TeacherPage(this);
        teacherPage->hide();

        userInfo = new SlidePage(10, SlidePage::FIXED, 250, 400, "", displayWidget, 0);
        userInfo->bgWidget->setStyleSheet("background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 200, 255), stop:0.3 rgba(255, 255, 240, 255), stop:1 rgba(255, 255, 255, 255));border-radius:10px;");
        slidePageList.push_back(userInfo);
        connect(userBtn, &bigIconButton::clicked, userInfo, &SlidePage::slideIn);

        guidePage = new GuidePage(this);
        guidePage->hide();

        classPage = new ClassPage(this);
        classPage->hide();

        classAddPage = new ClassAddPage(this);
        classAddPage->hide();

        connect(activityBtn, &bigIconButton::clicked, this, [=] {
            hideCurrentPage();
            if(currentPage == ACTIVITY){
                StartTimer* restart = new StartTimer();
                LoadInfo();
                showNewPage(displayWidget);
                currentPage = MAIN;
            } else {
                StopTimer* stop = new StopTimer();
                activityPage->LoadInfo();
                showNewPage(activityPage);
                currentPage = ACTIVITY;
            }
        });
        connect(classBtn, &bigIconButton::clicked, this, [=] {
            hideCurrentPage();
            if(currentPage == CLASS){
                StartTimer* restart = new StartTimer();
                LoadInfo();
                showNewPage(displayWidget);
                currentPage = MAIN;
            } else {
                StopTimer* stop = new StopTimer();
                classPage->LoadInfo();
                showNewPage(classPage);
                currentPage = CLASS;
            }
        });
        connect(guideBtn, &bigIconButton::clicked, this, [=] {
            hideCurrentPage();
            if (currentPage == GUIDE){
                StartTimer* restart = new StartTimer();
                LoadInfo();
                showNewPage(displayWidget);
                currentPage = MAIN;
            } else {
                StopTimer* stop = new StopTimer();
                guidePage->LoadInfo();
                showNewPage(guidePage);
                currentPage = GUIDE;
            }
        });
        connect(classAddBtn, &bigIconButton::clicked, this, [=] {
            hideCurrentPage();
            if (currentPage == CLASSADD){
                teacherPage->LoadInfo();
                showNewPage(teacherPage);
                currentPage = TEACHERPAGE;
            } else {
                classAddPage->reloadInfo();
                showNewPage(classAddPage);
                currentPage = CLASSADD;
            }
        });
        connect(logoutBtn, &bigIconButton::clicked, this, [=] {
            type = -1;
            studentId = teacherId = 255;
            hideCurrentPage();
            emit logout();
        });
        connect(guidePage, &GuidePage::addPage, this, [=](SlidePage* page){
            if(page){
                //qDebug()<<page->objectName();
                //pageList.push_back(page);
            }
        });

    QTimer* flushTimer = new QTimer;
    flushTimer->setSingleShot(true);
    flushTimer->start(100);
    connect(flushTimer, &QTimer::timeout,this, [=] {
        if(type == 0) LoadTriggerInfo();
        flushTimer->start(100);
    });

    connect(this, &mainPage::getInfo, this, [=] {
        if(type == STUDENT) {
            userDetail = new UserInfoWidget(userInfo);
            //qDebug()<<"debug begin";
            userInfo->AddContent(userDetail);
        } else if(type == TEACHER) {
            teacherDetail = new TeacherInfoWidget(userInfo);
            userInfo->AddContent(teacherDetail);
        }
    });

    connect(this, &mainPage::logined, this, [=] {
        LoadInfo();
        if(type == 0){
            showNewPage(displayWidget);
            currentPage = MAIN;
        } else {
            showNewPage(teacherPage);
            currentPage = TEACHERPAGE;
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
            mainLayout->removeWidget(classPage);
            classPage->hide();
            break;
        case ACTIVITY:
            mainLayout->removeWidget(activityPage);
            activityPage->hide();
            break;
        case GUIDE:
            mainLayout->removeWidget(guidePage);
            guidePage->hide();
            break;
        case TEACHERPAGE:
            mainLayout->removeWidget(teacherPage);
            teacherPage->hide();
        case CLASSADD:
            mainLayout->removeWidget(classAddPage);
            classAddPage->hide();
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

void mainPage::LoadInfo() {
    userInfo->clear();
    if(lastUserType == STUDENT) {
        toolLayout->removeWidget(classBtn);
        toolLayout->removeWidget(activityBtn);
        toolLayout->removeWidget(guideBtn);
        classBtn->hide();
        activityBtn->hide();
        guideBtn->hide();
        userInfo->RemoveContent(userDetail);
    } else {
        toolLayout->removeWidget(classAddBtn);
        classAddBtn->hide();
        userInfo->RemoveContent(teacherDetail);
    }
    lastUserType = type;
    toolLayout->removeWidget(logoutBtn);
    logoutBtn->hide();
    if(type == STUDENT) {
        classBtn->show();
        activityBtn->show();
        guideBtn->show();
        toolLayout->addWidget(classBtn);
        toolLayout->addWidget(activityBtn);
        toolLayout->addWidget(guideBtn);
        StudentInfoQuery* nowQuery = new StudentInfoQuery();
        connect(nowQuery, &StudentInfoQuery::receive, this, [=](QVariant varValue) {
            nowStudent = varValue.value<Student>();
            emit getInfo();
        });
        clockWidget->reloadInfo();
        LoadTriggerInfo();
        newTable->LoadData();
    } else {
        toolLayout->addWidget(classAddBtn);
        classAddBtn->show();
        TeacherInfoQuery* nowQuery = new TeacherInfoQuery();
        connect(nowQuery, &TeacherInfoQuery::receive, this, [=](QVariant varValue) {
            nowTeacher = varValue.value<Teacher>();
            emit getInfo();
        });
        teacherPage->LoadInfo();
    }
    logoutBtn->show();
    toolLayout->addWidget(logoutBtn);
}

void mainPage::LoadTriggerInfo() {
    TriggersQuery* nowQuery = new TriggersQuery();
    connect(nowQuery, &TriggersQuery::receive, this, [=](QVariant varValue) {
        QVector<QString> triggers = varValue.value<QVector<QString>>();
        for(int i = 0; i < triggers.size(); i++) {
            QMessageBox::information(this, "提醒", triggers[i]);
            QLabel* nowLabel = new QLabel(triggers[i], this);
            nowLabel->setStyleSheet("background-color:#ffffb3");
            infoContainer->addWidget(nowLabel, true);
        }
    });
}
