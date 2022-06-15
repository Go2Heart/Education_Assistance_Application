//
// Created by Go2Heart on 2022/6/13.
//
#include <QFileDialog>
#include "teacherpage.h"
#include "loginpage.h"
#include "global.h"

teacherClassInfoWidget::teacherClassInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent),
        teacherClassType(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/, "", "", 0, 0, this))
{
    this->info = info;
    id = info[5];
    int num = info[6].toInt();
    qDebug() << "num:" << num;
    for (int j = 0; j < num; j++) {
        qDebug() << info[7 + j];
        downloadInfo.push_back(info[7 + j]);
    }

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
    descLabel = new QLabel("[课程]" + info[0], infoWidget);
    descLabel->setFont(descFont);
    descLabel->setFixedHeight(descm.lineSpacing());
    descLabel->setStyleSheet("color: black");
    infoLayout->addWidget(descLabel);

    QFont detailFont = QFont("Corbel Light", 10);
    QFontMetrics detailm(detailFont);
    detailFont.setStyleStrategy(QFont::PreferAntialias);
    detailLabel = new QLabel("[教师]" + info[1] + "       [地点]" + info[2] + "     [时间]" + info[3], infoWidget);
    detailLabel->setFont(detailFont);
    detailLabel->setFixedHeight(detailm.lineSpacing());
    detailLabel->setStyleSheet("color: gray");
    infoLayout->addWidget(detailLabel);
    teacherClassType->setFixedSize(30, 30);
}

void teacherClassInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void teacherClassInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked();
}

void teacherClassInfoWidget::resizeEvent(QResizeEvent *event) {
    infoWidget->resize(this->width() - teacherClassType->width() - 2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    teacherClassType->move(this->width() - teacherClassType->width() - margin, this->height() / 2 - teacherClassType->height() / 2);
}

void teacherClassInfoWidget::modify(QVector<QString> info) {
    this->info = info;
    descLabel->setText("[课程]" + info[0]);
    detailLabel->setText("[教师]" + info[1]+"     [地点]" + info[2] + "       [时间]" + info[3]);
    //activityType->setPixmap( info[4] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/);
}

teacherClassListWidget::teacherClassListWidget(QString name, QVector<bigIconButton*> icons, QWidget* p, teacherClassDetailWidget *detailWidget, QWidget* parent) :
        QWidget(parent),
        extraIcons(icons),
        slideParent(p)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //setFixedHeight(maxHeight - titleHeight + overlap);
    titleWidget = new QWidget(this);
    titleWidget->setStyleSheet("background-color:rgb(200, 200, 200); border-radius:0px;");
    QFont textFont = QFont("Corbel Light", 24);
    QFontMetrics fm(textFont);
    nameLabel = new QLabel(name, titleWidget);
    textFont.setStyleStrategy(QFont::PreferAntialias);
    nameLabel->setFont(textFont);
    nameLabel->setFixedHeight(fm.lineSpacing());
    /*for(int i = 0; i < extraIcons.size(); i++) {
        extraIcons[i]->setParent(titleWidget);
        extraIcons[i]->setFixedSize(titleHeight - 10, titleHeight - 10);
        connect(extraIcons[i], &bigIconButton::clicked, this, [=] {emit clicked(i);});
    }*/
    container = new ScrollAreaCustom(false, this);
    connect(icons[0], &bigIconButton::clicked, this, [=] {
        teacherQuizAddPage* newPage = new teacherQuizAddPage(12, 1, 300, 0, "创建新活动", slideParent);
        emit addPage(newPage);
        connect(newPage, &teacherQuizAddPage::deliver, this, [=](QVector<QString> s) {
            teacherClassWidget* newWidget = new teacherClassWidget(s, this);
            emit newActivity(newWidget);
            addContent(newWidget);
            /* for details */
            connect(newWidget, &teacherClassWidget::clicked, this, [=] {
                QVector<QString> tmp = newWidget->getInfo();
                detailWidget->setActivity(newWidget);
                emit detailWidget->showDetail(tmp);
            });
            connect(detailWidget, &teacherClassDetailWidget::modify, this, [=](teacherClassWidget* activity) {
                qDebug() << "modify";
                QVector<QString> tmp = detailWidget->getLines();
                detailWidget->getActivity()->modify(tmp);
            });
            pageList.push_back(newPage);
        });
        newPage->slideIn();
    });
}

