#include "activitypage.h"
#include "loginpage.h"
activityInfoWidget::activityInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent),
        activityType(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/, "", 0, this))
{
    setStyleSheet("background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    infoWidget = new QWidget(this);
    infoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(15);
    QFont descFont = QFont("Corbel Light", 13);
    QFontMetrics descm(descFont);
    descFont.setStyleStrategy(QFont::PreferAntialias);
    descLabel = new QLabel("[活动]" + info[0], infoWidget);
    descLabel->setFont(descFont);
    descLabel->setFixedHeight(descm.lineSpacing());
    descLabel->setStyleSheet("color: black");
    infoLayout->addWidget(descLabel);

    QFont detailFont = QFont("Corbel Light", 10);
    QFontMetrics detailm(detailFont);
    detailFont.setStyleStrategy(QFont::PreferAntialias);
    detailLabel = new QLabel("[内容]" + info[1] + "       [地点]" + info[2] + "     [时间]" + info[3], infoWidget);
    detailLabel->setFont(detailFont);
    detailLabel->setFixedHeight(detailm.lineSpacing());
    detailLabel->setStyleSheet("color: gray");
    infoLayout->addWidget(detailLabel);
    activityType->setFixedSize(30, 30);
}

void activityInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void activityInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked();
}

