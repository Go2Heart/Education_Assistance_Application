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
    void enterEvent(QEvent *event);
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
    qreal value(){return selectedID;}

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

class bigIconButton : public QWidget {
    Q_OBJECT

private:
    QString defaultColor = "#0a0078D4";
    QString hoverColor = "#1a0078D4";
    QString pressColor = "#2a0078D4";
    QString noColor = "#00000000";

    enum {ICON = 1, TEXT = 2, FRAMELESS = 4, DISABLE = 8};
    QPixmap* iconImg = nullptr;
    QLabel* icon = nullptr;
    QLabel* text = nullptr;
    QWidget* bgWidget = nullptr;

    int cornerRadius, margin = 10, buttonType;
    QString radiusStyle;
    qreal scale = 1.0;

    bool selectable = false;
    bool mousePressed = false;
    bool onSelected = false;

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    bigIconButton(int type, const QString &iconPath = "", const QString &description = "", int radius = 0, QWidget* parent = nullptr);
    void setSelectable(bool sel = true){selectable = sel;}
    void setScale(qreal scale);
    void setPixmap(QString iconPath);

signals:
    void clicked();
    void selected();
};

class textInputItem : public QWidget{
    Q_OBJECT

private:
    const int margin = 10;
    const int spacing = 10;
    QLabel* itemName;
    QLineEdit* editor;
    QWidget* bgWidget;
    QWidget* indicator;
    QGraphicsOpacityEffect* opac;

    QString curText = "";
    bool mousePressed = false;
    bool onEditing = false;

    bool enabled = true;
    int type = 0;
    int nameWidth = 0;

    void enterEditEffect();
    void leaveEditEffect();

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);
    //void focusOutEvent(QFocusEvent* event);

public:
    textInputItem(const QString &name, QWidget* parent = nullptr, int type = 0);
    QLineEdit* lineEditor(){return editor;}
    QString value(){return editor->text();}

    void setValue(const QString &text);
    void setValidator(QValidator* vali){editor->setValidator(vali);}
    void setEnabled(bool enable = true){enabled = enable;}

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

class singleSelectGroupVertical : public QWidget{
    Q_OBJECT

private:
    const int middleSpacing = 5;
    const int bottomSpacing = 30;
    QLabel* groupName;
    QHBoxLayout* mainLayout;
    int selectedID = -1;
    QVector<selectionItem*> selections;

public:
    singleSelectGroupVertical(QString name = "", QWidget* parent = nullptr);
    void AddItem(selectionItem* item);
    void RemoveItem(selectionItem* item);
    void SetSelection(selectionItem* item);
    qreal value(){return selectedID;}

signals:
    void selectedItemChange(int selectID);
    void itemChange();

private slots:
    void changeSelection(selectionItem* item);
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
        container->addWidget(w, true);
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

class customWidget : public QWidget {
    Q_OBJECT

private:
    const int margin = 10;
    const int spacing = 10;
    QLabel* itemName;
    QWidget* bgWidget;
    QWidget* contentWidget;

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void resizeEvent(QResizeEvent* event);

public:
    customWidget(const QString &name, QWidget* content, QWidget* parent = nullptr);
};

#endif // CUSTOMWIDGETS_H
