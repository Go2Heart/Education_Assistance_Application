#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [=](){Init();});
    t->setSingleShot(true);
    t->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init() {
    unsetTimer = new QTimer(this);
    unsetTimer->setSingleShot(true);
    unsetTimer->start(100);
    connect(unsetTimer,
        &QTimer::timeout,
        this,
        [=]{
            if(ui->mainWidget->geometry().contains(ui->centralWidget->mapFromGlobal(QCursor::pos())) && !forceState)
                unsetCursor();
            unsetTimer->start(100);
    });

    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);

    QString mainStyle = "background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx", cornerRadius);
    ui->mainWidget->setStyleSheet(mainStyle);

    windowShadow = new QGraphicsDropShadowEffect(this);
    //设置阴影半径30
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0));//颜色黑色
    windowShadow->setOffset(0, 0);//偏移量为0
    ui->mainWidget->setGraphicsEffect(windowShadow);//使用阴影效果

    border = new QWidget(this);
    border->move(ui->mainWidget->pos() - QPoint(1, 1));
    border->resize(ui->mainWidget->size() + QSize(2, 2));
    QString borderStyle = "background-color:#00FFFFFF;border:1.5px solid #686868;" + QString::asprintf("border-radius:%dpx", cornerRadius);
    border->setStyleSheet(borderStyle);
    //边界设置对鼠标event不反应
    border->setAttribute(Qt::WA_TransparentForMouseEvents);
    border->show();

    miniBtn = new topButton(ui->mainWidget);
    miniBtn->move(ui->mainWidget->width() - verticalMargin - 2 * 12 - 2 * 6, horizontalMargin);
    adjBtn = new topButton(ui->mainWidget);
    adjBtn->move(ui->mainWidget->width() - verticalMargin - 1 * 12 - 1 * 6, horizontalMargin);
    closeBtn = new topButton(ui->mainWidget);
    closeBtn->move(ui->mainWidget->width() - verticalMargin, horizontalMargin);

    connect(miniBtn, &QPushButton::clicked, [=]{showMinimized();});
    connect(adjBtn, &QPushButton::clicked, [=]{controlWindowScale();});
    connect(closeBtn, &QPushButton::clicked, [=]{close();});

    //login page
    loginpage = new loginPage(ui->mainWidget);
    ui->mainLayout->addWidget(loginpage);
    loginpage->show();
    mainpage = new mainPage(ui->mainWidget);
    connect(loginpage,
        &loginPage::logined,
        [=] {
            loginpage->hide();
            ui->mainLayout->removeWidget(loginpage);
            ui->mainLayout->addWidget(mainpage);
            mainpage->show();
    });
}
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        mousePressed = true;
        forceState = true;
        lastPos = event->globalPos() - this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::NoButton)
        mousePressed = false;
    //qDebug()<<event->pos().x()<<" "<<event->pos().y();
    if(!mousePressed) {
        mouseState = 0;
        if(!maximized) {
            int ldis = ui->mainWidget->pos().x() - event->pos().x();
            int rdis = event->pos().x() - ui->mainWidget->pos().x() - ui->mainWidget->width();
            int tdis = ui->mainWidget->pos().y() - event->pos().y();
            int bdis = event->pos().y() - ui->mainWidget->pos().y() - ui->mainWidget->height();
            int mindis = 1, maxdis = 5;
            if(ldis > mindis && ldis < maxdis)
                mouseState |= AT_LEFT;
            if(rdis > mindis && rdis < maxdis)
                mouseState |= AT_RIGHT;
            if(tdis > mindis && tdis < maxdis)
                mouseState |= AT_TOP;
            if(bdis > mindis && bdis < maxdis)
                mouseState |= AT_BOTTOM;
        }
        if(mouseState == AT_TOP_LEFT || mouseState == AT_BOTTOM_RIGHT)
            setCursor(Qt::SizeFDiagCursor);
        else if(mouseState == AT_TOP_RIGHT || mouseState == AT_BOTTOM_LEFT)
            setCursor(Qt::SizeBDiagCursor);
        else if(mouseState & (AT_LEFT | AT_RIGHT))
            setCursor(Qt::SizeHorCursor);
        else if(mouseState & (AT_TOP | AT_BOTTOM))
            setCursor(Qt::SizeVerCursor);
        else unsetCursor();
    } else {
        if(mouseState == 0){
            if(maximized){
                //计算相对坐标比例
                qreal wRatio = (double)event->pos().x() / (double)ui->mainWidget->width();
                controlWindowScale();
                //unclear number -30,实测差不多？
                this->move(QPoint(event->globalPos().x() - ui->mainWidget->width(), -30));
                lastPos = QPoint(ui->mainWidget->width() * wRatio, event->globalPos().y());
            } else this->move(event->globalPos() - lastPos);
        } else {
            QPoint d = event->globalPos() - frameGeometry().topLeft() - lastPos;
            if(mouseState & AT_LEFT) {
                this->move(this->frameGeometry().x() + d.x(), this->frameGeometry().y());
                this->resize(this->width() - d.x(), this->height());
            }
            if(mouseState & AT_RIGHT) {
                this->resize(this->width() + d.x(), this->height());
            }
            if(mouseState & AT_TOP) {
                this->move(this->frameGeometry().x(), this->frameGeometry().y() + d.y());
                this->resize(this->width(), this->height() -d.y());
            }
            if(mouseState & AT_BOTTOM){
                this->resize(this->width(), this->height() + d.y());
            }
            //更新相对坐标
            lastPos = event->globalPos() - this->frameGeometry().topLeft();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent*) {
    //重构框架
    if(border)
        border->resize(ui->mainWidget->size() + QSize(2, 2));
    //重构遮罩
    QPainterPath path;
    path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
    QRegion mask(path.toFillPolygon().toPolygon());
    ui->mainWidget->setMask(mask);
    if(miniBtn)
        miniBtn->move(ui->mainWidget->width() - verticalMargin - 2 * 12 - 2 * 6, horizontalMargin);
    if(adjBtn)
        adjBtn->move(ui->mainWidget->width() - verticalMargin - 1 * 12 - 1 * 6, horizontalMargin);
    if(closeBtn)
        closeBtn->move(ui->mainWidget->width() - verticalMargin, horizontalMargin);
    //重构子页面
    if(loginpage)
        loginpage->resize(ui->mainWidget->size());
    if(mainpage)
        mainpage->resize(ui->mainWidget->size());
}

void MainWindow::controlWindowScale() {
    //存储最大化前的框架
    //设置边框，设置阴影显示及mainwidget的style样式
    //修改框架大小,画出mainwidget的背景
    //隐藏/显示边框

    if(!maximized) {
        lastGeometry = this->frameGeometry();

        ui->verticalLayout->setContentsMargins(0, 0, 0, 0);

        windowShadow->setEnabled(false);

        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + ";border-radius:0px}";

        ui->mainWidget->setStyleSheet(mainStyle);

        this->showMaximized();
        QPainterPath path;
        path.addRect(ui->mainWidget->rect());
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);

        border->hide();

        maximized = true;
    } else {
        ui->verticalLayout->setContentsMargins(30, 30, 30, 30);

        windowShadow->setEnabled(true);

        QString mainStyle = "QWidget#mainWidget{background-color:" + mainBackGround.name() + QString::asprintf(";border-radius:%dpx}", cornerRadius);
        ui->mainWidget->setStyleSheet(mainStyle);

        this->showNormal();
        QPainterPath path;
        path.addRoundedRect(ui->mainWidget->rect(), cornerRadius - 1, cornerRadius - 1);
        QRegion mask(path.toFillPolygon().toPolygon());
        ui->mainWidget->setMask(mask);

        this->resize(lastGeometry.width(),lastGeometry.height());
        this->move(lastGeometry.x(),lastGeometry.y());

        border->show();

        maximized = false;
    }
}
