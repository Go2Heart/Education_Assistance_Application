#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include <QPushButton>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSlider>
#include <QString>
#include <QtSvg>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QFocusEvent>
#include <QtGui>
#include <algorithm>
#include "customScrollContainer.h"
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QComboBox>
#include <QTableWidget>
#include <QStyleOptionViewItem>
#include <QHeaderView>
#include <QScrollBar>


class customIcon : public QPushButton{
    Q_OBJECT

    Q_PROPERTY(qreal rotationAngle READ rotationAngle WRITE setRotationAngle NOTIFY rotationAngleChanged)

private:
    int radius;
    qreal widgetRatio;
    qreal iconSizeRate = 0.8;
    qreal rotation = 0;
    QPixmap *iconImg;
    QString iconHint;

    /* for hover and click effects */
    QColor bgColor;
    QColor defaultColor = QColor(0, 0, 0, 0);
    QColor hoverColor = QColor(241, 241, 241, 200);

protected:
    void paintEvent(QPaintEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

protected:
    qreal rotationAngle() const {return rotation;}

public:
    customIcon(QString iconPath, QString hint = "", int r = 0, QWidget *parent = nullptr);
    customIcon(const QPixmap &icon, QString hint = "", int r = 0, QWidget *parent = nullptr);

    void setRotationAngle(qreal angle = 0){rotation = angle;update();}

signals:
    void rotationAngleChanged();
};

class selectionItem : public QWidget
{
    Q_OBJECT

private:
    QLabel* title;
    QLabel* description;
    QWidget* indicator;
    QWidget* mainContent;
    QWidget* bgWidget;
    QGraphicsOpacityEffect* opac;
    bool onSelected = false;
    bool mousePressed = false;

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    selectionItem(QString name, QString info = "", QWidget* parent = nullptr);
    void Select();
    void Deselect();
    void setTitle(QString titleText){title->setText(titleText);}
    void setDescription(QString descText){description->setText(descText);}

signals:
    void selected(selectionItem* item);
    //void heightChange();

};

class singleSelectGroup : public QWidget
{
    Q_OBJECT

private:
    const int middleSpacing = 5;
    const int bottomSpacing = 30;
    QLabel* groupName;
    QVBoxLayout* mainLayout;
    int selectedID = -1;
    QVector<selectionItem*> selections;

public:
    singleSelectGroup(QString name = "", QWidget* parent = nullptr);
    void AddItem(selectionItem* item);
    void RemoveItem(selectionItem* item);
    void SetSelection(selectionItem* item);
    int value(){return selectedID;}

signals:
    void selectedItemChange(int selectID);
    void itemChange();

private slots:
    void changeSelection(selectionItem* item);
};

class horizontalValueAdjuster : public QWidget{
    Q_OBJECT

private:
    const int middleSpacing = 5;
    const int bottomSpacing = 10;
    QLabel* title;
    qreal curValue;
    qreal minValue;
    qreal maxValue;
    qreal stepValue;
    QWidget* editArea;
    QLabel* valueLabel;
    //QDoubleSpinBox* editLabel;
    customIcon* decreaseBtn;
    customIcon* increaseBtn;
    QSlider* slider;

public:
    horizontalValueAdjuster(QString name, qreal min, qreal max, qreal step, QWidget* parent = nullptr);
    void setValue(qreal value);
    qreal value(){return curValue;}

signals:
    void valueChanged(qreal value);

};

//class itemGroup : public QWidget{
//    Q_OBJECT
//
//private:
//    const int middleSpacing = 5;
//    const int bottomSpacing = 30;
//    QLabel* groupName;
//    QVBoxLayout* mainLayout;
//    QVector<QWidget*> items;
//
//public:
//    itemGroup(const QString &name, QWidget* parent = nullptr);
//    void AddItem(QWidget* item);
//    void RemoveItem(QWidget* item);
//};


class bigIconButton : public QWidget { // 带边框的多功能按钮类，可用于内置元素或者图片，图片会根据按钮大小进行自适配重新渲染
    Q_OBJECT
private:
    QString defaultColor = "#0a0078D4"; // 默认背景颜色
    QString hoverColor = "#1a0078D4"; // 鼠标悬停背景颜色
    QString pressColor = "#2a0078D4"; // 鼠标按下背景颜色
    QString noColor = "#00000000"; // 当类型为FRAMELESS时，不对背景做渲染
    QPixmap* iconImg = nullptr; // 图标
    QLabel* icon = nullptr;
    QLabel* text = nullptr; // 标题
    QWidget* bgWidget = nullptr; // 背景