void teacherClassListWidget::resizeEvent(QResizeEvent*){
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

teacherClassWidget::teacherClassWidget(QVector<QString> info, QWidget* parent) :
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
    infoWidget = new teacherClassInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &teacherClassInfoWidget::clicked, this, [=] {emit clicked();});
}

void teacherClassWidget::resizeEvent(QResizeEvent*) {
    bgWidget->resize(this->size());
}

TeacherPage::TeacherPage(QWidget* parent):
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
    selectList<<"名称"<<"教师"<<"地点"<<"时间"<<"成员";
    ComboBox* selections = new ComboBox(searchBar);
    selections->setFixedWidth(100);
    selections->addItems(selectList);
    textInputItem* teacherClassSearch = new textInputItem("课程", searchBar);
    bigIconButton* searchteacherClass = new bigIconButton(1, ":/icons/icons/search.svg", "", "", 0, 6, searchBar);
    searchteacherClass->setFixedSize(30, 30);

    searchLayout->addWidget(teacherClassSearch);
    searchLayout->addWidget(selections);
    searchLayout->addWidget(searchteacherClass);
    eventLayout->addWidget(searchBar);

    //itemList = new ScrollAreaCustom(false, eventWidget);
    //itemList->setFixedWidth(400);
    //eventLayout->addWidget(itemList);
    // detail
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
    textButton* detailTabButton1 = new textButton("课程详情", detailTab);
    tabLayout->addWidget(detailTabButton1);

    textButton* detailTabButton2 = new textButton("材料提交", detailTab);
    tabLayout->addWidget(detailTabButton2);

    textButton* detailTabButton3 = new textButton("作业提交", detailTab);
    tabLayout->addWidget(detailTabButton3);

    detailLayout->addWidget(detailTab);

    QWidget* detailArea = new QWidget(detailWidget);
    detailArea->setStyleSheet("Height:400; Width:300; border: 5px transparent;background-color:#f6eef2");
    detailArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* areaLayout = new QVBoxLayout(detailArea);
    areaLayout->setAlignment(Qt::AlignTop);
    areaLayout->setContentsMargins(5,0,5,0);

    activityDtl = new teacherClassDetailWidget(detailArea);

    areaLayout->addWidget(activityDtl);
