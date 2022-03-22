#include "customScrollContainer.h"

ScrollAreaCustom::ScrollAreaCustom(bool b, QWidget *parent) :
    indicatorEnabled(b),
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    container = new ScrollListContainer(this);
    container->move(0, 0);
    container->resize(this->width() - margin, 3);
    getCord = new QTimer;
    getCord->setSingleShot(true);
    rfrshView = new QTimer;
    rfrshView->setSingleShot(true);

    indicator = new ScrollIndicator(this);
    //indicator->resize(6,0);
    //indicator->setStyleSheet("border-radius:3px;background-color:#646464");
    //indicator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //indicator->setMouseTracking(true);
    indicator->resize(6, (int)((double)this->height() * this->height() / container->height()));
    indicator->move(this->width() - indicator->width(), 0);
    //indicator->show();

    //tempindicator = new QWidget(this);
    //tempindicator->setStyleSheet("border-radius:3px;background-color:#646464");
    //tempindicator->resize(6, (int)((double)this->height() * this->height() / container->height()));
    //tempindicator->move(this->width() - tempindicator->width() - 3, 0);
    //tempindicator->show();

    this->setMouseTracking(true);
    container->setMouseTracking(true);
    indicator->setMouseTracking(true);

    bounce = new QPropertyAnimation(container, "pos");

    QObject::connect(getCord, SIGNAL(timeout()), this, SLOT(updateSpd()));
    QObject::connect(rfrshView, SIGNAL(timeout()), this, SLOT(scrollContainer()));
    QObject::connect(indicator, SIGNAL(scrollPage(int)), this, SLOT(scrollIndicator(int)));
}

void ScrollAreaCustom::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setPen(QColor(200, 200, 200));
    painter.setBrush(Qt::NoBrush);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(this->rect(), 12, 12);
    //qDebug()<< width() << " " << height();

    container->resize(this->width() - margin, container->height());
    //missing
    if(container->height() > this->height() && container->y() < this->height() - container->height() && curSpd == 0) {
        //container->move(container->x(), container->height())
    }
    if(container->height() <= this->height()) {
        container->move(container->x(), 0);
        indicator->hide();
    } else
        indicator->show();
    if(!indicatorEnabled)
        indicator->hide();
    indicator->resize(indicator->width(), (int)((double)this->height() * this->height() / container->height()));
    /*
    qDebug()<<indicator->width()<<"--"<<indicator->height();
    qDebug()<<indicator->styleSheet();
    qDebug()<<(int)((double)this->height() * this->height() / container->height())<<"---"<<container->height()<<"---"<<this->height();
    qDebug()<<"nowxy:"<<this->pos().x()<<"---"<<this->pos().y();*/
    indicator->move(this->width() - indicator->width(), -container->y() * this->height() / container->height());
}

void ScrollAreaCustom::mousePressEvent(QMouseEvent *event) {
    if(container->height() > this->height()) {
        if(container->y() <= 0 && container->y() + container->height() >= this->height())
            pressed = true;
        lastY = event->pos().y();
    }
    rfrshView->stop();
    curSpd = 0;
    outofEdge = false;
    moveStored = 0;
    nextMove = 1;
}

void ScrollAreaCustom::mouseMoveEvent(QMouseEvent *event) {
    if(pressed){
        if(!getCord->isActive() && event->pos().y() - lastY != 0) {
            getCord->start(30);
            strtY = event->pos().y();
        }
        //内部直接移动，外部需要用根号函数拟合
        if(container->y() <= 0 && container->y() + container->height() >= this->height())
            container->move(container->x(), container->y() + event->pos().y() - lastY);
        else {
            //后置判断比起move后判定outofedge会更为省事？
            //注意这里存储的是鼠标位置
            if(!outofEdge) {
                bfEdgeY = event->pos().y();
                container->move(container->x(),container->y() + event->pos().y() - lastY);
                outofEdge = true;
            } else {
                //接下来要根据当前事件位置来判定结束位置位于哪里
                int pos = container->y() >= 0 ? 1 : -1;
                int dp = event->pos().y() - bfEdgeY;
                if(!dp || dp / abs(dp) != pos) {
                    outofEdge = false;
                    container->move(container->x(), container->y() + dp);
                    nextMove = 1;
                    moveStored = 0;
                } else {
                    while(moveStored + nextMove <= abs(dp)) {
                        moveStored += nextMove;
                        container->move(container->x(), container->y() + pos);
                        nextMove++;
                    }
                    while(nextMove > 1 && moveStored > abs(dp)) {
                        nextMove--;
                        moveStored -= nextMove;
                        container->move(container->x(), container->y() - pos);
                    }
                }
            }
        }
        lastY = event->pos().y();
    }
}