    int cornerRadius, margin = 10, buttonType; // 圆角半径，边界，按钮类型
    QString radiusStyle;
    qreal scale = 1.0; // 图片放缩大小

    bool selectable = false; // 是否可以选中
    bool mousePressed = false; // 鼠标是否按下
    bool onSelected = false; // 是否被选中

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);

public:
    enum {ICON = 1, TEXT = 2, FRAMELESS = 4, DISABLE = 8};
    bigIconButton(int type, const QString &iconPath = "", const QString &description = "", const QString &fontstyle = "Corbel", int fontsize = 10, int radius = 0, QWidget* parent = nullptr);
    void setSelectable(bool sel = true){selectable = sel;} // 设置是否可以选择
    void setScale(qreal scale); // 设置图片放缩倍率
    void setPixmap(QString iconPath); // 传进新的图片路径，重新渲染图片

signals:
    void clicked();
    void selected();
};

class textInputItem : public QWidget { // 文本输入框
    Q_OBJECT

private:
    const int margin = 10; // 边界
    const int spacing = 10; // 标题与editor间隔
    QLabel* itemName; // 标题
    QLineEdit* editor; // 编辑框
    QWidget* bgWidget; // 背景
    QWidget* editorbgWidget;
    QWidget* indicator; // 指示条
    QGraphicsOpacityEffect* opac; // 透明度效果，用于indicator渐入

    QString curText = ""; // 当前内容
    bool mousePressed = false; // 鼠标是否按下
    bool onEditing = false; // 是否可编辑

    bool enabled = true; // 是否启用，不起用不会有悬停和按下效果
    bool isPassword = false; // 是否是密码，弃用
    int type = 0; // 输入框类型
    int nameWidth = 0;

    void enterEditEffect(); // 编辑输入框动画模块
    void leaveEditEffect(); // 离开编辑输入框动画模块

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    textInputItem(const QString &name, QWidget* parent = nullptr, int type = 0);
    QLineEdit* lineEditor(){return editor;} // 返回editor
    QString value(){return editor->text();} // 返回editor当前内容

    void setValue(const QString &text); // 设置editor文字
    void setValidator(QValidator* vali){editor->setValidator(vali);} //设置editor文本过滤器，如正则式，整数过滤器等
    void setIntValidator() {
        QRegExp regx("^[1-9][0-9]+$");
        setValidator(new QRegExpValidator(regx, this));
    }
    void setNumberValidator() {
        QRegExp regx("^[0-9]+$");
        setValidator(new QRegExpValidator(regx, this));
    }
    void setEnabled(bool enable = true){enabled = enable;} // 是否启用
    void setPasswordMode() { isPassword = true; editor->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit); } // 密码模式（弃用）

signals:
    void textEdited(QString text);
};
class textButton : public QWidget {
    Q_OBJECT

private:
    QLabel* btnText;
    QWidget* bgWidget;
    QString defaultColor = "#0a0078d4";
    QString hoverColor = "#1a0078d4";
    QString pressedColor = "#2a0078d4";

    bool mousePressed;

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    textButton(QString text, QWidget* parent = nullptr, qreal ratio = 0.5);
    textButton(QString text, QString defC, QString hoverC, QString pressedC, QWidget* parent = nullptr, qreal ratio = 0.5);
    void setText(QString s) { btnText->setText(s); }


signals:
    void clicked();
};