void activityInfoWidget::modify(QVector<QString> info) {
    descLabel->setText("#内容#" + info[0]);
    detailLabel->setText("#地点#" + info[1] + "       #时间#" + info[2]);
    activityType->setPixmap(info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg");
}

void activityInfoWidget::resizeEvent(QResizeEvent *event) {
    infoWidget->resize(this->width() - activityType->width() - 2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    activityType->move(this->width() - activityType->width() - margin, this->height() / 2 - activityType->height() / 2);
}


activityAddPage::activityAddPage(int radius, int type, int width, int height, QString name, QWidget* parent, int posy) :
        SlidePage(radius, type, width, height, name, parent, posy)
{
    title = new textInputItem("标题：", this);
    description = new textInputItem("内容：", this);
    place = new textInputItem("地点：", this);
    time = new textInputItem("时间：", this);
    activityBar = new QWidget(this);
    QHBoxLayout* activityLayout = new QHBoxLayout(activityBar);
    activityLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    activityLayout->setSpacing(10);
    activityLayout->setContentsMargins(0, 0, 0, 0);

    bigIconButton* alarmOn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", 0, activityBar);
    alarmOn->setFixedSize(30, 30);
    activityLayout->addWidget(alarmOn);
    bigIconButton* alarmOff = new bigIconButton(1, ":/icons/icons/alarm_off.svg", "", 0, activityBar);
    alarmOff->setFixedSize(30, 30);
    frequency = new textInputItem("频率：", activityBar);
    activityLayout->addWidget(frequency);

    bigIconButton* TypeBtn = new bigIconButton(1, ":/icons/icons/personal-activity.svg", "", 0, activityBar);
    TypeBtn->setFixedSize(30, 30);
    activityLayout->addWidget(TypeBtn);
    connect(TypeBtn, &bigIconButton::clicked, this, [=] {
        if(isPersonal) {
            TypeBtn->setPixmap(":/icons/icons/group-activity.svg");
            isPersonal = false;
        } else {
            TypeBtn->setPixmap(":/icons/icons/personal-activity.svg");
            isPersonal = true;
        }
    });

    connect(alarmOn, &bigIconButton::clicked, this, [=] {
        activityLayout->removeWidget(alarmOn);
        alarmOn->hide();
        activityLayout->insertWidget(0, alarmOff);
        alarmOff->show();
        alarm = false;
    });
    connect(alarmOff, &bigIconButton::clicked, this, [=] {
        activityLayout->removeWidget(alarmOff);
        alarmOff->hide();
        activityLayout->insertWidget(0, alarmOn);
        alarmOn->show();
        alarm = true;
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
    AddContent(activityBar);
    AddContent(time);
    AddContent(place);
    AddContent(description);
    AddContent(title);
}

QVector<QString> activityAddPage::collectMsg() {
    QVector<QString> tmp;
    tmp.push_back(title->value());
    tmp.push_back(description->value());
    tmp.push_back(place->value());
    tmp.push_back(time->value());
    tmp.push_back(isPersonal ? "true" : "false");
    tmp.push_back(alarm ? "true" : "false");
    tmp.push_back(frequency->value());
    return tmp;
}

activityListWidget::activityListWidget(QString name, QVector<bigIconButton*> icons, QWidget* p, QWidget* parent) :
        QWidget(parent),
        extraIcons(icons),
        slideParent(p)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
//    setStyleSheet("Height:540");
//    setFixedHeight(540);
    titleWidget = new QWidget(this);
    titleWidget->setStyleSheet("background-color:rgb(200, 200, 200); border-radius:0px;");
    QFont textFont = QFont("Corbel Light", 24);
    QFontMetrics fm(textFont);
    nameLabel = new QLabel(name, titleWidget);
    textFont.setStyleStrategy(QFont::PreferAntialias);
    nameLabel->setFont(textFont);
    nameLabel->setFixedHeight(fm.lineSpacing());
    for(int i = 0; i < extraIcons.size(); i++) {
        extraIcons[i]->setParent(titleWidget);
        extraIcons[i]->setFixedSize(titleHeight - 10, titleHeight - 10);
        connect(extraIcons[i], &bigIconButton::clicked, this, [=] {emit clicked(i);});
    }
    container = new ScrollAreaCustom(false, this);

    connect(icons[0], &bigIconButton::clicked, this, [=] {
        activityAddPage* newPage = new activityAddPage(12, 1, 300, 0, "创建新活动", slideParent);
        emit addPage(newPage);
        connect(newPage, &activityAddPage::deliver, this, [=](QVector<QString> s) {
            activityWidget* newWidget = new activityWidget(s, this);
            addContent(newWidget);
            connect(newWidget, &activityWidget::clicked, newPage, &SlidePage::slideIn);
            connect(newPage, &activityAddPage::modify, newWidget, [=](QVector<QString> s) {
                newWidget->modify(s);
            });
            pageList.push_back(newPage);
        });
        newPage->slideIn();
    });
    connect(this, &activityListWidget::addReceived, this, [=](QVector<QString> s) {
        activityAddPage* newPage = new activityAddPage(12, 1, 300, 0, "创建新活动", slideParent);
        emit addPage(newPage);
        emit newPage->deliver(s);
        connect(newPage, &activityAddPage::deliver, this, [=](QVector<QString> s) {
            activityWidget* newWidget = new activityWidget(s, this);
            addContent(newWidget);
            connect(newWidget, &activityWidget::clicked, newPage, &SlidePage::slideIn);
            connect(newPage, &activityAddPage::modify, newWidget, [=](QVector<QString> s) {
                newWidget->modify(s);
            });
            pageList.push_back(newPage);
        });
        //newPage->slideIn();
    });
}

void activityListWidget::resizeEvent(QResizeEvent*){
    titleWidget->resize(width(), titleHeight);
    titleWidget->move(0, 0);
    nameLabel->move(margin, titleHeight / 2 - nameLabel->height() / 2);
    int prex = width() - margin - spacing;
    for(int i = extraIcons.size() - 1; i >= 0; i--) {
        extraIcons[i]->move(prex - extraIcons[i]->width(), titleHeight / 2 - extraIcons[i]->height() / 2);
        prex -= extraIcons[i]->width() + spacing;
    }
    container->resize(width(), height() - titleHeight + overlap);
    container->move(0, titleHeight - overlap);
}

activityWidget::activityWidget(QVector<QString> info, QWidget* parent) :
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
    infoWidget = new activityInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &activityInfoWidget::clicked, this, [=] {emit clicked();});
}

void activityWidget::resizeEvent(QResizeEvent*) {
    bgWidget->resize(this->size());
}

ActivityPage::ActivityPage(QWidget* parent):
        QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    itemWidget = new QWidget(this);
    itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        itemWidget->setStyleSheet("border:1px solid gray;background-color:green");
    QHBoxLayout* itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(30, 30, 30, 30);
    itemLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    itemLayout->setSpacing(10);
    //活动列表 可以实现添加功能
    QWidget* listWidget = new QWidget(itemWidget);
    listWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    listWidget->setFixedWidth(400);
    QHBoxLayout* listLayout = new QHBoxLayout(listWidget);
    listLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    listLayout->setSpacing(15);
    listLayout->setContentsMargins(0,0,0,0);


    QWidget* eventWidget = new QWidget(itemWidget);
    eventWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    eventWidget->setFixedWidth(400);
    QVBoxLayout* eventLayout = new QVBoxLayout(eventWidget);
    eventLayout->setAlignment(Qt::AlignTop);
    eventLayout->setSpacing(15);
    eventLayout->setContentsMargins(0, 0, 0, 0);

    searchBar = new QWidget(eventWidget);
    searchBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchBar->setFixedHeight(35);
    searchBar->setFixedWidth(380);
//                    searchBar->setStyleSheet("border:1px solid gray;background-color:transparent; border-radius:6");
    QHBoxLayout* searchLayout = new QHBoxLayout(searchBar);
    searchLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    searchLayout->setSpacing(10);
    searchLayout->setContentsMargins(0, 0, 3, 0);
    QStringList selectList;
    selectList<<"名称"<<"地点"<<"时间"<<"类型"<<"成员";
    ComboBox* selections = new ComboBox(searchBar);
    selections->setFixedWidth(100);
//                        selections->setStyleSheet("border-radius: 3");
    selections->addItems(selectList);
    textInputItem* activitySearch = new textInputItem("活动", searchBar);
    bigIconButton* searchActivity = new bigIconButton(1, ":/icons/icons/search.svg", "", 6, searchBar);
    searchActivity->setFixedSize(30,30);

    /*TODO connect */

    searchLayout->addWidget(activitySearch);
    searchLayout->addWidget(selections);
    searchLayout->addWidget(searchActivity);
    eventLayout->addWidget(searchBar);

    //itemList = new ScrollAreaCustom(false, eventWidget);
    //itemList->setFixedWidth(400);
    //eventLayout->addWidget(itemList);

    QVector<bigIconButton*> iconVec;
    iconVec.push_back(new bigIconButton(9, ":/icons/icons/add.svg"));
    activityListWidget* activityList = new activityListWidget("activity", iconVec, itemWidget, eventWidget);
    connect(activityList, &activityListWidget::addPage, this, [=](activityAddPage* page){
        pageList.push_back(page);
    });

    eventLayout->addWidget(activityList);
    QVector<QWidget*> items;
//                    connect(iconVec[0], &bigIconButton::clicked, this, [=]{
//                        activityAddPage* newPage = new activityAddPage(12,1,300,0,"Create an activity", slideParent);
//                        emit addPage(newPage);
//                        connect(newPage, &activityAddPage::deliver, this, [=](QVector<Qstring> s){
//                            activity
//                        })
//                        activityInfo = new activityInfoWidget(info, this);
//                        eventLayout->addWidget(activityInfo);

//                    })



    itemList->addWidgets(items);
    itemLayout->addWidget(eventWidget);
    //活动详情信息
    QWidget* itemInfoTable = new QWidget(itemWidget);
    itemInfoTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    itemInfoTable->setStyleSheet("border:1px solid gray;background-color:dark blue");
    itemLayout->addWidget(itemInfoTable);
    mainLayout->addWidget(itemWidget);
    ActivityQuery* query = new ActivityQuery(studentId);
    connect(query, &ActivityQuery::receive, this, [=](QVariant varValue){
        QVector<ActivityResult*> activityResult = varValue.value<QVector<ActivityResult*>>();
        for(int i = 0; i < activityResult.size(); i++){
            QVector<QString> info;
            info.push_back(activityResult[i]->name);
            info.push_back(activityResult[i]->name);
            info.push_back(activityResult[i]->place);
            info.push_back(activityResult[i]->time);
            info.push_back("true");
            info.push_back("true");
            info.push_back("1");

            //info.push_back

            //activityWidget* newAct = new activityWidget(info, itemWidget);
            //activityList->addContent(newAct);
            emit activityList->addReceived(info);
            /*activityAddPage* newPage = new activityAddPage(12, 1, 300, 0, "创建新活动", itemWidget);
            emit activityList->addPage(newPage);
            emit newPage->deliver(info);
            newAct->modify(info);
            connect(newAct, &activityWidget::clicked, newPage, &SlidePage::slideIn);
            connect(newPage, &activityAddPage::deliver, this, [=](QVector<QString> s) {
                //activityWidget* newWidget = new activityWidget(s, this);
                activityList->addContent(newAct);
                connect(newAct, &activityWidget::clicked, newPage, &SlidePage::slideIn);
                connect(newPage, &activityAddPage::modify, newAct, [=](QVector<QString> s) {
                    newAct->modify(s);
                });
                pageList.push_back(newPage);
            });
            pageList.push_back(newPage);*/
        }
    });

}
void ActivityPage::resizeEvent(QResizeEvent*) {
    itemWidget->resize(this->size());
    for(int i = 0; i < pageList.size(); i++) {
        pageList[i]->resize(pageList[i]->width() - 1, pageList[i]->Type() == SlidePage::EXPANDING ? itemWidget->height() : pageList[i]->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}

