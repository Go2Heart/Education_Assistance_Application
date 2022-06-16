//
// Created by Go2Heart on 2022/3/24.
//
#include "mycanvas.h"

LogWidget::LogWidget(QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    QLabel *logLabel = new QLabel(this);
    logLabel->setText("LOG");
    logLabel->setFont(QFont("Corbel", 20));
    logLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    logLabel->setStyleSheet("color:#2c2c2c");
    QWidget *splitter = new QWidget(this);
    splitter->setFixedSize(30, 6);
    splitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    container = new ScrollAreaCustom(false, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(logLabel);
    layout->addWidget(splitter);
    layout->addWidget(container);
    setFixedWidth(250);
}

void LogWidget::clear() {
    container->clear();
}

MyCanvas::MyCanvas(QTextStream &ts, int radius, QWidget *parent) :
        QWidget(parent)
{
    /* create canvas */
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(20, 0, 20, 20);
    this->setLayout(mainLayout);
    QWidget* viewWidget = new QWidget(this);
    viewWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* viewLayout = new QVBoxLayout(viewWidget);
    viewLayout->setContentsMargins(0, 0, 0, 0);
        QLabel *viewLabel = new QLabel(this);
        viewLabel->setText("Graph");
        viewLabel->setFont(QFont("Corbel", 20));
        viewLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        viewLabel->setStyleSheet("color:#2c2c2c");
        QWidget *splitter = new QWidget(this);
        splitter->setFixedSize(30, 6);
        splitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
        view = new MyGraphicsView(MyGraphicsView::UDG);
        view->setSceneRect(view->rect());
        view->setStyleSheet("background-color: #FFFFFF;border:1px solid #cfcfcf;border-radius:10px;");

        viewLayout->addWidget(viewLabel);
        viewLayout->addWidget(splitter);
        viewLayout->addWidget(view);

    mainLayout->addWidget(viewWidget);
    g = new ALGraph(1);
    connect(view, SIGNAL(vexAdded(MyGraphicsVexItem*)), this, SLOT(addVex(MyGraphicsVexItem*)));
    connect(view, SIGNAL(arcAdded(MyGraphicsLineItem*)), this, SLOT(addArc(MyGraphicsLineItem*)));
    connect(view, &MyGraphicsView::visitClear, this, [=](){g->ClearVisit();});
    view->ReadFromFile(ts);
    for(int i = 0; i < view->arcNum; i++){
        qDebug()<< view->lines[i]->stVex()->id << view->lines[i]->edVex()->id;
        int w;
        ts >> w;
        if(w != 0)
            g->SetWeight(view->lines[i], w);
    }
    this->setFocusPolicy(Qt::ClickFocus);
    QTimer *delay = new QTimer(this);
    connect(delay, &QTimer::timeout, this, [=](){Init();});
    delay->setSingleShot(true);
    delay->start(10);
}

void MyCanvas::Init(){
    /* Create info widget */

    infoWidget = new QWidget(this);
    logWidget = new LogWidget(this);
    mainLayout->addWidget(logWidget);
    mainLayout->addWidget(infoWidget);
    mainLayout->setStretch(0, 6);
    mainLayout->setStretch(1, 2);
    mainLayout->setStretch(2, 2);
    infoWidget->setMinimumWidth(250);
    infoWidget->setMaximumWidth(500);

    //Set basic layout
    QVBoxLayout *infoLayout = new QVBoxLayout(infoWidget);
    infoWidget->setLayout(infoLayout);
    infoLayout->setContentsMargins(10, 0, 0, 0);
    infoLayout->setAlignment(Qt::AlignTop);

    QFont titleFont = QFont("Corbel", 20);

    QWidget *upper = new QWidget(infoWidget);
    QVBoxLayout *upperLayout = new QVBoxLayout(upper);
    upper->setLayout(upperLayout);
    upperLayout->setContentsMargins(0, 0, 0, 0);
    upper->setContentsMargins(0, 0, 0, 0);
    pageName = new QLabel(infoWidget);
    pageName->setText("校园导航");
    pageName->setFont(titleFont);
    pageName->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    pageName->setStyleSheet("color:#2c2c2c");
    QWidget *upperSplitter = new QWidget(upper);
    upperSplitter->setFixedSize(30, 6);
    upperSplitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    upperLayout->addWidget(pageName);
    upperLayout->addWidget(upperSplitter);

    /*QWidget *lower = new QWidget(infoWidget);
    QVBoxLayout *lowerLayout = new QVBoxLayout(lower);
    lower->setLayout(lowerLayout);
    lowerLayout->setContentsMargins(0, 5, 0, 0);
    lower->setObjectName("lower");
    lower->setStyleSheet("QWidget#lower{border:1px solid #cfcfcf;border-radius:5px;}");
     */
    /*
    QWidget *lowerSplitter = new QWidget(lower);
    lowerSplitter->setFixedSize(30, 6);
    lowerSplitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    lowerLayout->addWidget(lowerSplitter);
    */
    //QWidget *lowerSplitter1 = new QWidget(lower);
    //QHBoxLayout *lowerLayout1 = new QHBoxLayout(lowerSplitter1);
    //lowerSplitter1->setLayout(lowerLayout1);
    //lowerSplitter1->setObjectName("lowerSplitter1");
    //lowerSplitter1->setStyleSheet("QWidget#lowerSplitter1{border:2px solid transparent;border-radius:5px;}");
    //lowerLayout1->setContentsMargins(0, 0, 0, 0);
    QWidget *modeWidget = new QWidget(infoWidget);
    QHBoxLayout *modeLayout = new QHBoxLayout(modeWidget);
    modeLayout->setContentsMargins(0, 0, 0, 10);
    modeWidget->setObjectName("modeWidget");
    modeWidget->setStyleSheet("QWidget#modeWidget{border:2px solid transparent;border-radius:5px;}");
    modeWidget->setLayout(modeLayout);

        modeBox = new ComboBox(upper);
        modeBox->setFixedSize(120, 25);
        modeBox->addItem("最短距离");
        modeBox->addItem("最短时间");
        modeBox->addItem("交通工具的最短时间");
        modeBox->setCurrentIndex(0);
        connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [=](int index){ LoadInfo(); });
    customWidget* chooseMode = new customWidget("Mode", modeBox);
    modeLayout->addWidget(chooseMode);

    bigIconButton *modeBtn = new bigIconButton(1, ":/icons/icons/add.svg", "", "", 0, 15, upper);
    modeBtn->setFixedSize(30, 30);
    modeLayout->addWidget(modeBtn);
    connect(modeBtn, &bigIconButton::clicked, this, [=] {
        emit modeBtnClicked();
    });
    //connect(passChooseIcon, &bigIconButton::clicked, this, [=] {
    //    emit passBtnClicked();
    //    //modeInfo->slideIn();
    //});
    //passChooseIcon->setObjectName("PassChooseIcon");

    //passChooseIcon->setStyleSheet("border:1px solid trans;background-color:transparent;");
    // passChooseIcon->setFixedSize(30, 30);
    // lowerLayout2->addWidget(passChooseIcon);
    // lowerLayout->addWidget(lowerSplitter2);

    QWidget *path = new QWidget;
    QVBoxLayout *pathLayout = new QVBoxLayout(path);
    path->setLayout(pathLayout);
    pathLayout->setContentsMargins(0, 5, 0, 0);
    path->setObjectName("path");
    path->setStyleSheet("QWidget#path{border:1px solid #cfcfcf;border-radius:5px;}");
    QLabel *pathLabel = new QLabel(path);
    pathLabel->setObjectName("pathLabel");
    pathLabel->setStyleSheet("QLabel#pathLabel{border:2px solid transparent #cfcfcf;border-radius:5px;}");
    pathLabel->setText("路径");
    pathLabel->setObjectName("pathLabel");
    pathLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    ScrollAreaCustom *pathArea = new ScrollAreaCustom(false, path, false);
    pathArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathArea);


    /*Start Guide Button*/
    QWidget *lower3 = new QWidget(infoWidget);
    QHBoxLayout *lowerLayout3 = new QHBoxLayout(lower3);
    //lower3->resize(30, 40);
    lowerLayout3->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    lowerLayout3->setContentsMargins(11, 10, 0, 11);
    bigIconButton *goIcon = new bigIconButton(1, ":/icons/icons/find-path.svg", "", "", 0, 25, lower3);
    lowerLayout3->addWidget(goIcon);
    goIcon->setFixedSize(50, 50);

    /*QWidget *lower = new QWidget(infoWidget);
    QVBoxLayout *lowerLayout = new QVBoxLayout(lower);
    lower->setLayout(lowerLayout);
    lowerLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *logLabel = new QLabel(lower);
    logLabel->setText("LOG");
    logLabel->setFont(titleFont);
    logLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    logLabel->setStyleSheet("color:#2c2c2c");
    QWidget *lowerSplitter = new QWidget(lower);
    lowerSplitter->setFixedSize(30, 6);
    lowerSplitter->setStyleSheet("background-color:#3c3c3c;border-radius:3px;");
    ScrollAreaCustom *logDisplay = new ScrollAreaCustom(lower);
    logDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lowerLayout->addWidget(logLabel);
    lowerLayout->addWidget(lowerSplitter);
    lowerLayout->addWidget(logDisplay);


    infoLayout->addWidget(lower);
    //log
    */
    infoLayout->addWidget(upper);
    //infoLayout->addWidget(lower);
    infoLayout->addWidget(path);
    infoLayout->addWidget(lower3);
    //lower->hide();
    //Add specific items and connections
    //Default page
    QWidget *defInfoPage = new QWidget(infoWidget);
    QVBoxLayout *defInfoLayout = new QVBoxLayout(defInfoPage);
    defInfoLayout->setContentsMargins(0, 0, 0, 0);
    defInfoLayout->setAlignment(Qt::AlignTop);
    QWidget *defTextItems = new QWidget(defInfoPage);
    defTextItems->setObjectName("DefTextItems");
    defTextItems->setStyleSheet("QWidget#DefTextItems{border:1px solid #cfcfcf;border-radius:5px;}");
    QVBoxLayout *defTextLayout = new QVBoxLayout(defTextItems);
    defTextLayout->setContentsMargins(0, 5, 0, 5);
    textInputItem *textName = new textInputItem("Name", defInfoPage);
    textName->setValue("校区示意图");
    //connect(this, &MyCanvas::nameChanged, this, [=](){textName->setValue(canvasName);});
    textName->setEnabled(false);
    defTextLayout->addWidget(textName);
    textInputItem *textDesc = new textInputItem("Detail", defInfoPage);
    textDesc->setValue("请点击图示位置设置导航起点与终点");
    //connect(this, &MyCanvas::descChanged, this, [=](){textDesc->setValue(canvasDescription);});
    textDesc->setEnabled(false);
    defTextLayout->addWidget(textDesc);
    //guideWidget->resize(guideWidget->width()-2, guideWidget->height()-2);

    //QHBoxLayout *guideModeLayout = new QHBoxLayout();
    //guideWidget->setLayout(guideModeLayout);
    //guideModeLayout->setAlignment(Qt::AlignRight);

    //defTextLayout->addWidget();
    /*
     * customIcon* guideModeIcon = new customIcon(":/icons/icons/jiahao.svg", "guide", 0,guideWidget);
    guideModeIcon->setFixedSize(10, 10);
    guideModeLayout->addWidget(guideModeIcon);
     * bigIconButton *guideModeIcon = new bigIconButton(2, "", "＋", 0, guideWidget);
    guideModeIcon->setFixedSize(15, 15);
    guideModeLayout->addWidget(guideModeIcon);
    */
    //defTextLayout->addWidget(guideWidget);
    //defTextLayout->addWidget(guideMode);


    textInputItem *startPos = new textInputItem("Start", defInfoPage);
    connect(this, &MyCanvas::startChanged, this, [=](){startPos->setValue(start->Text());});
    startPos->setEnabled(false);
    defTextLayout->addWidget(startPos);
    /*
     * QWidget *choosePass = new QWidget(defInfoPage);
    QHBoxLayout *choosePassLayout = new QHBoxLayout(choosePass);
    choosePass->setLayout(choosePassLayout);
    textInputItem *passName = new textInputItem("Pass", defInfoPage);
    passName->setValue("");
    choosePassLayout->addWidget(passName);
    bigIconButton *passChooseIcon = new bigIconButton(1, ":/icons/icons/jiahao.svg", "", 12, choosePass);
    choosePassLayout->addWidget(passChooseIcon);
    defTextLayout->addWidget(choosePass);
    */
    //for horizontal layout adding
    textInputItem *endPos = new textInputItem("End", defInfoPage);
    connect(this, &MyCanvas::endChanged, this, [=](){endPos->setValue(end->Text());});
    endPos->setEnabled(false);
    defTextLayout->addWidget(endPos);
    defTextLayout->addWidget(modeWidget);
    /*
    textInputItem *vexNumText = new textInputItem("Vex", defInfoPage);
    vexNumText->setValue(QString::asprintf("%d", view->vexNum));
    vexNumText->setEnabled(false);
    defTextLayout->addWidget(vexNumText);
    textInputItem *arcNumText = new textInputItem("Arc", defInfoPage);
    arcNumText->setValue(QString::asprintf("%d", view->arcNum));
    arcNumText->setEnabled(false);
    defTextLayout->addWidget(arcNumText);
    */
    defInfoLayout->addWidget(defTextItems);
    upperLayout->addWidget(defInfoPage);
    defInfoPage->show();

    //VexPage
    QWidget *vexInfoPage = new QWidget(infoWidget);
    QVBoxLayout *vexInfoLayout = new QVBoxLayout(vexInfoPage);
    vexInfoLayout->setContentsMargins(0, 0, 0, 0);
    vexInfoLayout->setAlignment(Qt::AlignTop);
    vexInfoPage->setLayout(vexInfoLayout);
    QWidget *vexTextItems = new QWidget(vexInfoPage);
    vexTextItems->setObjectName("VexTextItems");
    vexTextItems->setStyleSheet("QWidget#VexTextItems{border:1px solid #cfcfcf;border-radius:5px;}");
    QVBoxLayout *vexTextLayout = new QVBoxLayout(vexTextItems);
    vexTextItems->setLayout(vexTextLayout);
    vexTextLayout->setContentsMargins(0, 5, 0, 5);
    textInputItem *textTag = new textInputItem("Tag", vexInfoPage, 1);
    vexTextLayout->addWidget(textTag);
    vexInfoLayout->addWidget(vexTextItems);

    QWidget *traverseBar = new QWidget(vexInfoPage);
    QHBoxLayout *traverseLayout = new QHBoxLayout(traverseBar);
    traverseBar->setLayout(traverseLayout);
    traverseLayout->setContentsMargins(0, 0, 0, 0);
    textButton *setStart =new textButton("Set Start", vexInfoPage);
    traverseLayout->addWidget(setStart);
    textButton *setEnd =new textButton("Set End", vexInfoPage);
    traverseLayout->addWidget(setEnd);
    vexInfoLayout->addWidget(traverseBar);
    textButton *add2Path =new textButton("Add to Path", vexInfoPage);
    vexInfoLayout->addWidget(add2Path);
    upperLayout->addWidget(vexInfoPage);
    vexInfoPage->hide();

    //ArcPage
    QWidget *arcInfoPage = new QWidget(infoWidget);
    QVBoxLayout *arcInfoLayout = new QVBoxLayout(arcInfoPage);
    arcInfoLayout->setContentsMargins(0, 0, 0, 0);
    arcInfoLayout->setAlignment(Qt::AlignTop);
    QWidget *arcTextItems = new QWidget(arcInfoPage);
    arcTextItems->setObjectName("VexTextItems");
    arcTextItems->setStyleSheet("QWidget#VexTextItems{border:1px solid #cfcfcf;border-radius:5px;}");
    QVBoxLayout *arcTextLayout = new QVBoxLayout(arcTextItems);
    arcTextLayout->setContentsMargins(0, 5, 0, 5);
    textInputItem *arcWeight = new textInputItem("路径耗时", arcInfoPage, 1);
    arcTextLayout->addWidget(arcWeight);
    /*info about chaos
    textInputItem *chaos = new textInputItem("Chaos", arcInfoPage, 1);
    arcTextLayout->addWidget(chaos);
    chaos->setValue("getChaos()");
    QRegularExpression re("^[1-9]\\d*$");
    arcWeight->setValidator(new QRegularExpressionValidator(re));
    */
    textInputItem *arcStart = new textInputItem("Start", arcInfoPage, 1);
    arcStart->setValue("NA");
    arcStart->setEnabled(false);
    arcTextLayout->addWidget(arcStart);
    textInputItem *arcEnd = new textInputItem("End", arcInfoPage, 1);
    arcEnd->setValue("NA");
    arcEnd->setEnabled(false);
    arcTextLayout->addWidget(arcEnd);
    arcInfoLayout->addWidget(arcTextItems);
    /*
        textButton *reverseBtn = new textButton("Reverse", arcInfoPage);
        arcInfoLayout->addWidget(reverseBtn);
        textButton *delArc = new textButton("Delete", "#1acb1b45","#2acb1b45","#3acb1b45", arcInfoPage);
        arcInfoLayout->addWidget(delArc);
    */
    upperLayout->addWidget(arcInfoPage);
    arcInfoPage->hide();

    connect(view, &MyGraphicsView::selected, this, [=](QGraphicsItem *item){
        int type = item->type();
        if(type == MyGraphicsVexItem::Type){
            defInfoPage->hide();
            arcInfoPage->hide();
            path->hide();
            goIcon->hide();
            vexInfoPage->show();
            textTag->setValue(view->selectedVex()->Text());
        }
        else if(type == MyGraphicsLineItem::Type){
            defInfoPage->hide();
            vexInfoPage->hide();
            path->hide();
            goIcon->hide();
            arcInfoPage->show();

            arcWeight->setValue(view->selectedArc()->weightText() == "" ? "1" : view->selectedArc()->weightText());
            arcStart->setValue(view->selectedArc()->stVex()->Text());
            arcEnd->setValue(view->selectedArc()->edVex()->Text());
        }
        else{
            vexInfoPage->hide();
            arcInfoPage->hide();
            defInfoPage->show();
            //vexNumText->setValue(QString::asprintf("%d",view->vexNum));
            //arcNumText->setValue(QString::asprintf("%d",view->arcNum));
        }
    });

    connect(view, &MyGraphicsView::unselected, this, [=](){
        vexInfoPage->hide();
        arcInfoPage->hide();
        defInfoPage->show();
        path->show();
        goIcon->show();
        //vexNumText->setValue(QString::asprintf("%d",view->vexNum));
        //arcNumText->setValue(QString::asprintf("%d",view->arcNum));
    });

    /**
     * button function
     * connections
     */
    connect(setStart, &textButton::clicked, this, [=]() {
        if (view->selectedVex() != nullptr) {
            //qDebug() << view->selectedVex()->Text() << view->selectedVex()->getData() << Qt::endl;
            //startName = view->selectedVex()->Text();
            //startId = view->selectedVex()->
            start = view->selectedVex();
            emit startChanged();
            //logDisplay->addWidget(new viewLog("[Vex] | Set \""+view->selectedVex()->Text()+"\" as start"),0);
        }
    });
    connect(setEnd, &textButton::clicked, this, [=]() {
        if (view->selectedVex() != nullptr) {
            //qDebug() << view->selectedVex()->Text() << view->selectedVex()->getData() << Qt::endl;
            end = view->selectedVex();
            emit endChanged();
            //logDisplay->addWidget(new viewLog("[Vex] | Set \""+view->selectedVex()->Text()+"\" as end"),0);
        }
    });
    connect(add2Path, &textButton::clicked, this, [=]() {
        if (view->selectedVex() != nullptr) {
            //qDebug() << view->selectedVex()->Text() << view->selectedVex()->getData() << Qt::endl;
            pathVector.push_back(view->selectedVex());
            pathArea->addWidget(new viewLog("Pass "+QString::number(pathVector.length())+" \""+view->selectedVex()->Text()+"\""),0);
            //logDisplay->addWidget(new viewLog("[Vex] | Add \""+view->selectedVex()->Text()+"\" to path"),0);
        }
    });
    connect(goIcon, &bigIconButton::clicked, this, [=] {
        if(start != nullptr && end != nullptr) {
            logWidget->clear();
            QVector<int> v;
            v.push_back(start->id);
            for(int i = 0; i < pathVector.size(); i++) v.push_back(pathVector[i]->id);
            v.push_back(end->id);
            DisQuery* query = new DisQuery(v, modeBox->currentIndex() + 1);
            connect(query, &DisQuery::receive, this, [=](QVariant varValue) {
                ResPackage result = varValue.value<ResPackage>();
                logWidget->AddContent(new viewLog("[搜索结果]    已找到路径。路径搜索模式为 " + modeBox->currentText() + " 。路径总耗时为 " + QString::asprintf("%d 分, %d 秒", result.timeCost.hour + result.timeCost.day * 24 + result.timeCost.week * 168, result.timeCost.minute), 3));
                for(int i = 0, j = 0; i < result.v.size(); i = j + 1, j = i) {
                    //view->hasVisitedItem = true;
                    QString normalTool[2] = {"步行", "骑自行车"};
                    QString specifiedTool[2] = {"乘公车", "乘班车"};
                    int nowMode = modeBox->currentIndex();
                    if(result.v[i].type == 3) {
                        while(result.v[j].type != 4) ++j;
                        int beginVexId;
                        if(i != 0) {
                            logWidget->AddContent(new viewLog("[路径]    从途经点 " + view->vexFromId(result.v[i + 1].id)->Text() + " 出发", 2));
                        } else {
                            logWidget->AddContent(new viewLog("[路径]    从起点 " + view->vexFromId(result.v[i + 1].id)->Text() + " 出发", 2));
                        }
                        for(int k = i + 1; k < j; k++) {
                            if(result.v[k].type == 5) continue;
                            if(result.v[k].type == 2) {
                                view->vexFromId(result.v[k].id)->visit();
                                beginVexId = result.v[k].id;
                            } else {
                                MyGraphicsLineItem* line = view->lineFromId(result.v[k].id);
                                if(line->stVex()->id != beginVexId) line->reverseDirection();
                                line->visit();
                                if(result.v[k].id == 0) {
                                    logWidget->AddContent(new viewLog("[路径]    " + (nowMode ? specifiedTool[result.v[k].tool] : QString("")) + "从点 " + line->stVex()->Text() + " 到 " + line->edVex()->Text() + " 。耗时" + QString::asprintf("%d 分, %d 秒", result.v[k + 1].tool / 60, result.v[k + 1].tool % 60), 3));
                                } else {
                                    logWidget->AddContent(new viewLog("[路径]    " + (nowMode ? normalTool[result.v[k].tool] : QString("")) + "从点 " + line->stVex()->Text() + " 到 " + line->edVex()->Text() + " 。耗时" + QString::asprintf("%d 分, %d 秒", line->weightText().toInt() / 60, line->weightText().toInt() % 60), 3));
                                }
                            }
                        }
                        if(j == result.v.size() - 1) {
                            logWidget->AddContent(new viewLog("[路径]    已到达终点" + view->vexFromId(result.v[j - 1].id)->Text(), 2));
                        } else {
                            logWidget->AddContent(new viewLog("[路径]    已到达途经点" + view->vexFromId(result.v[j - 1].id)->Text(), 2));
                        }
                    } else qDebug()<<"format error!";
                    //view->visitClear();
                }
            });
            pathVector.clear();
            pathArea->clear();
        }
    });
}

void MyCanvas::addVex(MyGraphicsVexItem *vex) {
    g->AddVex(vex);
}

void MyCanvas::addArc(MyGraphicsLineItem *arc) {
    g->AddArc(arc);
}

void MyCanvas::LoadInfo() {
    MapQuery* nowQuery = new MapQuery(modeBox->currentIndex() + 1);
    connect(nowQuery, &MapQuery::receive, this, [=](QVariant varValue) {
        QVector<int> v = varValue.value<QVector<int>>();
        view->lineFromId(0)->setText("校区连接路径");
        for(int i = 1; i < v.size(); i++) {
            view->lineFromId(i)->setText(QString::asprintf("%d", v[i]));
        }
    });
}

void MyCanvas::changeEndVex(int x) {
    end = view->vexFromId(x);
    emit endChanged();
}
