#include "slidepage.h"

SlidePage::SlidePage(int radius, int type, int Width, int Height, QString name, QWidget *parent, int posy) :
    QWidget(parent),
    type(type),
    Width(Width),
    Height(Height),
    cornerRadius(radius),
    pageName(name),
    posy(posy)
{
    QVBoxLayout *mainLayouter = new QVBoxLayout(this);
    mainLayouter->setContentsMargins(10, 10, 10, 10);
    mainLayouter->setAlignment(Qt::AlignTop);
    this->setLayout(mainLayouter);
    //初始隐藏窗口
    if(!(type & WIDTH_FIXED)) {

    }
    if(!(type & HEIGHT_FIXED)) {
        Height = parent->height();
    }
    this->resize(Width, Height);
    this->move(QPoint(-this->width() - 30, posy));

    opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0);

    pageContentContainer = new ScrollAreaCustom(false, this);
    pageContentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //设置初始透明度
    pageContentContainer->setGraphicsEffect(opacity);
    mainLayouter->addWidget(pageContentContainer);

    //搞个title.jpg
    if(name.length()) {
        //backIcon = new customIcon(":/icons/icons/back.svg", "", 5, this);
        /*connect(
            backIcon,
            &QPushButton::clicked,
            this,
            [=] {
                slideOut();
            }
        );*/
        nameLabel = new QLabel(pageName, this);
        nameLabel->setGraphicsEffect(opacity);
        //字体模糊
        textFont.setStyleStrategy(QFont::PreferAntialias);
        nameLabel->setFont(textFont);
        QWidget *titleBar = new QWidget(this);
        QHBoxLayout *titleLayouter = new QHBoxLayout(titleBar);
        //titleLayouter->addWidget(backIcon);

        titleLayouter->addWidget(nameLabel);
        titleLayouter->setAlignment(Qt::AlignLeft);
        titleBar->setLayout(titleLayouter);
        //qDebug()<<nameLabel->height();
        mainLayouter->addWidget(titleBar);
    }

    //为背景widget设置style
    QString style;
    style = "background-color:white;border-radius:" + QString::asprintf("%dpx", cornerRadius);
    bgWidget = new QWidget(this);
    //放到背景板后，这样需要的时候raise即可
    bgWidget->lower();
    bgWidget->resize(this->size());
    bgWidget->setStyleSheet(style);
    bgWidget->setMouseTracking(true);
    bgWidget->show();

    //shield搭建
    sheildLayer = new SheildLayer(this->parentWidget());
    sheildLayer->resize(this->parentWidget()->size());
    sheildLayer->setGraphicsEffect(opacity);
    sheildLayer->setMouseTracking(true);
    connect(
        sheildLayer,
        &SheildLayer::clicked,
        this,
        [=] {
            slideOut();
        }
    );

    //阴影设置
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0));
    shadow->setOffset(0, 0);
    this->setGraphicsEffect(shadow);

    //设置策略（焦点策略（多个子窗口的选中）鼠标跟踪 圆角弧度
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
    //why not set or set?
    this->setStyleSheet(style);
}

//resize指本层重构后如何对子图形产生影响
void SlidePage::resizeEvent(QResizeEvent*) {
    bgWidget->resize(this->size());
    sheildLayer->resize(this->parentWidget()->size());
    if(!onShown && !curAni)
        this->move(QPoint(-this->width() - 30, posy));
    //若此时正在渐出动画中，我们需要把动画终点改变
    else if(!onShown && curAni)
        emit sizeChange();
}

void SlidePage::SetRadius(int radius) {
    cornerRadius = radius;
    QString style;
    style = "background-color:white;border-radius:" + QString::asprintf("%dpx", cornerRadius);
    this->setStyleSheet(style);
}

void SlidePage::SetName(QString name) {
    pageName = name;
    if(nameLabel)
        nameLabel->setText(pageName);
}

void SlidePage::slideIn() {
    if(curAni) {
        curAni->stop();
        curAni->deleteLater();
        curAni = nullptr;
    }
    onShown = true;
    sheildLayer->raise();
    sheildLayer->setEnabled(true);
    this->raise();
    sheildLayer->show();
    QParallelAnimationGroup *inGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *slideInAni = new QPropertyAnimation(this, "pos", this);
    slideInAni->setStartValue(this->pos());
    slideInAni->setEndValue(QPoint(0, posy));
    slideInAni->setDuration(1000);
    slideInAni->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fadeInAni = new QPropertyAnimation(opacity, "opacity", this);
    fadeInAni->setStartValue(opacity->opacity());
    fadeInAni->setEndValue(0.99);
    fadeInAni->setDuration(750);
    inGroup->addAnimation(slideInAni);
    inGroup->addAnimation(fadeInAni);
    connect(
        inGroup,
        &QParallelAnimationGroup::finished,
        this,
        [=] {
            this->curAni = nullptr;
        }
    );
    inGroup->start();
    curAni = inGroup;
}

void SlidePage::slideOut() {
    if(curAni) {
        curAni->stop();
        curAni->deleteLater();
        curAni = nullptr;
    }
    onShown = false;
    sheildLayer->setEnabled(false);
    QParallelAnimationGroup *outGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *slideOutAni = new QPropertyAnimation(this, "pos", this);
    slideOutAni->setStartValue(this->pos());
    slideOutAni->setEndValue(QPoint(- this->width() - 30, posy));
    slideOutAni->setDuration(1000);
    slideOutAni->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fadeOutAni = new QPropertyAnimation(opacity, "opacity", this);
    fadeOutAni->setStartValue(opacity->opacity());
    fadeOutAni->setEndValue(0);
    fadeOutAni->setDuration(750);
    outGroup->addAnimation(slideOutAni);
    outGroup->addAnimation(fadeOutAni);
    connect(
        outGroup,
        &QParallelAnimationGroup::finished,
        this,
        [=] {
            this->curAni = nullptr;
            sheildLayer->hide();
            pageContentContainer->ScrollToTop();
        }
    );
    connect(
        this,
        &SlidePage::sizeChange,
        slideOutAni,
        [=] {
            slideOutAni->setEndValue(QPoint(- this->width() - 30, posy));
        }
    );
    outGroup->start();
    curAni = outGroup;
}