//        activityDtl->hide();

    QWidget* materialDlvr = new QWidget(detailArea);
    materialDlvr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //materialDlvr->setStyleSheet("Height:400; Width:400");
    QVBoxLayout* MDlvrLayout = new QVBoxLayout(materialDlvr);
    MDlvrLayout->setAlignment(Qt::AlignCenter);

    fileDlvr = new teacherClassFileDeliver(materialDlvr);
    MDlvrLayout->addWidget(fileDlvr);

    areaLayout->addWidget(materialDlvr);
    materialDlvr->hide();

    QWidget* homeworkDlvr = new QWidget(detailArea);
    homeworkDlvr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //homeworkDlvr->setStyleSheet("Height:400; Width:400");
    QVBoxLayout* HDlvrLayout = new QVBoxLayout(homeworkDlvr);
    HDlvrLayout->setAlignment(Qt::AlignCenter);

    homework = new teacherClassHomeworkWidget(homeworkDlvr);
    HDlvrLayout->addWidget(homework);

    areaLayout->addWidget(homeworkDlvr);
    homeworkDlvr->hide();


    connect(detailTabButton1, &textButton::clicked, this, [=]{
        materialDlvr->hide();
        homeworkDlvr->hide();
        activityDtl->show();
        //fileDlvr->setActivity(activityDtl->getActivity());
        //detailTabButton1->setEnabled(false);

    });
    connect(detailTabButton2, &textButton::clicked, this, [=]{
        activityDtl->hide();
        homeworkDlvr->hide();
        materialDlvr->show();
        //fileDlvr->setActivity(activityDtl->getActivity());
        //detailTabButton1->setEnabled(true);
    });
    connect(detailTabButton3, &textButton::clicked, this, [=]{
        homework->cleanContent();
        emit checkHomework(studentId, activityDtl->getActivity()->getInfoWidget()->getId().toInt());
        homework->homeworkPost = new HomeworkPost(7, "作业1");
        qDebug() << studentId << "teacherClass id"<< activityDtl->getActivity()->getInfoWidget()->getId();
        activityDtl->hide();
        materialDlvr->hide();
        homeworkDlvr->show();

    });
    connect(this, &TeacherPage::checkHomework, this, [=](int studentId, int teacherClassId) {
        homework->homeworkQuery = new HomeworkQuery(studentId, teacherClassId);
        connect(homework->homeworkQuery, &HomeworkQuery::receive, this, [=](QVariant varValue){
            QVector<HomeworkResult*> result = varValue.value<QVector<HomeworkResult*>>();
            for(int i = 0; i < result.size(); i++){
                QVector<QString> info;
                info.push_back(QString::number(result[i]->id));
                info.push_back(QString::number(result[i]->finished));
                info.push_back(result[i]->desc);

                teacherHomeworkWidget *infoWidget = new teacherHomeworkWidget(info, homework);
                homework->addContent(infoWidget);
                connect(infoWidget, &teacherHomeworkWidget::clicked, this, [=](int homeworkId){
                    homework->chooseId = homeworkId;
                });
            }
        });
    });

    QVector<bigIconButton*> iconVec;
    iconVec.push_back(new bigIconButton(9, ":/icons/icons/add.svg"));
    detailLayout->addWidget(detailArea);
    teacherClassList = new teacherClassListWidget("teacherClass", iconVec, itemWidget, activityDtl,eventWidget);


    eventLayout->addWidget(teacherClassList);
    QVector<QWidget*> items;
//                    connect(iconVec[0], &bigIconButton::clicked, this, [=]{
//                        teacherClassAddPage* newPage = new teacherClassAddPage(12,1,300,0,"Create an teacherClass", slideParent);
//                        emit addPage(newPage);
//                        connect(newPage, &teacherClassAddPage::deliver, this, [=](QVector<Qstring> s){
//                            teacherClass
//                        })
//                        teacherClassInfo = new teacherClassInfoWidget(info, this);
//                        eventLayout->addWidget(teacherClassInfo);

