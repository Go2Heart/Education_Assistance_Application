#include "specifiedWidgets.h"

clockAddPage::clockAddPage(int radius, int type, int width, int height, QString name, QWidget* parent, int posy) :
    SlidePage(radius, type, width, height, name, parent, posy)
{
    description = new textInputItem("描述：", this);
    place = new textInputItem("地点：", this);
    timeBar = new QWidget(this);
    timeBar->setStyleSheet("background-color:transparent;");
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* timeLayout = new QHBoxLayout(timeBar);
    timeLayout->setContentsMargins(0, 0, 0, 0);
    timeLayout->setSpacing(3);
    timeLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        hour = new ComboBox(timeBar);
        hour->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        hour->setFixedSize(45, 25);
        hour->setNumberRange(0, 24);
        timeLayout->addWidget(hour);
        QFont font = QFont("Corbel Light", 13);
        QFontMetrics fm(font);
        font.setStyleStrategy(QFont::PreferAntialias);
        QLabel* split = new QLabel(":", timeBar);
        split->setFont(font);
        split->setFixedHeight(fm.lineSpacing());
        split->setStyleSheet("color: black");
        timeLayout->addWidget(split);
        minute = new ComboBox(timeBar);
        minute->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        minute->setFixedSize(45, 25);
        minute->setNumberRange(0, 60);
        timeLayout->addWidget(minute);
    timeBar->setFixedHeight(30);
    customWidget* timeWidget = new customWidget("时间：", timeBar, this);

    clockBar = new QWidget(this);
    QHBoxLayout* clockLayout = new QHBoxLayout(clockBar);
    clockLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    clockLayout->setSpacing(10);
    clockLayout->setContentsMargins(0, 0, 0, 0);
        bigIconButton* alarmBtn = new bigIconButton(1, ":/icons/icons/alarm_on.svg", "", 0, clockBar);
        alarmBtn->setFixedSize(30, 30);
        clockLayout->addWidget(alarmBtn);
        frequency = new ComboBox(clockBar);
        frequency->addItem("仅一次");
        frequency->addItem("每周");
        frequency->addItem("每月");
        frequency->setFixedSize(100, 25);
        customWidget* freWidget = new customWidget("频率：", frequency, this);
        clockLayout->addWidget(freWidget);
    clockBar->setFixedHeight(40);

    connect(alarmBtn, &bigIconButton::clicked, this, [=] {
        if(alarm){
            alarmBtn->setPixmap(":/icons/icons/alarm_off.svg");
            alarm = false;
        } else {
            alarmBtn->setPixmap(":/icons/icons/alarm_on.svg");
            alarm = true;
        }
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
    AddContent(timeWidget);
    AddContent(place);
    AddContent(description);
}

QVector<QString> clockAddPage::collectMsg() {
    QVector<QString> tmp;
    tmp.push_back(description->value());
    tmp.push_back(place->value());
    tmp.push_back(hour->currentText() + ":" + minute->currentText());
    tmp.push_back(alarm ? "true" : "alse");
    tmp.push_back(frequency->currentText());
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
    etSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
