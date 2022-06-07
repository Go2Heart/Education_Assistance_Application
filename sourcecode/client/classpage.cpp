//
// Created by Go2Heart on 2022/4/27.
//

#include <QFileDialog>
#include "classpage.h"
#include "loginpage.h"

classInfoWidget::classInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent),
        classType(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/personal-activity.svg"/*改成单人*/ : ":/icons/icons/group-activity.svg"/*改成集体*/, "", 0, this))
{
    this->info = info;
    id = info[4];
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
    classType->setFixedSize(30, 30);
}

void classInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void classInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked();
}


void classInfoWidget::resizeEvent(QResizeEvent *event) {
    infoWidget->resize(this->width() - classType->width() - 2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    classType->move(this->width() - classType->width() - margin, this->height() / 2 - classType->height() / 2);
}




classListWidget::classListWidget(QString name, int h, QWidget* p, QWidget* parent) :
        QWidget(parent),
        maxHeight(h),
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
    for(int i = 0; i < extraIcons.size(); i++) {
        extraIcons[i]->setParent(titleWidget);
        extraIcons[i]->setFixedSize(titleHeight - 10, titleHeight - 10);
        connect(extraIcons[i], &bigIconButton::clicked, this, [=] {emit clicked(i);});
    }
    container = new ScrollAreaCustom(false, this);


}

void classListWidget::resizeEvent(QResizeEvent*){
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

classWidget::classWidget(QVector<QString> info, QWidget* parent) :
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
    infoWidget = new classInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &classInfoWidget::clicked, this, [=] {emit clicked();});
}

void classWidget::resizeEvent(QResizeEvent*) {
    bgWidget->resize(this->size());
}

ClassPage::ClassPage(QWidget* parent):
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
    textInputItem* classSearch = new textInputItem("课程", searchBar);
    bigIconButton* searchclass = new bigIconButton(1, ":/icons/icons/search.svg", "", 6, searchBar);
    searchclass->setFixedSize(30,30);

    /*TODO connect */

    searchLayout->addWidget(classSearch);
    searchLayout->addWidget(selections);
    searchLayout->addWidget(searchclass);
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
    textButton* detailTabButton1 = new textButton("活动详情", detailTab);
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

    activityDtl = new classDetailWidget(detailArea);
    areaLayout->addWidget(activityDtl);
//        activityDtl->hide();

    QWidget* materialDlvr = new QWidget(detailArea);
    materialDlvr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    materialDlvr->setStyleSheet("Height:400; Width:400");
    QVBoxLayout* MDlvrLayout = new QVBoxLayout(materialDlvr);
    MDlvrLayout->setAlignment(Qt::AlignCenter);

    fileDlvr = new classFileDeliver(materialDlvr);
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
    classListWidget* classList = new classListWidget("class", 500, itemWidget, eventWidget);


    eventLayout->addWidget(classList);
    QVector<QWidget*> items;
//                    connect(iconVec[0], &bigIconButton::clicked, this, [=]{
//                        classAddPage* newPage = new classAddPage(12,1,300,0,"Create an class", slideParent);
//                        emit addPage(newPage);
//                        connect(newPage, &classAddPage::deliver, this, [=](QVector<Qstring> s){
//                            class
//                        })
//                        classInfo = new classInfoWidget(info, this);
//                        eventLayout->addWidget(classInfo);

//                    })



    itemList->addWidgets(items);
    itemLayout->addWidget(eventWidget);
    itemLayout->addWidget(detailWidget);
    //活动详情信息

    mainLayout->addWidget(itemWidget);
    ClassQuery* query = new ClassQuery(studentId);
    connect(query, &ClassQuery::receive, this, [=](QVariant varValue){
        QVector<ClassResult*> classResult = varValue.value<QVector<ClassResult*>>();
        for(int i = 0; i < classResult.size(); i++){
            QVector<QString> info;
            info.push_back(classResult[i]->name);
            info.push_back(classResult[i]->teacher);
            info.push_back(classResult[i]->place);
            info.push_back(classResult[i]->time);
            info.push_back(classResult[i]->id);
            //info.push_back

            classWidget * newClass = new classWidget(info, itemWidget);
            connect(newClass, &classWidget::clicked, this, [=]{
                activityDtl->showDetail(info);
                activityDtl->setActivity(newClass);
                fileDlvr->setActivity(newClass);
            //TODO adding specific class page
            });
            classList->addContent(newClass);
        }
    });
}
void ClassPage::resizeEvent(QResizeEvent*) {
    itemWidget->resize(this->size());
    for(int i = 0; i < pageList.size(); i++) {
        pageList[i]->resize(pageList[i]->width() - 1, pageList[i]->Type() == SlidePage::EXPANDING ? itemWidget->height() : pageList[i]->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}


classFileDeliver::classFileDeliver(QWidget *parent):QWidget(parent){
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
            fileUploader = new FileUpload(id,fileNames[i], filesToSubmit[i], 1);
        }
        fileNames.clear();
        fileList->clear();
        filesToSubmit.clear();

    });
    mainLayout->addWidget(select);
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(upload);

}

classDetailWidget::classDetailWidget(QWidget *parent) : QWidget(parent){
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
    mainLayout->addWidget(title);
    mainLayout->addWidget(description);
    mainLayout->addWidget(place);
    mainLayout->addWidget(time);
}
QVector<QString> classDetailWidget::collectMsg() {
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

void classDetailWidget::showDetail(QVector<QString> info) {
    title->setValue(info[0]);
    description->setValue(info[1]);
    place->setValue(info[2]);
    time->setValue(info[3]);
    //isPersonal = info[4].toInt();
    //alarm = info[5].toInt();
    //frequency->setValue(info[6]);

}