//                    })



    itemList->addWidgets(items);
    itemLayout->addWidget(eventWidget);
    itemLayout->addWidget(detailWidget);
    //活动详情信息

    mainLayout->addWidget(itemWidget);
    connect(searchteacherClass, &bigIconButton::clicked, this, [=]{
        /*
         * if(teacherClassSearch->value() == ""){
            teacherClassList->cleanContent();
            teacherClassQuery* query = new teacherClassQuery(studentId);
            connect(query, &teacherClassQuery::receive, this, [=](QVariant varValue){
                QVector<teacherClassResult*> teacherClassResult = varValue.value<QVector<teacherClassResult*>>();
                for(int i = 0; i < teacherClassResult.size(); i++){
                    QVector<QString> info;
                    info.push_back(teacherClassResult[i]->name);
                    info.push_back(teacherClassResult[i]->teacher);
                    info.push_back(teacherClassResult[i]->place);
                    info.push_back(teacherClassResult[i]->time);
                    info.push_back(teacherClassResult[i]->QQ);
                    info.push_back(QString::asprintf("%d", teacherClassResult[i]->id));
                    info.push_back(QString::number(teacherClassResult[i]->fileNames.size()));
                    for(int j = 0; j < teacherClassResult[i]->fileNames.size(); j++){
                        info.push_back(teacherClassResult[i]->fileNames[j]);
                    }
                    //info.push_back

                    teacherClassWidget * newteacherClass = new teacherClassWidget(info, itemWidget);
                    connect(newteacherClass, &teacherClassWidget::clicked, this, [=]{
                        activityDtl->showDetail(info);
                        activityDtl->setActivity(newteacherClass);
                        fileDlvr->setActivity(newteacherClass);
                        fileDlvr->setDownloadInfo(newteacherClass->getInfoWidget()->getDownloadInfo());
                        emit fileDlvr->download();
                        homework->setteacherClassId(newteacherClass);
                    });
                    teacherClassList->addContent(newteacherClass);
                }
            });
            return;
        }
        */
        search = new ClassSearch(teacherClassSearch->value(), selections->currentIndex());
        connect(search, &ClassSearch::receive, this, [=](QVariant varValue){
            teacherClassList->cleanContent();
            QVector<ClassResult*> teacherClassResult = varValue.value<QVector<ClassResult*>>();
            for(int i = 0; i < teacherClassResult.size(); i++){
                QVector<QString> info;
                info.push_back(teacherClassResult[i]->name);
                info.push_back(teacherClassResult[i]->teacher);
                info.push_back(teacherClassResult[i]->place);
                info.push_back(teacherClassResult[i]->time);
                info.push_back(teacherClassResult[i]->QQ);
                info.push_back(QString::asprintf("%d", teacherClassResult[i]->id));
                info.push_back(QString::number(teacherClassResult[i]->fileNames.size()));
                for(int j = 0; j < teacherClassResult[i]->fileNames.size(); j++){
                    info.push_back(teacherClassResult[i]->fileNames[j]);
                }
                info.push_back(QString::number(teacherClassResult[i]->examBegin.week));
                info.push_back(QString::number(teacherClassResult[i]->examBegin.day));
                info.push_back(teacherClassResult[i]->examBegin.ToString());
                info.push_back(teacherClassResult[i]->examEnd.ToString());
                info.push_back(teacherClassResult[i]->examPlace);
                //info.push_back

                teacherClassWidget * newteacherClass = new teacherClassWidget(info, itemWidget);
                connect(newteacherClass, &teacherClassWidget::clicked, this, [=]{
                    activityDtl->showDetail(info);
                    activityDtl->setActivity(newteacherClass);
                    fileDlvr->setActivity(newteacherClass);
                    fileDlvr->setDownloadInfo(newteacherClass->getInfoWidget()->getDownloadInfo());
                    emit fileDlvr->download();
                    homework->setteacherClassId(newteacherClass);
                });
                teacherClassList->addContent(newteacherClass);
            }
        });
    });
}

