//
// Created by Go2Heart on 2022/3/24.
//

#include "guidepage.h"

GuidePage::GuidePage(QWidget *parent) :
    QWidget(parent)
{
    //this->setFixedSize(800,480);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setStyleSheet("background-color:transparent;");

    //QLabel *label = new QLabel(this);
    //label->setGeometry(0,0,800,480);
    //label->setStyleSheet("background-image:url(:/images/guidePage.png);");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    displayWidget = new QWidget(this);
    displayLayout = new QVBoxLayout(displayWidget);

    QWidget* spacing = new QWidget(displayWidget);
    spacing->setFixedHeight(50);
    spacing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    spacing->setStyleSheet("background-color:transparent; border:transparent;");
    displayLayout->addWidget(spacing);

    QString inputPath = ":/map/test.map";
    MyCanvas *newCanvas = loadCanvas(inputPath);
    if(newCanvas != nullptr) {
        displayLayout->addWidget(newCanvas);
        newCanvas->show();
        SlidePage *classInfo = new SlidePage(10, SlidePage::FIXED, 250, 500, "Modeinfo", this, 10);
        QWidget *whiteSpace = new QWidget(classInfo);
        whiteSpace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        whiteSpace->setStyleSheet("background-color:transparent;");
        whiteSpace->setFixedHeight(10);
        QWidget *modeChange = new QWidget(classInfo);
        modeChange->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QHBoxLayout *modeChangeLayout = new QHBoxLayout(modeChange);
        modeChange->setStyleSheet("border:2px solid transparent;");
        //modeChangeLayout->setContentsMargins(0, 0, 0, 0);
        //modeChangeLayout->setSpacing(0);
        //modeChangeLayout->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        modeChangeLayout->setSpacing(10);
        modeChangeLayout->setContentsMargins(10, 0, 0, 0);
        pageList.push_back(classInfo);

        textButton *classMode = new textButton("Class Mode", classInfo);
        textButton *timeMode = new textButton("Time Mode", classInfo);
        classMode->setEnabled(false);
        QWidget *whiteSpace2 = new QWidget(classInfo);
        whiteSpace2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        whiteSpace2->setStyleSheet("background-color:transparent;");
        whiteSpace2->setFixedHeight(10);

        modeChangeLayout->addWidget(classMode);
        modeChangeLayout->addWidget(timeMode);
        modeChange->setFixedHeight(classMode->height());
        whiteSpace->hide();

        textInputItem *className = new textInputItem("Class", classInfo);
        textButton *classSearch = new textButton("Search", classInfo);
        /*
        QVector<QWidget*> *classModeWidgets = new QVector<QWidget*>();
        classModeWidgets->push_back(classSearch);
        //classModeWidgets->push_back(whiteSpace);
        classModeWidgets->push_back(className);
       // classModeWidgets->push_back(whiteSpace2);
        classModeWidgets->push_back(modeChange);*/
        classInfo->AddContent(classSearch);
        classInfo->AddSpacing(10);
        classInfo->AddContent(className);
        classInfo->AddSpacing(10);
        classInfo->AddContent(modeChange);
        /*QVector<QWidget*> *timeModeWidgets = new QVector<QWidget*>();
        textButton *timeSearch = new textButton("Search", classInfo);
        textInputItem *timeStart = new textInputItem("Start", classInfo);
        timeModeWidgets->push_back(timeSearch);
        //timeModeWidgets->push_back(whiteSpace);
        timeModeWidgets->push_back(timeStart);
        //timeModeWidgets->push_back(whiteSpace2);
        timeModeWidgets->push_back(modeChange);
        timeSearch->hide();
        timeStart->hide();*/
        SlidePage *timeInfo = new SlidePage(10, SlidePage::FIXED, 250, 500, "Timeinfo", this, 10);
        textButton *timeSearch = new textButton("Search", timeInfo);
        textInputItem *timeStart = new textInputItem("Start", timeInfo);
        QWidget *modeChange2 = new QWidget(timeInfo);
        modeChange2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QHBoxLayout *modeChangeLayout2 = new QHBoxLayout(modeChange2);
        modeChange2->setStyleSheet("border:2px solid transparent;");
        //modeChangeLayout->setContentsMargins(0, 0, 0, 0);
        //modeChangeLayout->setSpacing(0);
        //modeChangeLayout->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        modeChangeLayout2->setSpacing(10);
        modeChangeLayout2->setContentsMargins(10, 0, 0, 0);
        //timeInfo->hide();
        //classInfo->hide();
        textButton *classMode2 = new textButton("Class Mode", timeInfo);
        textButton *timeMode2 = new textButton("Time Mode", timeInfo);
        timeMode2->setEnabled(false);
        modeChangeLayout2->addWidget(classMode2);
        modeChangeLayout2->addWidget(timeMode2);
        modeChange2->setFixedHeight(classMode2->height());
        timeInfo->AddContent(timeSearch);
        timeInfo->AddSpacing(10);
        timeInfo->AddContent(timeStart);
        timeInfo->AddSpacing(10);
        timeInfo->AddContent(modeChange2);
        pageList.push_back(timeInfo);

        connect(timeMode, &textButton::clicked, this, [=]() {
            //timeInfo->show();
            classInfo->slideOut();
            timeInfo->slideIn();
        });

        connect(classMode2, &textButton::clicked, this, [=]() {
            //classInfo->show();
            timeInfo->slideOut();
            classInfo->slideIn();
        });

        /*
        classInfo->AddContent(classSearch);
        classInfo->AddContent(whiteSpace);
        classInfo->AddContent(className);
        classInfo->AddContent(whiteSpace2);
        classInfo->AddContent(modeChange);*/
        connect(newCanvas, &MyCanvas::modeBtnClicked, classInfo, [=]() {
            emit addPage(classInfo);
            emit addPage(timeInfo);
            classInfo->slideIn();
        });
        SlidePage *passInfo = new SlidePage(10,  SlidePage::FIXED, 250, 500, "Passinfo", this, 10);
        QWidget *passBar = new QWidget(passInfo);
        passBar->setStyleSheet("background-color:transparent;");
        passBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QHBoxLayout *passBarLayout = new QHBoxLayout(passBar);
        passBarLayout->setContentsMargins(0, 0, 0, 0);
        passBarLayout->setSpacing(3);
        passBarLayout->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

        ComboBox *passList = new ComboBox(passBar);
        passList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        passList->setFixedSize(45,25);
        passList->setNumberRange(0,24);
        passBarLayout->addWidget(passList);
        QLabel *split = new QLabel(":", passBar);
        split->setFixedHeight(25);
        split->setStyleSheet("color:black;");
        passBarLayout->addWidget(split);
        passBar->setFixedHeight(30);
        customWidget *passWidget = new customWidget("Pass:", passBar, this);
        passInfo->AddContent(passWidget);
        pageList.push_back(passInfo);

        connect(newCanvas, &MyCanvas::passBtnClicked, passInfo, [=](){
            emit addPage(passInfo);
            passInfo->slideIn();
        });

        connect(classSearch, &textButton::clicked, this, [=]() {
            //qDebug()<<"classSearch";
            classInfo->slideOut();
        });
        connect(timeSearch, &textButton::clicked, this, [=]() {
            //qDebug()<<"timeSearch";
            timeInfo->slideOut();
        });
    }
}
MyCanvas* GuidePage::loadCanvas(const QString &path){
    QFile input(path);
    input.open(QIODevice::ReadOnly);
    QTextStream ts(&input);
    MyCanvas *newCanvas = new MyCanvas(ts, cornerRadius, displayWidget);
    input.close();
    return newCanvas;
}

void GuidePage::resizeEvent(QResizeEvent*) {
    displayWidget->resize(this->size());
    for(int i = 0; i < pageList.size(); i++) {
        pageList[i]->resize(pageList[i]->width() - 1, pageList[i]->Type() == SlidePage::EXPANDING ? height() : pageList[i]->height());
        pageList[i]->resize(pageList[i]->width() + 1, pageList[i]->height());
    }
}
