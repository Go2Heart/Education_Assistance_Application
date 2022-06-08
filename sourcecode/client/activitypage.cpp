#include "activitypage.h"
#include "loginpage.h"
activityInfoWidget::activityInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent),
        activityType(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/, "", 0, this))
{
    this->info = info;
    id = info[4];
    qDebug() << "id" << id;
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
    detailLabel = new QLabel("[内容]" + info[1] + "     [地点]" + info[2] + "     [时间]" + info[3], infoWidget);
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
    this->info = info;
    descLabel->setText("[活动]" + info[0]);
    detailLabel->setText("[内容]" + info[1]+"     [地点]" + info[2] + "       [时间]" + info[3]);
    //activityType->setPixmap( info[4] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/);
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

activityListWidget::activityListWidget(QString name, QVector<bigIconButton*> icons, QWidget* p, activityDetailWidget* detailWidget,QWidget* parent) :
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
            emit newActivity(newWidget);
            addContent(newWidget);
            /* for details */
            connect(newWidget, &activityWidget::clicked, this, [=] {
                QVector<QString> tmp = newWidget->getInfo();
                detailWidget->setActivity(newWidget);
                emit detailWidget->showDetail(tmp);
            });
            connect(detailWidget, &activityDetailWidget::modify, this, [=](activityWidget* activity) {
                qDebug() << "modify";
                QVector<QString> tmp = detailWidget->getLines();
                detailWidget->getActivity()->modify(tmp);
            });
            pageList.push_back(newPage);
        });
        newPage->slideIn();
    });
    //for Server message
    connect(this, &activityListWidget::addReceived, this, [=](QVector<QString> s) {
        /*@todo  How to create slidePage for Server message?*/
        //activityAddPage* newPage = new activityAddPage(12, 1, 300, 500, "创建新活动", slideParent);
        //emit addPage(newPage);
        //QLabel *title = new QLabel("s[0]", newPage);
        //newPage->AddContent(title);
        //emit newPage->(s);
        //connect(newPage, &activityAddPage::, this, [=](QVector<QString> s) {
        activityWidget* newWidget = new activityWidget(s, this);
        addContent(newWidget);
        connect(newWidget, &activityWidget::clicked, this, [=]() {
            emit showDetail(newWidget);

        });
        //connect(newPage, &activityAddPage::modify, newWidget, [=](QVector<QString> s) {
        //     newWidget->modify(s);
        // });
       //pageList.push_back(newPage);
        //});
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

activityDetailWidget::activityDetailWidget(QWidget* parent) : QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setAlignment(Qt::AlignVCenter);
    this->setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("Height:300; Width:200;");
    title=new textInputItem("标题：", this);
    description = new textInputItem("内容：", this);
    place = new textInputItem("地点：",this);
    time = new textInputItem("时间：", this);
    textButton* modifyBtn = new textButton("Modify!", this);
    connect(modifyBtn, &textButton::clicked, this, [=]{
        qDebug() << "modify1";
        currentActivity->modify(getLines());
        //emit modify(getActivity());
    });
    mainLayout->addWidget(title);
    mainLayout->addWidget(description);
    mainLayout->addWidget(place);
    mainLayout->addWidget(time);
    mainLayout->addWidget(modifyBtn);
}
QVector<QString> activityDetailWidget::collectMsg() {
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

void activityDetailWidget::showDetail(QVector<QString> info) {
    title->setValue(info[0]);
    description->setValue(info[1]);
    place->setValue(info[2]);
    time->setValue(info[3]);
    //isPersonal = info[4].toInt();
    //alarm = info[5].toInt();
    //frequency->setValue(info[6]);

}

activityFileDeliver::activityFileDeliver(QWidget* parent) : QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("Height:400; Width:500; background-color:transparent");
    this->setLayout(mainLayout);
//    singleSelectGroupVertical* fileType = new singleSelectGroupVertical("请选择", this);
//        fileType->setFixedHeight(60);
//        selectionItem* material = new selectionItem("上传资料", "", this);
//        selectionItem* homework = new selectionItem("上交作业", "", this);
//        fileType->AddItem(material);
//        fileType->AddItem(homework);
    select = new textButton("选择文件", this);
    upload = new textButton("上传文件", this);
    QWidget* listWidget = new QWidget(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    setStyleSheet("Height:400; Width:400; background-color:transparent");
    QVBoxLayout* listLayout = new QVBoxLayout(listWidget);
    listLayout->setAlignment(Qt::AlignTop);
        fileList = new ScrollListContainer(listWidget);
        fileList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        fileList->setStyleSheet("Height:500; Width:600; ");
//        fileList->AddWidget();
    listLayout->addWidget(fileList);

    connect(select, &textButton::clicked, this,[=]{
        QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), "", QStringLiteral("All Files(*.*);;docs(*.doc *.docx);;PDF Files(*.pdf);;code Files(*.c *.cpp *h. *.hpp *.html *.css *.js *.ts);;images(*.jpg;;*.jepg;;*.png;;*.bmp)"));
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QByteArray filestring = file.readAll();
        filesToSubmit.push_back(filestring.toStdString());
        QFileInfo fileInfo(filePath);
        QLabel* tmp = new QLabel(fileInfo.fileName(),fileList);
        fileNames.push_back(fileInfo.fileName());
        fileList->AddWidget(tmp,true);
        file.close();

    });

    connect(upload, &textButton::clicked, this, [=]{
        emit deliver(filesToSubmit);
        for(int i = 0; i < fileNames.size(); i++) {
            qDebug() << fileNames[i];
            fileUploader = new FileUpload(id,fileNames[i], filesToSubmit[i]);
        }
        fileNames.clear();
        fileList->clear();
        filesToSubmit.clear();

    });
    mainLayout->addWidget(select);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(upload);
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

    /* detail layout*/

    QWidget* detailWidget = new QWidget(itemWidget);
    detailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    detailWidget->setStyleSheet("border:1px solid gray;background-color:dark blue");

    QVBoxLayout* detailLayout = new QVBoxLayout(detailWidget);
    detailLayout->setAlignment(Qt::AlignTop);
    detailLayout->setContentsMargins(5, 5, 5, 5);
    detailLayout->setSpacing(5);
    QWidget* detailTab = new QWidget(detailWidget);
    detailTab->setStyleSheet("border:0px transparent gray;background-color:transparent");
    detailTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    detailTab->setFixedHeight(22);
    QHBoxLayout *tabLayout = new QHBoxLayout(detailTab);
    tabLayout->setSpacing(0);
    tabLayout->setAlignment(Qt::AlignTop);
    tabLayout->setContentsMargins(5, 0, 5, 0);
    textButton* detailTabButton1 = new textButton("活动详情", detailTab);
    tabLayout->addWidget(detailTabButton1);

    textButton* detailTabButton2 = new textButton("材料提交", detailTab);
    tabLayout->addWidget(detailTabButton2);

    //textButton* detailTabButton3 = new textButton("作业提交", detailTab);
    //tabLayout->addWidget(detailTabButton3);

    detailLayout->addWidget(detailTab);

    QWidget* detailArea = new QWidget(detailWidget);
    detailArea->setStyleSheet("Height:400; Width:300; border: 5px transparent;background-color:#f6eef2");
    detailArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* areaLayout = new QVBoxLayout(detailArea);
    areaLayout->setAlignment(Qt::AlignTop);
    areaLayout->setContentsMargins(5,0,5,0);

    activityDtl = new activityDetailWidget(detailArea);
    areaLayout->addWidget(activityDtl);
//        activityDtl->hide();

    QWidget* materialDlvr = new QWidget(detailArea);
    materialDlvr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    materialDlvr->setStyleSheet("Height:400; Width:400");
    QVBoxLayout* MDlvrLayout = new QVBoxLayout(materialDlvr);
    MDlvrLayout->setAlignment(Qt::AlignCenter);

        fileDlvr = new activityFileDeliver(materialDlvr);
        MDlvrLayout->addWidget(fileDlvr);


    areaLayout->addWidget(materialDlvr);
    materialDlvr->hide();
    connect(detailTabButton1, &textButton::clicked, this, [=]{
        materialDlvr->hide();
        activityDtl->show();
        //fileDlvr->setActivity(activityDtl->getActivity());
        qDebug() << fileDlvr->getId();
        detailTabButton1->setEnabled(false);
    });
    connect(detailTabButton2, &textButton::clicked, this, [=]{
        activityDtl->hide();
        materialDlvr->show();
        //fileDlvr->setActivity(activityDtl->getActivity());
        qDebug() << fileDlvr->getId();
        detailTabButton1->setEnabled(true);

    });

    detailLayout->addWidget(detailArea);

    /*end of detail layout*/

    activityListWidget* activityList = new activityListWidget("activity", iconVec, itemWidget, activityDtl,eventWidget);
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
    itemLayout->addWidget(detailWidget);
    mainLayout->addWidget(itemWidget);
    ActivityQuery* query = new ActivityQuery(studentId);
    connect(activityList, &activityListWidget::newActivity, this, [=](activityWidget* activityWidget) {
        QVector<QString> info = activityWidget->getInfo();
        //activityUpload* upload = new activityUpload(info, studentId);
        ActivityUpload* upload = new ActivityUpload(info, studentId);
    });

    /*Detail Widget*/



    connect(query, &ActivityQuery::receive, this, [=](QVariant varValue){
        QVector<ActivityResult*> activityResult = varValue.value<QVector<ActivityResult*>>();
        for(int i = 0; i < activityResult.size(); i++){
            QVector<QString> info;
            info.push_back(activityResult[i]->name);
            info.push_back(activityResult[i]->name);
            info.push_back(activityResult[i]->place);
            info.push_back(activityResult[i]->time);
            info.push_back(activityResult[i]->id);
            info.push_back("true");
            info.push_back("true");
            info.push_back("1");

            //info.push_back
            activityWidget* newWidget = new activityWidget(info, this);
            activityList->addContent(newWidget);
            connect(newWidget, &activityWidget::clicked, this, [=](){
                activityDtl->showDetail(newWidget->getInfo());
                activityDtl->setActivity(newWidget);
                fileDlvr->setActivity(newWidget);
            });
        }
    });
    connect(searchActivity, &bigIconButton::clicked, this, [=] {
        search = new ActivitySearch(activitySearch->value(), selections->currentIndex());
        connect(search, &ActivitySearch::receive, this, [=](QVariant varValue){
            activityList->cleanContent();
            QVector<ActivityResult*> activityResult = varValue.value<QVector<ActivityResult*>>();
            for(int i = 0; i < activityResult.size(); i++){
                QVector<QString> info;
                info.push_back(activityResult[i]->name);
                info.push_back(activityResult[i]->name);
                info.push_back(activityResult[i]->place);
                info.push_back(activityResult[i]->time);
                info.push_back(activityResult[i]->id);
                info.push_back("true");
                info.push_back("true");
                info.push_back("1");

                //info.push_back
                activityWidget* newWidget = new activityWidget(info, this);
                activityList->addContent(newWidget);
                connect(newWidget, &activityWidget::clicked, this, [=](){
                    activityDtl->showDetail(newWidget->getInfo());
                    activityDtl->setActivity(newWidget);
                    fileDlvr->setActivity(newWidget);
                });
            }
        });
    });

}
void ActivityPage::resizeEvent(QResizeEvent*) {
    itemWidget->resize(this->size());
    for(int i = 0; i < pageList.size(); i++) {
        pageList[i]->resize(pageList[i]->width() - 1, pageList[i]->Type() == SlidePage::EXPANDING ? itemWidget->height() : pageList[i]->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}