void TeacherPage::resizeEvent(QResizeEvent*) {
    itemWidget->resize(this->size());
    for(int i = 0; i < pageList.size(); i++) {
        pageList[i]->resize(pageList[i]->width() - 1, pageList[i]->Type() == SlidePage::EXPANDING ? itemWidget->height() : pageList[i]->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}

void TeacherPage::LoadInfo() {
    ClassQuery* query = new ClassQuery();
    connect(query, &ClassQuery::receive, this, [=](QVariant varValue){
        QVector<ClassResult*> teacherClassResult = varValue.value<QVector<ClassResult*>>();
        for(int i = 0; i < teacherClassResult.size(); i++){
            QVector<QString> info;
            info.push_back(teacherClassResult[i]->name);
            info.push_back(teacherClassResult[i]->teacher);
            info.push_back(teacherClassResult[i]->place);
            info.push_back(teacherClassResult[i]->time);
            info.push_back(teacherClassResult[i]->QQ);
            info.push_back(QString::asprintf("%d", teacherClassResult[i]->id));
            info.push_back(QString::number(teacherClassResult[i]->fileNames.size()));
            for(int j = 0; j < teacherClassResult[i]->fileNames.size(); j++){
                info.push_back(teacherClassResult[i]->fileNames[j]);
            }
            //info.push_back

            teacherClassWidget * newteacherClass = new teacherClassWidget(info, itemWidget);
            connect(newteacherClass, &teacherClassWidget::clicked, this, [=]{
                activityDtl->showDetail(info);
                activityDtl->setActivity(newteacherClass);
                fileDlvr->setActivity(newteacherClass);
                fileDlvr->setDownloadInfo(newteacherClass->getInfoWidget()->getDownloadInfo());
                emit fileDlvr->download();
                homework->setteacherClassId(newteacherClass);
                //TODO adding specific teacherClass page
            });
            teacherClassList->addContent(newteacherClass);
        }
    });
}


teacherClassFileDeliver::teacherClassFileDeliver(QWidget *parent):QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color:transparent");
    this->setLayout(mainLayout);
//    singleSelectGroupVertical* fileType = new singleSelectGroupVertical("请选择", this);
//        fileType->setFixedHeight(60);
//        selectionItem* material = new selectionItem("上传资料", "", this);
//        selectionItem* homework = new selectionItem("上交作业", "", this);
//        fileType->AddItem(material);
//        fileType->AddItem(homework);
    select = new textButton("选择文件", this);
    upload = new textButton("上传文件", this);
    downloadButton = new textButton("下载文件", this);
    QWidget* listWidget = new QWidget(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    setStyleSheet("Height:400; Width:400; background-color:transparent");
    QVBoxLayout* listLayout = new QVBoxLayout(listWidget);
    listLayout->setAlignment(Qt::AlignTop);
    fileList = new ScrollAreaCustom(false, listWidget);
    //fileList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    fileList->setFixedWidth(600);
//        fileList->AddWidget();
    listLayout->addWidget(fileList);
    QWidget* downloadWidget = new QWidget(this);
    QVBoxLayout* downloadLayout = new QVBoxLayout(downloadWidget);
    downloadLayout->setAlignment(Qt::AlignTop);
    downloadList = new ScrollAreaCustom(false, downloadWidget);
    //downloadList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    downloadList->setFixedWidth(600);
    downloadLayout->addWidget(downloadList);



    connect(select, &textButton::clicked, this,[=]{
        QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), "", QStringLiteral("All Files(*.*);;docs(*.doc *.docx);;PDF Files(*.pdf);;code Files(*.c *.cpp *h. *.hpp *.html *.css *.js *.ts);;images(*.jpg;;*.jepg;;*.png;;*.bmp)"));
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QByteArray filestring = file.readAll();
        filesToSubmit.push_back(filestring.toStdString());
        QFileInfo fileInfo(filePath);
        QLabel* tmp = new QLabel(fileInfo.fileName(),fileList);
        tmp->setFixedHeight(30);
        fileNames.push_back(fileInfo.fileName());
        fileList->addWidget(tmp,true);
        file.close();

    });

    connect(upload, &textButton::clicked, this, [=]{
        for(int i = 0; i < fileNames.size(); i++) {
            qDebug() << fileNames[i];
            fileUploader = new FileUpload(id,fileNames[i], filesToSubmit[i], studentId, 1);
        }
        fileNames.clear();
        fileList->clear();
        filesToSubmit.clear();
    });
    connect(downloadButton, &textButton::clicked, this, [=]{
        for(int i = 0; i < fileToDownload.size(); i ++) {
            fileDownloader = new FileDownload(id, fileToDownload[i], studentId, 1);
            connect(fileDownloader, &FileDownload::receive, this, [=](QVariant varValue){
                FileResult* fileResult = varValue.value<FileResult*>();
                QString filePath = QFileDialog::getSaveFileName(this, QStringLiteral("保存文件"), "", QStringLiteral("All Files(*.*);;docs(*.doc *.docx);;PDF Files(*.pdf);;code Files(*.c *.cpp *h. *.hpp *.html *.css *.js *.ts);;images(*.jpg;;*.jepg;;*.png;;*.bmp)"));
                QFile file(filePath);
                file.open(QIODevice::WriteOnly);
                QByteArray fileData = QByteArray::fromStdString(fileResult->str);
                file.write(fileData);
                file.close();
            });
        }
    });
    QWidget * splitter = new QWidget(this);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->setStyleSheet("background-color:black");
    splitter->setFixedHeight(3);


    connect(this, &teacherClassFileDeliver::download, this, [=]{
        downloadList->clear();
        for(int i = 0; i < fileToDownload.size(); i++) {

            QLabel* tmp = new QLabel(fileToDownload[i], downloadList);
            downloadList->addWidget(tmp, true);
        }

    });
    mainLayout->addWidget(downloadWidget);
    mainLayout->addWidget(downloadButton);
    mainLayout->addWidget(splitter);
    mainLayout->addWidget(select);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(upload);



}

