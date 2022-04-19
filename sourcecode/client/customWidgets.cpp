#include "customWidgets.h"

//*********************************************************//
//CustomIcon class implementation
//*********************************************************//

customIcon::customIcon(QString iconPath, QString hint, int r, QWidget* parent) :
    QPushButton(parent),
    radius(r),
    iconHint(hint)
{
    //qDebug()<<height();
    QSvgRenderer renderer;
    renderer.load(iconPath);
    QSize size = renderer.defaultSize();
    iconImg = new QPixmap(size);
    iconImg->fill(Qt::transparent);
    QPainter painter(iconImg);
    painter.setRenderHints(QPainter::Antialiasing);
    renderer.render(&painter);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetRatio = iconImg->height() / iconImg->width();
    bgColor = defaultColor;
}

customIcon::customIcon(const QPixmap &icon, QString hint, int r, QWidget *parent):
    QPushButton(parent),
    radius(r),
    iconHint(hint)
{
    iconImg = new QPixmap(icon);
    widgetRatio = iconImg->height() / iconImg->width();
    bgColor = defaultColor;
}

void customIcon::paintEvent(QPaintEvent *event){
    //qDebug()<<"height:"<<height()<<"width:"<<width();
    resize(height() / widgetRatio,  height());
    QPainter bgPainter(this);
    bgPainter.setRenderHints(QPainter::Antialiasing);
    bgPainter.setPen(Qt::NoPen);
    bgPainter.setBrush(bgColor);
    bgPainter.drawRoundedRect(this->rect(), radius, radius);

    QPainter pixmapPainter(this);
    pixmapPainter.setRenderHints(QPainter::Antialiasing);
    pixmapPainter.translate(width() / 2, height() / 2);
    pixmapPainter.rotate(rotation);
    pixmapPainter.translate(-width() / 2, -height() / 2);
    int w = iconSizeRate * width();
    int h = iconSizeRate * height();
    pixmapPainter.drawPixmap(width() / 2 - w / 2, height() / 2 - h / 2, w, h, *iconImg);
}

void customIcon::enterEvent(QEvent *event){
    bgColor = hoverColor;
    update();
}

void customIcon::leaveEvent(QEvent *event){
    bgColor = defaultColor;
    update();
}

void customIcon::mousePressEvent(QMouseEvent *event){
    emit clicked();
    setFocus();
    iconSizeRate -= 0.1;
    update();
}

void customIcon::mouseReleaseEvent(QMouseEvent *event){
    iconSizeRate += 0.1;
    update();
}


//*********************************************************//
//selectionItem class implementation
//*********************************************************//

selectionItem::selectionItem(QString name, QString info, QWidget *parent) :
    QWidget(parent){
    /* set labels */
    QFont titleFont = QFont("Corbel", 13);
    QFontMetrics fm(titleFont);
    qreal height = fm.lineSpacing();
    title = new QLabel(this);
    title->setText(name);
    title->setFont(titleFont);
    title->setMinimumHeight(height);
    title->setStyleSheet("color:#2c2c2c");
    title->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    QFont descFont = QFont("Corbel Light", 11);
    fm = QFontMetrics(descFont);
    height = fm.lineSpacing();
    description = new QLabel(this);
    description->setText(info);
    description->setFont(descFont);
    description->setMinimumHeight(height);
    description->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    description->setStyleSheet("color:#707070");

    indicator = new QWidget(this);

    /* set minimum height and layout */
    setFixedHeight(title->height() + (info == "" ? 0 : description->height() + 5));
    indicator->resize(6, 0.4 * this->height());
    indicator->move(4, 0.3 * this->height());
    indicator->setStyleSheet("border-radius:3px;background-color:#0078D4");
    opac = new QGraphicsOpacityEffect(indicator);
    opac->setOpacity(0);
    indicator->setGraphicsEffect(opac);

    QVBoxLayout *contentLayout = new QVBoxLayout(this);
    contentLayout->setContentsMargins(20, 0, 0, 0);
    contentLayout->setSpacing(2);
    this->setLayout(contentLayout);
    contentLayout->addWidget(title);
    if(info != "")
        contentLayout->addWidget(description);
    contentLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* set background widget */
    bgWidget = new QWidget(this);
    bgWidget->resize(this->size());
    bgWidget->setStyleSheet("border-radius:5px;background-color:#00000000");
    bgWidget->lower();
    bgWidget->show();

    this->setMouseTracking(true);
}

void selectionItem::enterEvent(QEvent *event){
    bgWidget->setStyleSheet("border-radius:5px;background-color:#0a000000");
    QParallelAnimationGroup *enter = new QParallelAnimationGroup(this);
    QPropertyAnimation *longer = new QPropertyAnimation(indicator, "geometry", this);
    longer->setStartValue(indicator->geometry());
    longer->setEndValue(QRectF(4, 0.25 * this->height(), 6, this->height() * 0.5));
    longer->setDuration(150);
    longer->setEasingCurve(QEasingCurve::OutBack);
    QPropertyAnimation *fadeIn = new QPropertyAnimation(opac, "opacity", this);
    fadeIn->setStartValue(opac->opacity());
    fadeIn->setEndValue(0.99);
    fadeIn->setDuration(100);
    enter->addAnimation(longer);
    enter->addAnimation(fadeIn);
    enter->start();
}