void ScrollAreaCustom::mouseReleaseEvent(QMouseEvent *event) {
    if(container->y() > 0 || container->y() + container->height() < this->height())
        bounceback();
    else rfrshView->start(30);
    pressed = false;
}

void ScrollAreaCustom::wheelEvent(QWheelEvent *event) {
    if(container->y() > 0 || container->y() + container->height() < this->height()) return;
    bool newDirection = event->angleDelta().y() > 0;
    curSpd += 5;
    if(newDirection != scrollDown) curSpd = 5;
    if(curSpd > MAXSPEED) curSpd = MAXSPEED;
    scrollDown = newDirection;
    if(!rfrshView->isActive())
        rfrshView->start(30);
}

void ScrollAreaCustom::bounceback() {
    rfrshView->stop();
    bounce->setDuration(500);
    bounce->setStartValue(container->pos());
    if(container->y() > 0)
        bounce->setEndValue(QPoint(container->x(), 0));
    else
        bounce->setEndValue(QPoint(container->x(), this->height() - container->height()));
    bounce->setEasingCurve(QEasingCurve::OutQuad);
    bounce->start();
}

void ScrollAreaCustom::scrollContainer(){
    if(curSpd > 0) {
        //当且仅当回滚顶部的时候会忽略最大速度，而且一旦降到maxspeed速度内就需归位
        if(curSpd > MAXSPEED && !ignoreMaxSpeed)
            curSpd = MAXSPEED;
        else if(curSpd <= MAXSPEED) ignoreMaxSpeed = false;
        int dp = scrollDown ? curSpd : -curSpd;
        container->move(container->x(), container->y() + dp);
    } else return;
    //如果在内，就正常减速，否则快速减速
    if(container->y() <= 0 && container->y() + container->height() >= this->height()) {
        curSpd -= damp;
        curSpd = curSpd < 0 ? 0 : curSpd;
    }
    else curSpd /= 2;
    if(curSpd == 0 && (container->y() > 0 || container->y() + container->height() < this->height()))
        bounceback();
    else
        rfrshView->start(30);
}

void ScrollAreaCustom::updateSpd() {
    int spd = lastY - strtY;
    scrollDown = spd >= 0;
    strtY = lastY;
    curSpd = abs(spd);
}

void ScrollAreaCustom::ScrollToTop() {
    curSpd = sqrt(2 * -container->pos().y()) + 1;
    scrollDown = true;
    getCord->stop();
    rfrshView->stop();
    moveStored = 0;
    nextMove = 1;
    outofEdge = false;
    ignoreMaxSpeed = true;
    rfrshView->start(30);
}

void ScrollAreaCustom::scrollIndicator(int dp) {
    int newY = container->y() - dp * container->height() / this->height();
    if(newY > 0) newY = 0;
    else if(newY < this->height() - container->height())
        newY = this->height() - container->height();
    container->move(container->x(), newY);
    update();
}

void ScrollAreaCustom::addWidget(QWidget *widget, bool setAnimation) {
    //important
    widget->setParent(container);
    container->AddWidget(widget, setAnimation);
}

void ScrollAreaCustom::removeWidget(QWidget *widget) {
    container->RemoveWidget(widget);
}

void ScrollAreaCustom::updateHeight() {
    container->updateHeight();
}

ScrollListContainer::ScrollListContainer(QWidget *parent) :
    QWidget(parent)
{

}