teacherClassDetailWidget::teacherClassDetailWidget(QWidget *parent) : QWidget(parent){
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setAlignment(Qt::AlignVCenter);
    this->setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("Height:300; Width:200;");
    title=new textInputItem("课程：", this);
    description = new textInputItem("教师：", this);
    place = new textInputItem("地点：",this);
    time = new textInputItem("时间：", this);
    qq = new textInputItem("QQ：", this);
    QWidget* examWidget = new QWidget(this);
    examTime = new QLabel("考试时间：", examWidget);
    QHBoxLayout* examLayout = new QHBoxLayout();
    examLayout->addWidget(examTime);
     week = new textInputItem("周：", examWidget);
    examLayout->addWidget(week);
     day = new textInputItem("天：", examWidget);
    examLayout->addWidget(day);
     begin = new textInputItem("开始：", examWidget);
    examLayout->addWidget(begin);
     end = new textInputItem("结束：", examWidget);
    examLayout->addWidget(end);
    examWidget->setLayout(examLayout);
    examPlace = new textInputItem("考试地点：", this);
    textButton* modifyBtn = new textButton("Modify!", this);

    connect(modifyBtn, &textButton::clicked, this, [=]{
        /*
            QString classBegin = time->value().first(6).last(5);
            QString classEnd = time->value().last(6).first(5);
            Timer* classBeginTimer = new Timer(classBegin.first(2).toInt(), classBegin.last(2).toInt());
            Timer* classEndTimer = new Timer(classEnd.first(2).toInt(), classEnd.last(2).toInt());
            Timer *beginTimer = new Timer( begin->value().first(2).toInt(), begin->value().sliced(3).toInt(), day->value().toInt(), week->value().toInt() );
            Timer *endTimer = new Timer(end->value().first(2).toInt(), end->value().sliced(3).toInt(), day->value().toInt(), week->value().toInt());
        */
        QString classBegin = time->value().left(6).right(5);
        QString classEnd = time->value().right(6).left(5);
        Timer* classBeginTimer = new Timer(classBegin.left(2).toInt(), classBegin.right(2).toInt());
        Timer* classEndTimer = new Timer(classEnd.left(2).toInt(), classEnd.right(2).toInt());
        Timer *beginTimer = new Timer( begin->value().left(2).toInt(), begin->value().right(2).toInt(), day->value().toInt(), week->value().toInt() );
        Timer *endTimer = new Timer(end->value().left(2).toInt(), end->value().right(2).toInt(), day->value().toInt(), week->value().toInt());
        //qDebug() << "w:" << week->value().toInt() << "d:" << day->value().toInt()<< "h:" << begin->value().first(2).toInt() << "m:" << begin->value().sliced(3).toInt();
        //qDebug() << "modify1";
        ClassChange* classChange = new ClassChange(id.toInt(), title->value(), description->value(), place->value(), classBeginTimer->Zip(),
                                                   classEndTimer->Zip(), qq->value(), beginTimer->Zip(), endTimer->Zip(),
                                                   examPlace->value());
        currentActivity->modify(getLines());
        //emit modify(getActivity());
    });

    mainLayout->addWidget(title);
    mainLayout->addWidget(description);
    mainLayout->addWidget(place);
    mainLayout->addWidget(time);
    mainLayout->addWidget(qq);
    mainLayout->addWidget(examWidget);
    mainLayout->addWidget(examPlace);
    mainLayout->addWidget(modifyBtn);
}
QVector<QString> teacherClassDetailWidget::collectMsg() {
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

void teacherClassDetailWidget::showDetail(QVector<QString> info) {
    title->setValue(info[0]);
    description->setValue(info[1]);
    place->setValue(info[3]);
    time->setValue(info[2]);
    qq->setValue(info[4]);
    week->setValue(info[info.size() - 5]);
    day->setValue(info[info.size() - 4]);
    begin->setValue(info[info.size() - 3]);
    end->setValue(info[info.size() - 2]);
    examPlace->setValue(info[info.size() - 1]);
}
teacherClassHomeworkWidget::teacherClassHomeworkWidget(QWidget *parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    searchBar = new QWidget(this);
    QHBoxLayout *searchLayout = new QHBoxLayout(searchBar);
    searchLayout->setContentsMargins(0,0,0,0);
    searchBar->setLayout(searchLayout);
    searchBar->setFixedHeight(40);
    searchBar->setStyleSheet("background-color:rgb(255,255,255);");
    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    searchLayout->setAlignment(Qt::AlignLeft);
    textInputItem* search = new textInputItem("搜索作业", searchBar);
    search->setStyleSheet("background-color:rgb(255,255,255);");
    bigIconButton* searchButton = new bigIconButton(1, ":/icons/icons/search.svg", "", "", 0, 6, searchBar);
    searchButton->setFixedSize(30, 30);
    searchButton->setStyleSheet("background-color:rgb(255,255,255);");
    searchLayout->addWidget(search);
    searchLayout->addWidget(searchButton);
    mainLayout->addWidget(searchBar);

    container = new ScrollAreaCustom(false, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(container);


    textButton* choose = new textButton("添加待提交作业", this);
    choose->setFixedHeight(40);
    mainLayout->addWidget(choose);
    tempHomework = new ScrollAreaCustom(false, this);
    tempHomework->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tempHomework->setFixedHeight(100);
    mainLayout->addWidget(tempHomework);
    textButton* deliver = new textButton("提交作业", this);
    deliver->setFixedHeight(40);
    mainLayout->addWidget(deliver);

    connect(choose, &textButton::clicked, this,[=]{
        QString filePath = QFileDialog::getOpenFileName(this, QStringLiteral("添加待提交"), "", QStringLiteral("All Files(*.*);;docs(*.doc *.docx);;PDF Files(*.pdf);;code Files(*.c *.cpp *h. *.hpp *.html *.css *.js *.ts);;images(*.jpg;;*.jepg;;*.png;;*.bmp)"));
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QByteArray filestring = file.readAll();
        filesToSubmit.push_back(filestring.toStdString());
        QFileInfo fileInfo(filePath);
        QLabel* tmp = new QLabel(fileInfo.fileName(),tempHomework);
        fileNames.push_back(fileInfo.fileName());
        tempHomework->addWidget(tmp,true);
        file.close();
    });

    connect(deliver, &textButton::clicked, this, [=] {
        homeworkUploader = new HomeworkUpload(studentId, teacherClassId, chooseId, fileNames.size(), fileNames, filesToSubmit);
        fileNames.clear();
        filesToSubmit.clear();
        tempHomework->clear();
    });

    connect(searchButton, &bigIconButton::clicked, this, [=]{
        this->cleanContent();
        HomeworkSearch *searchEvent = new HomeworkSearch(studentId, teacherClassId, search->value());
        connect(searchEvent, &HomeworkSearch::receive, this, [=](QVariant varValue) {
            QVector<HomeworkResult*> result = varValue.value<QVector<HomeworkResult*>>();
            for(int i = 0; i < result.size(); i++){
                QVector<QString> info;
                info.push_back(QString::number(result[i]->id));
                info.push_back(QString::number(result[i]->finished));
                info.push_back(result[i]->desc);
                teacherHomeworkWidget *infoWidget = new teacherHomeworkWidget(info, this);
                this->addContent(infoWidget);
                connect(infoWidget, &teacherHomeworkWidget::clicked, this, [=](int homeworkId){
                    this->chooseId = homeworkId;
                });
            }
        });
    });

}

void teacherClassHomeworkWidget::mouseReleaseEvent(QMouseEvent *) {
    emit clicked();
}

void teacherClassHomeworkInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void teacherClassHomeworkInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked(id);
}


void teacherClassHomeworkInfoWidget::resizeEvent(QResizeEvent *event) {
    infoWidget->resize(this->width() -2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
}
teacherClassHomeworkInfoWidget::teacherClassHomeworkInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent)
{
    this->info = info;
    id = info[0].toInt();
    finished = info[1].toInt();
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
    QString isFinished = finished == 1 ? "已完成" : "未完成";
    titleLabel = new QLabel("[作业ID]" + info[0], infoWidget);
    titleLabel->setFont(descFont);
    titleLabel->setFixedHeight(descm.lineSpacing());
    titleLabel->setStyleSheet("color: black");
    infoLayout->addWidget(titleLabel);

    QFont detailFont = QFont("Corbel Light", 10);
    QFontMetrics detailm(detailFont);
    detailFont.setStyleStrategy(QFont::PreferAntialias);
    descriptionLabel = new QLabel("[状态]" + isFinished + "[内容]" + info[2], infoWidget);
    descriptionLabel->setFont(detailFont);
    descriptionLabel->setFixedHeight(detailm.lineSpacing());
    descriptionLabel->setStyleSheet("color: gray");
    infoLayout->addWidget(descriptionLabel);

}

teacherHomeworkWidget::teacherHomeworkWidget(QVector<QString> info, QWidget *parent): QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(100);
    bgWidget = new QWidget(this);
    bgWidget->resize(this->size());
    bgWidget->setStyleSheet("background-color:" + defaultColor + ";border-radius:12px;");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    infoWidget = new teacherClassHomeworkInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &teacherClassHomeworkInfoWidget::clicked, this, [=](int homeworkId) {emit clicked(homeworkId);});
}
void teacherHomeworkWidget::resizeEvent(QResizeEvent*) {
    bgWidget->resize(this->size());
}