void selectionItem::leaveEvent(QEvent *event){
    bgWidget->setStyleSheet("border-radius:5px;background-color:#00000000");
    QParallelAnimationGroup *leave = new QParallelAnimationGroup(this);
    QPropertyAnimation *shorter = new QPropertyAnimation(indicator, "geometry", this);
    shorter->setStartValue(indicator->geometry());
    shorter->setEndValue(QRectF(4, 0.3 * this->height(), 6, this->height() * 0.4));
    shorter->setDuration(150);
    shorter->setEasingCurve(QEasingCurve::OutBack);
    QPropertyAnimation *fadeOut = new QPropertyAnimation(opac, "opacity", this);
    fadeOut->setStartValue(opac->opacity());
    fadeOut->setEndValue(onSelected ? 0.99 : 0);
    fadeOut->setDuration(100);
    leave->addAnimation(shorter);
    leave->addAnimation(fadeOut);
    leave->start();

    if(mousePressed)
        mousePressed = false;
}

void selectionItem::mousePressEvent(QMouseEvent *event){
    bgWidget->setStyleSheet("border-radius:5px;background-color:#1a000000");
    QPropertyAnimation *shorter = new QPropertyAnimation(indicator, "geometry", this);
    shorter->setStartValue(indicator->geometry());
    shorter->setEndValue(QRectF(4, 0.4 * this->height(), 6, this->height() * 0.2));
    shorter->setDuration(100);
    shorter->setEasingCurve(QEasingCurve::OutBack);
    shorter->start();

    mousePressed = true;
}

void selectionItem::mouseReleaseEvent(QMouseEvent *event){
    if(mousePressed){
        bgWidget->setStyleSheet("border-radius:5px;background-color:#0a000000");
        QPropertyAnimation *longer = new QPropertyAnimation(indicator, "geometry", this);
        longer->setStartValue(indicator->geometry());
        longer->setEndValue(QRectF(4, 0.25 * this->height(), 6, this->height() * 0.5));
        longer->setDuration(150);
        longer->setEasingCurve(QEasingCurve::OutBack);
        longer->start();

        if(!onSelected){
            onSelected = true;
            title->setStyleSheet("color:#005FB8");
            description->setStyleSheet("color:#3a8fb7");
            emit selected(this);
            setFocus();
        }
        mousePressed = false;
    }
}

void selectionItem::resizeEvent(QResizeEvent *event){
    bgWidget->resize(this->size());
}

void selectionItem::Select(){
    if(!onSelected){
        onSelected = true;
        title->setStyleSheet("color:#005FB8");
        description->setStyleSheet("color:#3a8fb7");
        indicator->setGeometry(4, 0.5 * this->height(), 6, 0);

        QParallelAnimationGroup *sel = new QParallelAnimationGroup(this);
        QPropertyAnimation *longer = new QPropertyAnimation(indicator, "geometry", this);
        longer->setStartValue(indicator->geometry());
        longer->setEndValue(QRectF(4, 0.3 * this->height(), 6, this->height() * 0.4));
        longer->setDuration(150);
        longer->setEasingCurve(QEasingCurve::OutBack);
        QPropertyAnimation *fadeIn = new QPropertyAnimation(opac, "opacity", this);
        fadeIn->setStartValue(opac->opacity());
        fadeIn->setEndValue(0.99);
        fadeIn->setDuration(100);
        sel->addAnimation(longer);
        sel->addAnimation(fadeIn);
        sel->start();

        emit selected(this);
    }
}

void selectionItem::Deselect(){
    if(onSelected){
        onSelected = false;
        title->setStyleSheet("color:#2c2c2c");
        description->setStyleSheet("color:#707070");

        QPropertyAnimation *fadeOut = new QPropertyAnimation(opac, "opacity", this);
        fadeOut->setStartValue(opac->opacity());
        fadeOut->setEndValue(0);
        fadeOut->setDuration(100);
        fadeOut->start();
    }
}

singleSelectGroup::singleSelectGroup(QString name, QWidget *parent) :
    QWidget(parent){
    QFont titleFont = QFont("Corbel", 16);
    QFontMetrics fm(titleFont);
    qreal height = fm.lineSpacing();
    groupName = new QLabel(this);
    groupName->setMinimumHeight(height);
    groupName->setFont(titleFont);
    groupName->setText(name);

    QWidget *spacingLine = new QWidget(this);
    spacingLine->setFixedHeight(1);
    spacingLine->setStyleSheet("background-color:#0a000000");

    this->setFixedHeight(groupName->height() + middleSpacing + 1 + bottomSpacing);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 0, 10, bottomSpacing);
    mainLayout->setSpacing(middleSpacing);
    mainLayout->addWidget(groupName);
    mainLayout->addWidget(spacingLine);
}

void singleSelectGroup::AddItem(selectionItem *item){
    selections.push_back(item);
    this->setFixedHeight(this->height() + middleSpacing + item->height());
    mainLayout->addWidget(item);
    if(selectedID == -1){
        item->Select();
        selectedID = 0;
    }
    connect(item, SIGNAL(selected(selectionItem*)), this, SLOT(changeSelection(selectionItem*)));
    emit itemChange();
}