void ScrollListContainer::AddWidget(QWidget *widget, bool setAnimation) {
    //整体加长
    this->resize(this->width(), this->height() + widget->height() + spacing);
    widgets.push_back(widget);
    size++;
    ys.push_back(0);

    //重订宽度
    widget->resize(this->width(), widget->height());
    widget->show();

    if(setAnimation) {
        QGraphicsOpacityEffect *widgetOpac = new QGraphicsOpacityEffect(widget);
        widgetOpac->setOpacity(0);
        widget->setGraphicsEffect(widgetOpac);
        QParallelAnimationGroup *dpGroup = new QParallelAnimationGroup;
        QSequentialAnimationGroup *newWidgetFadeIn = new QSequentialAnimationGroup;
        for(int i = 0; i < size - 1; i++) {
            ys[i] += widget->height() + spacing;
            QPropertyAnimation *move = new QPropertyAnimation(widgets[i], "pos");
            move->setDuration(750);
            move->setStartValue(widgets[i]->pos());
            move->setEndValue(QPoint(widgets[i]->pos().x(), ys[i]));
            dpGroup->addAnimation(move);
        }
        newWidgetFadeIn->addPause(300);
        QPropertyAnimation *fade = new QPropertyAnimation(widgetOpac, "opacity", widget);
        fade->setDuration(300);
        fade->setStartValue(0);
        fade->setEndValue(0.99);
        newWidgetFadeIn->addAnimation(fade);
        dpGroup->addAnimation(newWidgetFadeIn);
        dpGroup->start();
        connect(dpGroup,
            &QPropertyAnimation::stateChanged,
            [=] {
                if(dpGroup->state() == QAbstractAnimation::Stopped) {
                    if(widgetOpac->opacity() != 0.99) {
                        fade->start(QAbstractAnimation::DeleteWhenStopped);
                        connect(fade,
                            &QPropertyAnimation::finished,
                            [=] {
                            //why here deleteLater
                                widgetOpac->deleteLater();
                        });
                    } else {
                        dpGroup->deleteLater();
                        widgetOpac->deleteLater();
                    }
                }
        });
    } else {
        for(int i = 0; i < size - 1; i++) {
            ys[i] += widget->height() + spacing;
            widgets[i]->move(QPoint(widgets[i]->pos().x(), ys[i]));
        }
    }
}

void ScrollListContainer::RemoveWidget(QWidget *widget) {
    int index;
    if(widget == nullptr) {
        index = size - 1;
        if(index != -1) widget = widgets[index];
    } else index = widgets.indexOf(widget);
    if(index == -1 || widget == nullptr) return;
    this->resize(this->width(), this->height() - widget->height() - spacing);
    //why here?
    this->parentWidget()->update();
    widget->hide();
    widget->setParent(nullptr);
    QParallelAnimationGroup *dpGroup = new QParallelAnimationGroup;
    for(int i=index - 1; i >= 0; i--) {
        ys[i] -= widget->height() + spacing;
        QPropertyAnimation *move = new QPropertyAnimation(widgets[i], "pos");
        move->setDuration(750);
        move->setStartValue(widgets[i]->pos());
        move->setEndValue(QPoint(widgets[i]->pos().x(), ys[i]));
        dpGroup->addAnimation(move);
    }
    dpGroup->start(QAbstractAnimation::DeleteWhenStopped);
    widgets.remove(index);
    size--;
    ys.remove(index);
}

void ScrollListContainer::updateHeight() {
    for(int i = size - 2; i >= 0; i--) {
        ys[i] = ys[i + 1] + widgets[i + 1]->height() + spacing;
        widgets[i]->move(widgets[i]->pos().x(), ys[i]);
    }
    this->resize(this->width(), ys[0] + widgets[0]->height() + spacing);
}

void ScrollListContainer::clear() {
    int n = size;
    for(int i = 0; i < n; i++) RemoveWidget();
}

void ScrollListContainer::paintEvent(QPaintEvent *event) {
    for(int i = 0; i < widgets.size(); i++) {
        widgets[i]->resize(this->width(), widgets[i]->height());
    }
}

//自定义原件不能采用qstyle的setstylesheet
void ScrollIndicator::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(curColor);
    painter.drawRoundedRect(this->rect(), 4, 4);
}

ScrollIndicator::ScrollIndicator(QWidget *parent) :
    QWidget(parent)
{
    this->resize(6,0);
    curColor = defaultColor;
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setMouseTracking(true);
}

void ScrollIndicator::enterEvent(QEvent *event) {
    curColor = hoverColor;
    update();
}

void ScrollIndicator::leaveEvent(QEvent *event) {
    curColor = defaultColor;
    update();
}

void ScrollIndicator::mousePressEvent(QMouseEvent *event) {
    curColor = pressColor;
    pressed = true;
    lastY = event->globalPos().y();
    update();
}

void ScrollIndicator::mouseMoveEvent(QMouseEvent *event) {
    if(pressed) {
        int dp = event->globalPos().y() - lastY;
        emit scrollPage(dp);
        lastY = event->globalPos().y();
    }
}

void ScrollIndicator::mouseReleaseEvent(QMouseEvent *event) {
    curColor = hoverColor;
    pressed = false;
    update();
}