class selectionItemVertical : public QWidget // 横向的选择item类，数据成员与成员函数意义同selectionItem类
{
    Q_OBJECT
private:
    QLabel* title;
    QWidget* indicator;
    QWidget* bgWidget;
    QGraphicsOpacityEffect* opac;
    bool onSelected = false;
    bool mousePressed = false;
    int margin = 5;

    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void resizeEvent(QResizeEvent*);

public:
    selectionItemVertical(QString name, QWidget* parent = nullptr);
    void Select();
    void Deselect();
    void setTitle(QString titleText){title->setText(titleText);}

signals:
    void selected(selectionItemVertical* item);
    //void heightChange();

};

class singleSelectGroupVertical : public QWidget { // 横向的选择item群，数据成员与成员函数意义同singleSelectGroup类
    Q_OBJECT

private:
    const int middleSpacing = 5;
    QLabel* groupName;
    QHBoxLayout* mainLayout;
    int selectedID = -1;
    QVector<selectionItemVertical*> selections;

public:
    singleSelectGroupVertical(QString name = "", QWidget* parent = nullptr);
    void AddItem(selectionItemVertical* item);
    void RemoveItem(selectionItemVertical* item);
    void SetSelection(selectionItemVertical* item);
    int value() const { return selectedID; }

signals:
    void selectedItemChange(int selectID);
    void itemChange();

private slots:
    void changeSelection(selectionItemVertical* item);
};
class topButton : public QPushButton {
    Q_OBJECT
private:
    bool event(QEvent* e);
public:
    topButton(QWidget* parent = nullptr);
};

class QLineDelegate : public QStyledItemDelegate {
public:
    QLineDelegate(QTableView* tableView);
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
    QPen pen;
    QTableView* view;
};

class ClockTable : public QTableWidget {
    Q_OBJECT
private:
    int Width = 100, Height = 30;
public:
    ClockTable(QWidget* parent = nullptr);
};


class foldWidget : public QWidget {
    Q_OBJECT
private:
    QWidget* titleWidget;
    QLabel* nameLabel;
    bigIconButton* leftIcon;
    bigIconButton* downIcon;
    QVector<bigIconButton*> extraIcons;

    ScrollAreaCustom* container;
    int overlap = 5, margin = 10, titleHeight = 40, maxHeight, spacing = 3;
    bool fold = true;
    void resizeEvent(QResizeEvent*);
    void foldChange();
public:
    foldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* parent = nullptr);
    void addContent(QWidget* w) {
        container->addWidget(w, false);
    }
    void clear() {
        container->clear();
    }
signals:
    void clicked(int id);
};

class ComboBox : public QComboBox {
    Q_OBJECT
public:
    ComboBox(QWidget* parent = nullptr);
    void setNumberRange(int l, int r);
};

class customWidget : public QWidget { // 包裹窗体类，用于将没有边框的窗口包裹
    Q_OBJECT

private:
    const int margin = 10; // 边框
    const int spacing = 10; // 间隔
    QLabel* itemName; // 名字
    QWidget* bgWidget; // 背景
    QWidget* contentWidget; // 内容widget
    int type;
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void resizeEvent(QResizeEvent* event);
public:
    customWidget(const QString &name, QWidget* content, QWidget* parent = nullptr, int type = 0);
};

class textItem : public QLabel { // 文字类，可自定义字体大小，字体样式，文字内容
    Q_OBJECT
public:
    QFont font;
    textItem(QString text, QString fontstyle, int fontsize, QWidget* parent = nullptr, QString style = "color: black;");
    void SetText(QString text) {
        setText(text);
        QFontMetrics fm(font);
        setFixedWidth(fm.size(Qt::TextSingleLine, this->text()).width() + 3);
    }
};

#endif // CUSTOMWIDGETS_H