void singleSelectGroup::RemoveItem(selectionItem *item){
    int id = selections.indexOf(item);
    if(id < 0)  return;
    selections.erase(selections.begin() + id);
    mainLayout->removeWidget(item);
    item->setParent(nullptr);
    item->deleteLater();
    this->setFixedHeight(this->height() - middleSpacing - item->height());
    if(selections.size() == 0)
        selectedID = -1;
    else{
        selectedID = id < selections.size() ? id : id - 1;
        selections[selectedID]->Select();
    }
    emit selectedItemChange(selectedID);
    emit itemChange();
}

void singleSelectGroup::SetSelection(selectionItem *item){
    int id = selections.indexOf(item);
    selections[id]->Select();
}

void singleSelectGroup::changeSelection(selectionItem *item){
    int id = selections.indexOf(item);
    for(int i = 0; i < selections.size(); i++){
        if(i == id) continue;
        selections[i]->Deselect();
    }
    selectedID = id;
    emit selectedItemChange(id);
}

horizontalValueAdjuster::horizontalValueAdjuster(QString name, qreal min, qreal max, qreal step, QWidget *parent) :
    QWidget(parent),
    curValue(min),
    minValue(min),
    maxValue(max),
    stepValue(step)
{
    QFont titleFont = QFont("Corbel", 16);
    QFontMetrics fm(titleFont);
    qreal height = fm.lineSpacing();
    title = new QLabel(this);
    title->setMinimumHeight(height);
    title->setFont(titleFont);
    title->setText(name);

    QWidget *spacingLine = new QWidget(this);
    spacingLine->setFixedHeight(1);
    spacingLine->setStyleSheet("background-color:#0a000000");

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum((max - min) / step + 1);
    slider->setPageStep(1);
    QString grooveStyle = "QSlider::groove:horizontal{height:6px; border-radius:3px;} ";
    QString sliderStyle = "QSlider::handle:horizontal{width:12px; margin-bottom:-3px; margin-top:-3px; background:#c2c2c2; border-radius:6px;} ";
    QString sliderHStyle = "QSlider::handle:horizontal:hover{width:12px; margin-bottom:-3px; margin-top:-3px; background:#3a8fb7; border-radius:6px;} ";
    QString sliderPStyle = "QSlider::handle:horizontal:pressed{width:12px; margin-bottom:-3px; margin-top:-3px; background:#005fb8; border-radius:6px;} ";
    QString subStyle = "QSlider::sub-page:horizontal{background:#0078D4; border-radius:3px} ";
    QString addStyle = "QSlider::add-page:horizontal{background:#1a000000; border-radius:3px} ";
    slider->setStyleSheet(grooveStyle+sliderStyle+sliderHStyle+sliderPStyle+subStyle+addStyle);


    QFont valueFont = QFont("Corbel", 13);
    fm = QFontMetrics(titleFont);
    height = fm.lineSpacing();
    valueLabel = new QLabel(this);
    valueLabel->setMinimumHeight(height);
    valueLabel->setFont(valueFont);
    valueLabel->setText(QString::asprintf("%g", min));
    valueLabel->setMinimumWidth(30);
    valueLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    valueLabel->setStyleSheet("margin-bottom:5px");

    QWidget *content = new QWidget(this);
    content->setMinimumHeight(valueLabel->height() < slider->height() ? valueLabel->height() : slider->height());
    QHBoxLayout *contentLayout = new QHBoxLayout(content);
    contentLayout->setAlignment(Qt::AlignVCenter);
    content->setLayout(contentLayout);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(10);
    contentLayout->addWidget(valueLabel);
    contentLayout->addWidget(slider);

    this->setMinimumHeight(title->height() + 2 * middleSpacing + 1 + content->height() + bottomSpacing);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    this->setLayout(mainLayout);
    mainLayout->setContentsMargins(10, 0, 10, bottomSpacing);
    mainLayout->setSpacing(middleSpacing);
    mainLayout->addWidget(title);
    mainLayout->addWidget(spacingLine);
    mainLayout->addWidget(content);

    connect(slider, &QSlider::valueChanged, this, [=](qreal value){
        valueLabel->setText(QString::asprintf("%g", value * stepValue + minValue));
        curValue = value * stepValue + minValue;
        emit valueChanged(curValue);
    });
}

void horizontalValueAdjuster::setValue(qreal value){
    valueLabel->setText(QString::asprintf("%g", value));
    slider->setValue((value - minValue) / stepValue);
    curValue = value;
    emit valueChanged(value);
}

//********************

//itemGroup::itemGroup(const QString &name, QWidget *parent){
//    QFont titleFont = QFont("DengXian", 16, QFont::DemiBold);
//    QFontMetrics fm(titleFont);
//    qreal height = fm.lineSpacing();
//    groupName = new QLabel(this);
//    groupName->setFixedHeight(height);
//    groupName->setFont(titleFont);
//    groupName->setText(name);
//
//    QWidget *spacingLine = new QWidget(this);
//    spacingLine->setFixedHeight(1);
//    spacingLine->setStyleSheet("background-color:#0a000000");
//
//    this->setFixedHeight(groupName->height() + middleSpacing + 1 + bottomSpacing);
//
//    mainLayout = new QVBoxLayout(this);
//    mainLayout->setContentsMargins(10, 0, 10, bottomSpacing);
//    mainLayout->setSpacing(middleSpacing);
//    mainLayout->addWidget(groupName);
//    mainLayout->addWidget(spacingLine);
//}
//
//void itemGroup::AddItem(QWidget *item){
//    items.push_back(item);
//    this->setFixedHeight(this->height() + middleSpacing + item->height());
//    mainLayout->addWidget(item);
//}
//
//void itemGroup::RemoveItem(QWidget *item){
//    items.erase(items.begin() + items.indexOf(item));
//    mainLayout->removeWidget(item);
//    this->setFixedHeight(this->height() - middleSpacing - item->height());
//}


