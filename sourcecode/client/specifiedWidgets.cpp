#include "specifiedWidgets.h"

clockAddPage::clockAddPage(int radius, int type, int width, int height, QString name, QWidget* parent, int posy) :
    SlidePage(radius, type, width, height, name, parent, posy)
{
    description = new textInputItem("描述：", this);
    place = new textInputItem("地点：", this);
    time = new textInputItem("时间：", this);
    clockBar = new QWidget(this);
    QHBoxLayout* clockLayout = new QHBoxLayout(clockBar);
    clockLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    clockLayout->setSpacing(10);
    clockLayout->setContentsMargins(0, 0, 0, 0);
        bigIconButton* alarmOn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", 0, clockBar);
        alarmOn->setFixedSize(30, 30);
        clockLayout->addWidget(alarmOn);
        bigIconButton* alarmOff = new bigIconButton(1, ":/icons/icons/alarm_off.svg", "", 0, clockBar);
        alarmOff->setFixedSize(30, 30);
        frequency = new textInputItem("频率：", clockBar);
        clockLayout->addWidget(frequency);

        connect(alarmOn, &bigIconButton::clicked, this, [=] {
            clockLayout->removeWidget(alarmOn);
            alarmOn->hide();
            clockLayout->insertWidget(0, alarmOff);
            alarmOff->show();
            alarm = false;
        });
        connect(alarmOff, &bigIconButton::clicked, this, [=] {
            clockLayout->removeWidget(alarmOff);
            alarmOff->hide();
            clockLayout->insertWidget(0, alarmOn);
            alarmOn->show();
            alarm = true;
        });

    textButton* createBtn = new textButton("Create!", this);
    connect(createBtn, &textButton::clicked, this, [=] {
        slideOut();
        createBtn->setText("Modify!");
        if(created)
            emit modify(collectMsg());
        else {
            created = true;
            emit deliver(collectMsg());
        }
    });
    AddContent(createBtn);
    AddContent(clockBar);
    AddContent(time);
    AddContent(place);
    AddContent(description);
}

QVector<QString> clockAddPage::collectMsg() {
    QVector<QString> tmp;
    tmp.push_back(description->value());
    tmp.push_back(place->value());
    tmp.push_back(time->value());
    tmp.push_back(alarm ? "true" : "alse");
    tmp.push_back(frequency->value());
    return tmp;
}

clockInfoWidget::clockInfoWidget(QVector<QString> info, QWidget* parent) :
    QWidget(parent),
    alerm(new bigIconButton(13, info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg", "", 0, this))
{
    setStyleSheet("background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    infoWidget = new QWidget(this);
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(15);
        QFont descFont = QFont("Corbel Light", 13);
        QFontMetrics descm(descFont);
        descFont.setStyleStrategy(QFont::PreferAntialias);
        descLabel = new QLabel("#内容#" + info[0], infoWidget);
        descLabel->setFont(descFont);
        descLabel->setFixedHeight(descm.lineSpacing());
        descLabel->setStyleSheet("color: black");
        infoLayout->addWidget(descLabel);

        QFont detailFont = QFont("Corbel Light", 10);
        QFontMetrics detailm(detailFont);
        detailFont.setStyleStrategy(QFont::PreferAntialias);
        detailLabel = new QLabel("#地点#" + info[1] + "       #时间#" + info[2], infoWidget);
        detailLabel->setFont(detailFont);
        detailLabel->setFixedHeight(detailm.lineSpacing());
        detailLabel->setStyleSheet("color: gray");
        infoLayout->addWidget(detailLabel);
    alerm->setFixedSize(30, 30);
}

void clockInfoWidget::mousePressEvent(QMouseEvent *) {
    mousePressed = true;
}

void clockInfoWidget::mouseReleaseEvent(QMouseEvent *) {
    mousePressed = false;
    emit clicked();
}

void clockInfoWidget::resizeEvent(QResizeEvent*) {
    infoWidget->resize(this->width() - alerm->width() - 2 * margin - spacing, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    alerm->move(this->width() - alerm->width() - margin, this->height() / 2 - alerm->height() / 2);
}

void clockInfoWidget::modify(QVector<QString> info) {
    descLabel->setText("#内容#" + info[0]);
    detailLabel->setText("#地点#" + info[1] + "       #时间#" + info[2]);
    alerm->setPixmap(info[3] == "true" ? ":/icons/icons/alarm_on.svg" : ":/icons/icons/alarm_off.svg");
}

clockWidget::clockWidget(QVector<QString> info, QWidget* parent) :
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
    infoWidget = new clockInfoWidget(info, this);
    layout->addWidget(infoWidget);
    connect(infoWidget, &clockInfoWidget::clicked, this, [=] {emit clicked();});
}

void clockWidget::resizeEvent(QResizeEvent *) {
    bgWidget->resize(this->size());
}

clockfoldWidget::clockfoldWidget(QString name, int h, QVector<bigIconButton*> icons, QWidget* p, QWidget* parent) :
    foldWidget(name, h, icons, parent),
    slideParent(p)
{
    connect(icons[0], &bigIconButton::clicked, this, [=] {
        clockAddPage* newPage = new clockAddPage(12, 1, 300, 0, "创建新待办", slideParent);
        emit addPage(newPage);
        connect(newPage, &clockAddPage::deliver, this, [=](QVector<QString> s) {
            clockWidget* newWidget = new clockWidget(s, this);
            addContent(newWidget);
            connect(newWidget, &clockWidget::clicked, newPage, &SlidePage::slideIn);
            connect(newPage, &clockAddPage::modify, newWidget, [=](QVector<QString> s) {
                newWidget->modify(s);
            });
            pageList.push_back(newPage);
        });
        newPage->slideIn();
    });
}
