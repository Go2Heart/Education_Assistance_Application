//
// Created by Go2Heart on 2022/4/27.
//

#include "classpage.h"
#include "loginpage.h"

classInfoWidget::classInfoWidget(QVector<QString> info, QWidget* parent) :
        QWidget(parent),
        classType(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/personal-class.svg"/*改成单人*/ : ":/icons/icons/group-class.svg"/*改成集体*/, "", 0, this))
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

void classInfoWidget::modify(QVector<QString> info) {
    descLabel->setText("#内容#" + info[0]);
    detailLabel->setText("#地点#" + info[1] + "       #时间#" + info[2]);
    classType->setPixmap(info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg");
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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(maxHeight - titleHeight + overlap);
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
    textInputItem* classSearch = new textInputItem("课程", searchBar);
    bigIconButton* searchclass = new bigIconButton(1, ":/icons/icons/search.svg", "", 6, searchBar);
    searchclass->setFixedSize(30,30);

    /*TODO connect */

    searchLayout->addWidget(classSearch);
    searchLayout->addWidget(searchclass);
    eventLayout->addWidget(searchBar);

    //itemList = new ScrollAreaCustom(false, eventWidget);
    //itemList->setFixedWidth(400);
    //eventLayout->addWidget(itemList);

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
    //活动详情信息
    QWidget* itemInfoTable = new QWidget(itemWidget);
    itemInfoTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    itemInfoTable->setStyleSheet("border:1px solid gray;background-color:dark blue");
    itemLayout->addWidget(itemInfoTable);
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
            //info.push_back

            classWidget * newClass = new classWidget(info, itemWidget);
            connect(newClass, &classWidget::clicked, this, [=]{
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