//*****************

bigIconButton::bigIconButton(int type, const QString &iconPath, const QString &description, int radius, QWidget *parent) :
    buttonType(type),
    QWidget(parent),
    cornerRadius(radius)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    /* set layout */
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    layout->setContentsMargins(0, 5, 0, 5);
    layout->setSpacing(6);
    layout->setAlignment(Qt::AlignCenter);
    if(type & ICON) {
        iconImg = new QPixmap(iconPath);
        icon = new QLabel(this);
        icon->setPixmap(*iconImg);
        icon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        icon->setAlignment(Qt::AlignCenter);
        icon->resize(iconImg->size());
        layout->addWidget(icon);
    }
    if(type & TEXT) {
        QFont textFont = QFont("Corbel", 10);
        QFontMetrics fm(textFont);
        text = new QLabel(this);
        text->setFont(textFont);
        text->setText(description);
        text->setWordWrap(true);
        text->setMinimumHeight(fm.lineSpacing());
        text->setAlignment(Qt::AlignCenter);
        layout->addWidget(text);
    }

    /* set background */
    bgWidget = new QWidget(this);
    bgWidget->resize(this->size());
    if(type & FRAMELESS)
        radiusStyle = QString::asprintf("border-radius:%dpx;border:transparent;", cornerRadius);
    else
        radiusStyle = QString::asprintf("border-radius:%dpx;border:1px solid gray;", cornerRadius);
    bgWidget->setStyleSheet(radiusStyle + "background-color:" + ((type & DISABLE) ? noColor : defaultColor));
    bgWidget->show();
}

void bigIconButton::resizeEvent(QResizeEvent *event){
    //qDebug()<<width()<<height();
    if(buttonType & ICON) {
        scale = 1.0 * (width() - margin) / iconImg->width();
        if(scale > 0)setScale(scale);
    }
    bgWidget->setFixedSize(this->size());
}

void bigIconButton::enterEvent(QEvent *event){
    bgWidget->setStyleSheet(radiusStyle + "background-color:" + ((buttonType & DISABLE) ? noColor : hoverColor));
}

void bigIconButton::leaveEvent(QEvent *event){
    bgWidget->setStyleSheet(radiusStyle + "background-color:" + ((buttonType & DISABLE) ? noColor : defaultColor));
}

void bigIconButton::mousePressEvent(QMouseEvent *event){
    bgWidget->setStyleSheet(radiusStyle + "background-color:" + ((buttonType & DISABLE) ? noColor : pressColor));
    mousePressed = true;
}

void bigIconButton::mouseReleaseEvent(QMouseEvent *event){
    if(mousePressed){
        bgWidget->setStyleSheet(radiusStyle + "background-color:#0a0078D4");
        mousePressed = false;
        emit clicked();
        if(selectable){
            emit selected();
            onSelected = true;
        }
    }
}

