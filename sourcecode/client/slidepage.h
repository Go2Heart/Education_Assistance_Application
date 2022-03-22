#ifndef SLIDEPAGE_H
#define SLIDEPAGE_H

#include <QLabel>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "customScrollContainer.h"
#include "customWidgets.h"

class SheildLayer : public QWidget{
    Q_OBJECT
private:
    bool pressed = false;
    bool enabled = true;
    QWidget *bg;
    void mousePressEvent(QMouseEvent*) {
        if(enabled) pressed =true;
    }
    void mouseReleaseEvent(QMouseEvent*) {
        if(enabled && pressed)
            emit clicked();
    }
    void resizeEvent(QResizeEvent*) {
        bg->resize(this->parentWidget()->size());
    }
public:
    SheildLayer(QWidget *parent = nullptr) :
        QWidget(parent)
    {
        bg = new QWidget(this);
        bg->resize(parent->size());
        bg->setStyleSheet("background-color:#5a000000;border-radius:0px");
        bg->setAttribute(Qt::WA_TransparentForMouseEvents);
        bg->show();
    }
    void setEnabled(bool e) {enabled = e;}
signals:
    void clicked();
};


class SlidePage : public QWidget
{
    Q_OBJECT
private:
    //模块部分：圆角半径，返回按钮，背景widget，字体，页面名字
    //点击外部需要进行回退，所以还需要一个layer
    int cornerRadius, Width, Height, type, posy;
    ScrollAreaCustom* pageContentContainer;
    customIcon* backIcon;
    QWidget* bgWidget;
    QFont textFont = QFont("Corbel Light", 24);
    QString pageName;
    QLabel* nameLabel = nullptr;
    SheildLayer* sheildLayer;

    //状态部分：是否显示，当前动画，透明度
    bool onShown = false;
    QParallelAnimationGroup *curAni = nullptr;
    QGraphicsOpacityEffect *opacity;

    //槽函数部分，由于不需要进行拖动，我们只需要实现resize即可
    void resizeEvent(QResizeEvent*);
public:
    enum{ EXPANDING = 0, WIDTH_FIXED = 1, HEIGHT_FIXED = 2, FIXED = 3};
    //是否需要采用explicit?
    SlidePage(int radius, int type, int width, int height, QString name, QWidget *parent = nullptr, int posy = 0);
    int Type() { return type; }
    void SetRadius(int radius);
    void SetName(QString name);
    void AddContent(QWidget* widget) { pageContentContainer->addWidget(widget, false); }
    void AddContents(QVector<QWidget*> widgets) { pageContentContainer->addWidgets(widgets); }
    void RemoveContents(QVector<QWidget*> widgets) {
        for(int i = 0; i < widgets.size(); i++)
            pageContentContainer->removeWidget(widgets[i]);
    }
    //高度变化，对子模块产生影响
    void UpdateContents() { pageContentContainer->updateHeight(); }
    //回滚至顶部
    void ScrollToTop() { pageContentContainer->ScrollToTop(); }
signals:
    void sizeChange();
public slots:
    void slideIn();
    void slideOut();
};

#endif // SLIDEPAGE_H