teacherQuizAddPage::teacherQuizAddPage(int radius, int type, int width, int height, QString name, QWidget *parent,
                                       int posy) : SlidePage(radius, type, width, height, name, parent, posy) {
    title = new textInputItem("标题：", this);
    description = new textInputItem("内容：", this);
    place = new textInputItem("地点：", this);
    time = new textInputItem("时间：", this);
    activityBar = new QWidget(this);
    QHBoxLayout* activityLayout = new QHBoxLayout(activityBar);
    activityLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    activityLayout->setSpacing(10);
    activityLayout->setContentsMargins(0, 0, 0, 0);

    bigIconButton* alarmOn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", "", 0, 0, activityBar);
    alarmOn->setFixedSize(30, 30);
    activityLayout->addWidget(alarmOn);
    bigIconButton* alarmOff = new bigIconButton(1, ":/icons/icons/alarm_off.svg", "", "", 0, 0, activityBar);
    alarmOff->setFixedSize(30, 30);
    frequency = new textInputItem("频率：", activityBar);
    activityLayout->addWidget(frequency);

    bigIconButton* TypeBtn = new bigIconButton(1, ":/icons/icons/personal-activity.svg", "", "", 0, 0, activityBar);
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

QVector<QString> teacherQuizAddPage::collectMsg() {
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