void bigIconButton::setScale(qreal scale){
    iconImg = new QPixmap(iconImg->scaled(iconImg->size() * scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    icon->setPixmap(*iconImg);
}

void bigIconButton::setPixmap(QString iconPath) {
    iconImg = new QPixmap(iconPath);
    iconImg = new QPixmap(iconImg->scaled(iconImg->size() * scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    icon->setPixmap(*iconImg);
}

textInputItem::textInputItem(const QString &name, QWidget *parent, int t) :
    QWidget(parent),
    type(t)
{
    if(!t)
        setStyleSheet("border:1px solid gray");
    else setStyleSheet("border:transparent");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont nameFont = QFont("Corbel", 12);
    QFontMetrics fm(nameFont);
    qreal height = fm.lineSpacing();
    itemName = new QLabel(this);
    itemName->setText(name);
    itemName->setFont(nameFont);
    itemName->setFixedHeight(height);
    itemName->setAlignment(Qt::AlignHCenter);//change
    itemName->setStyleSheet("border-style:none; background-color:#00000000;color:#1c1c1c");

    QFont textFont = QFont("Corbel", 12);
    fm = QFontMetrics(textFont);
    editor = new QLineEdit(this);
    editor->setText("");
    editor->setFixedHeight(fm.lineSpacing());
    editor->setStyleSheet("color:#5c5c5c;background-color:#00000000;border-style:none;");
    editor->setReadOnly(true);
    editor->setFont(textFont);

    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("border:transparent; background-color:#00000000; border-radius:5px;");
    bgWidget->lower();
    bgWidget->show();

    indicator = new QWidget(this);
    indicator->setFixedHeight(4);
    indicator->setStyleSheet("background-color:#0078d4;border-radius:2px");

    opac = new QGraphicsOpacityEffect(this);
    opac->setOpacity(0);
    indicator->setGraphicsEffect(opac);

    this->setFixedHeight(itemName->height() + 10);

    connect(editor, &QLineEdit::returnPressed, this, [=](){
        leaveEditEffect();
        onEditing = false;
        editor->setReadOnly(true);
        curText = editor->text();
    });
    connect(editor, &QLineEdit::editingFinished, this, [=](){
        leaveEditEffect();
        onEditing = false;
        editor->setReadOnly(true);
        curText = editor->text();
        QTimer *delay = new QTimer(this);
        connect(delay, &QTimer::timeout, this, [=](){mousePressed = false;});
        delay->setSingleShot(true);
        delay->start(10);
        mousePressed = false;
        emit textEdited(curText);
    });
}

void textInputItem::resizeEvent(QResizeEvent *event){
    itemName->move(margin, this->height() / 2 - itemName->height() / 2);
    nameWidth = std::max(QFontMetrics(itemName->font()).size(Qt::TextSingleLine, itemName->text()).width() + 3, (int)(width() * 0.3) - margin - spacing);
    itemName->setFixedWidth(nameWidth);
    int leftWidth = width() - nameWidth - 2 * margin - spacing;
    if(!onEditing) {
        int editorWidth = std::min(QFontMetrics(editor->font()).size(Qt::TextSingleLine, editor->text()).width() + 3, leftWidth);
        editor->resize(editorWidth, editor->height());
        editor->move(this->width() - margin - editor->width(), this->height() / 2 - editor->height() / 2);
        indicator->move(this->width() - margin, this->height() - 7);
    } else {
        editor->resize(leftWidth, editor->height());
        editor->move(this->width() - margin - editor->width(), this->height() / 2 - editor->height() / 2);
        indicator->move(margin + nameWidth + spacing, this->height() - 7);
    }
    bgWidget->setFixedSize(this->size());
}

void textInputItem::enterEditEffect(){
    editor->setCursorPosition(editor->text().length());
    editor->setStyleSheet("color:#1c1c1c;background-color:#00000000;border-style:none;");
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QPropertyAnimation *longer = new QPropertyAnimation(indicator, "geometry", this);
    longer->setStartValue(indicator->geometry());
    longer->setEndValue(QRectF(margin + nameWidth + spacing, this->height() - 7, width() - nameWidth - 2 * margin - spacing, 4));
    longer->setDuration(500);
    longer->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fade = new QPropertyAnimation(opac, "opacity", this);
    fade->setStartValue(opac->opacity());
    fade->setEndValue(0.99);
    fade->setDuration(150);
    QPropertyAnimation *move = new QPropertyAnimation(editor, "geometry", this);
    move->setStartValue(editor->geometry());
    move->setEndValue(QRectF(margin + nameWidth + spacing, this->height() / 2 - editor->height() / 2 - 2, width() - nameWidth - 2 * margin - spacing, editor->height()));
    move->setDuration(500);
    move->setEasingCurve(QEasingCurve::InOutExpo);
    group->addAnimation(longer);
    group->addAnimation(fade);
    group->addAnimation(move);
    group->start();
}

void textInputItem::leaveEditEffect(){
    editor->setCursorPosition(0);
    editor->setStyleSheet("color:#5c5c5c;background-color:#00000000;border-style:none;");
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    QPropertyAnimation *shorter = new QPropertyAnimation(indicator, "geometry", this);
    shorter->setStartValue(indicator->geometry());
    shorter->setEndValue(QRectF(this->width() - margin - 4, this->height() - 7, 4, 4));
    shorter->setDuration(500);
    shorter->setEasingCurve(QEasingCurve::InOutExpo);
    QPropertyAnimation *fade = new QPropertyAnimation(opac, "opacity", this);
    fade->setStartValue(opac->opacity());
    fade->setEndValue(0);
    fade->setDuration(350);
    QPropertyAnimation *move = new QPropertyAnimation(editor, "geometry", this);
    move->setStartValue(editor->geometry());
    int leftWidth = width() - nameWidth - 2 * margin - spacing;
    int width = std::min(QFontMetrics(editor->font()).size(Qt::TextSingleLine, editor->text()).width() + 3, leftWidth);
    move->setEndValue(QRectF(this->width() - width - margin, this->height() / 2 - editor->height() / 2, width, editor->height()));
    move->setDuration(500);
    move->setEasingCurve(QEasingCurve::InOutExpo);
    group->addAnimation(shorter);
    group->addAnimation(fade);
    group->addAnimation(move);
    group->start();
}
void textInputItem::enterEvent(QEvent *event){
    bgWidget->setStyleSheet("border:transparent; border-radius:5px;background-color:#0a000000");
}

void textInputItem::leaveEvent(QEvent *event){
    bgWidget->setStyleSheet("border:transparent; border-radius:5px;background-color:#00000000");
}

void textInputItem::mousePressEvent(QMouseEvent *event){
    bgWidget->setStyleSheet("border:transparent; border-radius:5px;background-color:#1a000000");
    mousePressed = true;
}

void textInputItem::mouseReleaseEvent(QMouseEvent *event){
    if(mousePressed){
        bgWidget->setStyleSheet("border:transparent; border-radius:5px;background-color:#0a000000");
        if(onEditing){
            leaveEditEffect();
            onEditing = false;
            curText = editor->text();
            editor->setReadOnly(true);
            emit textEdited(curText);
        }
        else{
            if(enabled){
                enterEditEffect();
                editor->raise();
                onEditing = true;
                editor->setReadOnly(false);
                editor->setText(curText + "");
                editor->setFocus();
            }
        }
        mousePressed = false;
    }
}

void textInputItem::setValue(const QString &text){
    editor->setText(text);
    editor->setCursorPosition(0);
    curText = text;
    int width = QFontMetrics(editor->font()).size(Qt::TextSingleLine, editor->text()).width() + 3;
    if(!onEditing){
        if(width > this->width() * 0.7 - margin)
            editor->resize(this->width() * 0.7 - margin, editor->height());
        else
            editor->resize(width, editor->height());
        editor->move(this->width() - margin - editor->width(), this->height() / 2 - editor->height() / 2);
    }
    else{
        editor->resize(this->width() * 0.7 - margin, editor->height());
        editor->move(this->width() * 0.3, this->height() / 2 - editor->height() / 2 - 2);
    }
}


textButton::textButton(QString text, QWidget *parent, qreal ratio) :
    QWidget(parent)
{
    QFont textFont = QFont("Corbel", 10);
    QFontMetrics fm(textFont);
    qreal height = fm.lineSpacing();
    btnText = new QLabel(this);
    btnText->setText(text);
    btnText->setFont(textFont);
    btnText->setFixedHeight(height);
    btnText->setFixedWidth(fm.size(Qt::TextSingleLine, text).width() + 2);
    btnText->setStyleSheet("color:#1c1c1c");
    btnText->setAlignment(Qt::AlignCenter);

    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("background-color:"+defaultColor+";border-radius:5px;");

    this->setFixedHeight(btnText->height() / ratio);
}

textButton::textButton(QString text, QString defC, QString hoverC, QString pressedC, QWidget *parent, qreal ratio):
    QWidget(parent),
    defaultColor(defC),
    hoverColor(hoverC),
    pressedColor(pressedC)
{
    QFont textFont = QFont("Corbel", 10);
    QFontMetrics fm(textFont);
    qreal height = fm.lineSpacing();
    btnText = new QLabel(this);
    btnText->setText(text);
    btnText->setFont(textFont);
    btnText->setFixedHeight(height);
    btnText->setFixedWidth(fm.size(Qt::TextSingleLine, text).width() + 2);
    btnText->setStyleSheet("color:#1c1c1c");
    btnText->setAlignment(Qt::AlignCenter);

    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("background-color:"+defaultColor+";border-radius:5px;");

    this->setFixedHeight(btnText->height() / ratio);
}

void textButton::resizeEvent(QResizeEvent *event){
    bgWidget->resize(this->size());
    btnText->move(this->width() / 2 - btnText->width() / 2, this->height() / 2 - btnText->height() / 2);
}

void textButton::enterEvent(QEvent *event){
    bgWidget->setStyleSheet("background-color:"+hoverColor+";border-radius:5px;");
}

void textButton::leaveEvent(QEvent *event){
    bgWidget->setStyleSheet("background-color:"+defaultColor+";border-radius:5px;");
    if(mousePressed){
        bgWidget->setStyleSheet("background-color:"+pressedColor+";border-radius:5px;");
        QPropertyAnimation *enlarge = new QPropertyAnimation(bgWidget, "geometry", this);
        enlarge->setStartValue(bgWidget->geometry());
        enlarge->setEndValue(QRect(0, 0, this->width(), this->height()));
        enlarge->setDuration(150);
        enlarge->setEasingCurve(QEasingCurve::OutBounce);
        enlarge->start();
        mousePressed = false;
    }
}

void textButton::mousePressEvent(QMouseEvent *event){
    bgWidget->setStyleSheet("background-color:"+pressedColor+";border-radius:5px;");
    QPropertyAnimation *shrink = new QPropertyAnimation(bgWidget, "geometry", this);
    shrink->setStartValue(bgWidget->geometry());
    shrink->setEndValue(QRect(0.05 * this->width(), 0.05 * this->height(), this->width() * 0.9, this->height() * 0.9));
    shrink->setDuration(100);
    shrink->setEasingCurve(QEasingCurve::OutBack);
    shrink->start();
    mousePressed = true;
    setFocus();
}

void textButton::mouseReleaseEvent(QMouseEvent *event){
    if(mousePressed){
        bgWidget->setStyleSheet("background-color:"+hoverColor+";border-radius:5px;");
        QPropertyAnimation *enlarge = new QPropertyAnimation(bgWidget, "geometry", this);
        enlarge->setStartValue(bgWidget->geometry());
        enlarge->setEndValue(QRect(0, 0, this->width(), this->height()));
        enlarge->setDuration(150);
        enlarge->setEasingCurve(QEasingCurve::OutBounce);
        enlarge->start();
        mousePressed = false;
        emit clicked();
    }
}
singleSelectGroupVertical::singleSelectGroupVertical(QString name, QWidget *parent) :
    QWidget(parent){
    QFont titleFont = QFont("Corbel", 16);
    QFontMetrics fm(titleFont);
    qreal height = fm.lineSpacing();
    groupName = new QLabel(this);
    groupName->setMinimumHeight(height);
    groupName->setFont(titleFont);
    groupName->setText(name);

    this->setFixedWidth(groupName->width() + middleSpacing + 1);

    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 0, 10, 0);
    mainLayout->setSpacing(middleSpacing);
    mainLayout->addWidget(groupName);
}

void singleSelectGroupVertical::AddItem(selectionItem *item){
    selections.push_back(item);
    this->setFixedWidth(this->width() + middleSpacing + item->width());
    mainLayout->addWidget(item);
    if(selectedID == -1){
        item->Select();
        selectedID = 0;
    }
    connect(item, SIGNAL(selected(selectionItem*)), this, SLOT(changeSelection(selectionItem*)));
    emit itemChange();
}

void singleSelectGroupVertical::RemoveItem(selectionItem *item){
    int id = selections.indexOf(item);
    if(id < 0)  return;
    selections.erase(selections.begin() + id);
    mainLayout->removeWidget(item);
    item->setParent(nullptr);
    item->deleteLater();
    this->setFixedWidth(this->width() - middleSpacing - item->width());
    if(selections.size() == 0)
        selectedID = -1;
    else{
        selectedID = id < selections.size() ? id : id - 1;
        selections[selectedID]->Select();
    }
    emit selectedItemChange(selectedID);
    emit itemChange();
}

void singleSelectGroupVertical::SetSelection(selectionItem *item){
    int id = selections.indexOf(item);
    selections[id]->Select();
}

void singleSelectGroupVertical::changeSelection(selectionItem *item){
    int id = selections.indexOf(item);
    for(int i = 0; i < selections.size(); i++){
        if(i == id) continue;
        selections[i]->Deselect();
    }
    selectedID = id;
    emit selectedItemChange(id);
}

topButton::topButton(QWidget* parent) :
    QPushButton(parent)
{
    QString buttonStyle = "QPushButton{background-color:#c2c2c2;border-radius:6px}\nQPushButton:hover {background-color:#227d51;border-radius:6px;}\nQPushButton:pressed {background-color:#2d6d4b;border-radius:6px;}";
    setStyleSheet(buttonStyle);
    resize(12, 12);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMouseTracking(true);
    show();
}

bool topButton::event(QEvent* e) {
    raise();
    return QPushButton::event(e);
}

QLineDelegate::QLineDelegate(QTableView* tableView)
{
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItem());
    QColor gridColor = static_cast<QRgb>(gridHint);
    pen = QPen(gridColor, 0, tableView->gridStyle());
    view = tableView;
}

void QLineDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,const QModelIndex& index)const
{
    QStyledItemDelegate::paint(painter, option, index);
    QPen oldPen = painter->pen();
    painter->setPen(pen);

    //draw verticalLine
    //painter->drawLine(option.rect.topRight(), option.rect.bottomRight());

    //draw horizontalLine
    //painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    //above code, line have breakpoint, the following code can solve it well

    QPoint p1 = QPoint(option.rect.bottomLeft().x()-1,option.rect.bottomLeft().y());
    QPoint p2 = QPoint(option.rect.bottomRight().x()+1,option.rect.bottomRight().y());
    painter->drawLine(p1, p2);
    painter->setPen(oldPen);
}

ClockTable::ClockTable(QWidget* parent) :
    QTableWidget(parent)
{
    setColumnCount(5);
    setRowCount(5);
    setFocusPolicy(Qt::NoFocus);
    setAlternatingRowColors(true);
    setStyleSheet("border-radius:0px;"
        "background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(151, 169, 255, 100), stop:1 rgba(151, 169, 255, 150));"
        "alternate-background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(151, 169, 255, 250), stop:1 rgba(151, 169, 255, 200));"
        "selection-background-color:gray");
    setShowGrid(false);
    //horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setDefaultSectionSize(100);
    //horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setStyleSheet("QHeaderView::section{"
        "background-color:qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(151, 169, 255, 0), stop:1 rgba(151, 169, 255, 100));"
        "color: black;"
        //"border-radius:12px"
        "border-top:0px solid #D8D8D8;"
        "border-left:0px solid #D8D8D8;"
        "border-right:0px solid #D8D8D8;"
        "border-bottom:1px solid #D8D8D8;"
        "padding:4px;"
    "}");
    verticalHeader()->setVisible(false);

    horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
        "QScrollBar::handle:hover{background:gray;}"
        "QScrollBar::sub-line{background:transparent;}"
        "QScrollBar::sub-page{background:transparent;}"
        "QScrollBar::add-line{background:transparent;}"
        "QScrollBar::add-page{background:transparent;}");
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setItemDelegate(new QLineDelegate(this));
    QStringList header;
    header<<QString("时间")<<QString("仅一次")<<QString("循环");
    setHorizontalHeaderLabels(header);
    //setStyleSheet("selection-background-color:pink;border-radius:0px");
    for(int i = 0; i < 5; i++) setRowHeight(i, 30), Height += 30;
    for(int i = 0; i < 5; i++) setColumnWidth(i, 50), Width += 50;
    setItem(0,0,new QTableWidgetItem("Tom"));
    //qDebug()<<table->width()<<table->height();
    resize(Width, Height);
}

foldWidget::foldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* parent) :
    QWidget(parent),
    maxHeight(h),
    extraIcons(icons),
    leftIcon(new bigIconButton(9, ":/icons/icons/arrow-left.svg")),
    downIcon(new bigIconButton(9, ":/icons/icons/arrow-down.svg"))
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(titleHeight);
    titleWidget = new QWidget(this);
    titleWidget->setStyleSheet("background-color:rgb(200, 200, 200); border-radius:0px;");
        QFont textFont = QFont("Corbel Light", 24);
        QFontMetrics fm(textFont);
        nameLabel = new QLabel(name, titleWidget);
        textFont.setStyleStrategy(QFont::PreferAntialias);
        nameLabel->setFont(textFont);
        nameLabel->setFixedHeight(fm.lineSpacing());
        leftIcon->setParent(titleWidget);
        leftIcon->setFixedSize(titleHeight - 10, titleHeight - 10);
        connect(leftIcon, &bigIconButton::clicked, this, [=] {
           leftIcon->hide();
           downIcon->show();
           foldChange();
        });
        downIcon->setParent(titleWidget);
        downIcon->setFixedSize(titleHeight - 10, titleHeight - 10);
        connect(downIcon, &bigIconButton::clicked, this, [=] {
           downIcon->hide();
           leftIcon->show();
           foldChange();
        });
        for(int i = 0; i < extraIcons.size(); i++) {
            extraIcons[i]->setParent(titleWidget);
            extraIcons[i]->setFixedSize(titleHeight - 10, titleHeight - 10);
            connect(extraIcons[i], &bigIconButton::clicked, this, [=] {emit clicked(i);});
        }
    container = new ScrollAreaCustom(false, this);
    container->lower();
}
void foldWidget::resizeEvent(QResizeEvent*) {
    titleWidget->resize(width(), titleHeight);
    titleWidget->move(0, 0);
        nameLabel->move(margin, titleHeight / 2 - nameLabel->height() / 2);
        leftIcon->move(width() - margin - leftIcon->width(), titleHeight / 2 - leftIcon->height() / 2);
        downIcon->move(width() - margin - downIcon->width(), titleHeight / 2 - downIcon->height() / 2);
    if(fold) {
        leftIcon->show();
        downIcon->hide();
    }
    else {
        leftIcon->hide();
        downIcon->show();
    }
    int prex = width() - margin - leftIcon->width() - spacing;
    for(int i = extraIcons.size() - 1; i >= 0; i--) {
        extraIcons[i]->move(prex - extraIcons[i]->width(), titleHeight / 2 - extraIcons[i]->height() / 2);
        prex -= extraIcons[i]->width() + spacing;
    }
    container->resize(width(), height() - titleHeight + overlap);
    container->move(0, titleHeight - overlap);
}
void foldWidget::foldChange() {
    QPropertyAnimation* animation = new QPropertyAnimation(this, ""); //qt动画类
    animation->setDuration(500);
    if(fold) {
        animation->setStartValue(height());//动画开始值和结束值
        animation->setEndValue(maxHeight - titleHeight + overlap);
        connect(animation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value){
            this->setFixedHeight(value.toInt());
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        fold = false;
    } else {
        animation->setStartValue(height());
        animation->setEndValue(titleHeight);
        connect(animation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value){
            this->setFixedHeight(value.toInt());
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        fold = true;
    }
}

ComboBox::ComboBox(QWidget* parent) :
    QComboBox(parent)
{
    QFile qss(":/style/combostyle.qss");
    qss.open(QFile::ReadOnly);
    QString styleSheet = "QScrollBar{background:transparent; height:10px;}"
            "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
            "QScrollBar::handle:hover{background:gray;}"
            "QScrollBar::sub-line{background:transparent;}"
            "QScrollBar::sub-page{background:transparent;}"
            "QScrollBar::add-line{background:transparent;}"
            "QScrollBar::add-page{background:transparent;}" +
            QLatin1String(qss.readAll());
    setStyleSheet(styleSheet);
    qss.close();
}

void ComboBox::setNumberRange(int l, int r) {
    for(int i = l; i < r; i++) {
        addItem(QString::asprintf("%d", i));
    }
}

customWidget::customWidget(const QString &name, QWidget* content, QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QWidget::{}");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont nameFont = QFont("Corbel", 12);
    QFontMetrics fm(nameFont);
    qreal height = fm.lineSpacing();
    itemName = new QLabel(this);
    itemName->setText(name);
    itemName->setFont(nameFont);
    itemName->setFixedHeight(height);
    itemName->setAlignment(Qt::AlignHCenter);//change
    itemName->setStyleSheet("border-style:none; background-color:#00000000;color:#1c1c1c");

    contentWidget = new QWidget(this);
    QHBoxLayout* contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        content->setParent(contentWidget);
    contentLayout->addWidget(content);
    contentWidget->setFixedHeight(content->height());
    contentWidget->setStyleSheet("background-color:transparent;");

    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("border:transparent;background-color:#00000000;border-radius:5px;");
    bgWidget->lower();
    bgWidget->show();

    this->setFixedHeight(std::max(itemName->height() + 10, content->height() + 10));
}

void customWidget::resizeEvent(QResizeEvent *event){
    itemName->move(margin, this->height() / 2 - itemName->height() / 2);
    int nameWidth = std::max(QFontMetrics(itemName->font()).size(Qt::TextSingleLine, itemName->text()).width() + 3, (int)(width() * 0.3) - margin - spacing);
    itemName->setFixedWidth(std::max(nameWidth, 0));
    int leftWidth = width() - nameWidth - 2 * margin - spacing;
    contentWidget->setFixedWidth(std::max(leftWidth, 0));
    contentWidget->move(this->width() - margin - contentWidget->width(), this->height() / 2 - contentWidget->height() / 2);
    bgWidget->setFixedSize(this->size());
}

void customWidget::enterEvent(QEvent *event){
    bgWidget->setStyleSheet("border:transparent;border-radius:5px;background-color:#0a000000");
}

void customWidget::leaveEvent(QEvent *event){
    bgWidget->setStyleSheet("border:transparent;border-radius:5px;background-color:#00000000");
}
