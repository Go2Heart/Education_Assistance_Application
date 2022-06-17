#include "specifiedWidgets.h"
#include "basicClass.h"
#include "graph_view.h"

timeWidget::timeWidget(QWidget* parent) :
    QWidget(parent)
{
    setStyleSheet("background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(3);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    for(int i = 0; i <= 1; i++) {
        ComboBox* box = new ComboBox(this);
        box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        box->setFixedSize(45, 25);
        box->addItem("");
        boxs.push_back(box);
    }
    textItem* split = new textItem(":", "Corbel Light", 13, this);
    boxs[0]->setNumberRange(0, 24);
    boxs[1]->setNumberRange(0, 60);
    layout->addWidget(boxs[0]);
    layout->addWidget(split);
    layout->addWidget(boxs[1]);
    setFixedSize(150, 30);
}

Timer timeWidget::GetTimer() const {
    return Timer(atoi(boxs[0]->currentText().toLatin1().data()), atoi(boxs[1]->currentText().toLatin1().data()));
}

StudentSmallWidget::StudentSmallWidget(Student student, QWidget* parent) :
    student(student), QWidget(parent)
{
    bgWidget = new QWidget(this);
    bgWidget->lower();
    bgWidget->show();
    bgWidget->setStyleSheet("background-color:#22ffa000;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    infoWidget = new QWidget(this);
    infoWidget->setStyleSheet("background-color:transparent;");
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    infoLayout->setContentsMargins(0, 0, 0, 0);
    infoLayout->setSpacing(5);
        name = new textItem(student.name, "Corbel Light", 13, infoWidget);
        QWidget* space = new QWidget(infoWidget);
        space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        space->setFixedHeight(10);
        id = new textItem("[学号]" + student.studentNumber, "Corbel Light", 10, infoWidget, "color: black");
        infoLayout->addWidget(name);
        infoLayout->addWidget(space);
        infoLayout->addWidget(id);
    setFixedHeight(100);
}

void StudentSmallWidget::mousePressEvent(QMouseEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#66ffa000;");
    }
    mousePressed = true;
}

void StudentSmallWidget::mouseReleaseEvent(QMouseEvent*) {
    mousePressed = false;
    emit clicked();
}

void StudentSmallWidget::enterEvent(QEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#44ffa000;");
    }
}

void StudentSmallWidget::leaveEvent(QEvent*) {
    if(!selected) {
        bgWidget->setStyleSheet("background-color:#22ffa000;");
    }
}

void StudentSmallWidget::resizeEvent(QResizeEvent*) {
    infoWidget->resize(this->width() - 2 * margin, this->height() - 2 * margin);
    infoWidget->move(margin, margin);
    bgWidget->resize(this->size());
}

bool StudentSmallWidget::Selected() {
    if(selected) {
        bgWidget->setStyleSheet("background-color:#22ffa000;");
        selected = false;
    } else {
        selected = true;
        bgWidget->setStyleSheet("background-color:#66ffa000;");
    }
    return selected;
}

StudentSelectWidget::StudentSelectWidget(QWidget* parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(10);
        container = new ScrollAreaCustom(false, this);
        container->setFixedWidth(350);
        layout->addWidget(container);
}

void StudentSelectWidget::reloadInfo() {
    selectedWidgets.clear();
    container->clear();
    StudentGroupQuery* nowQuery = new StudentGroupQuery();
    connect(nowQuery, &StudentGroupQuery::receive, this, [=](QVariant varValue) {
        QVector<Student> result = varValue.value<QVector<Student>>();
        for(int i = 0; i < result.size(); i++) {
            StudentSmallWidget* widget = new StudentSmallWidget(result[i], this);
            connect(widget, &StudentSmallWidget::clicked, widget, [=] {
                if(widget->Selected()) {
                    selectedWidgets.push_back(widget);
                } else {
                    selectedWidgets.remove(selectedWidgets.indexOf(widget));
                }
            });
            container->addWidget(widget, true);
        }
    });
}
QVector<Student> StudentSelectWidget::GetStudents() {
    QVector<Student> result;
    for(int i = 0; i < selectedWidgets.size(); i++) result.push_back(selectedWidgets[i]->student);
    return result;
}

CellWidget::CellWidget(QWidget* w) :
    QWidget()
{
    w->setParent(this);
    setStyleSheet("border:none; background-color:transparent;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(w);
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
    setColumnCount(8);
    setRowCount(14);
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

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
        "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
        "QScrollBar::handle:hover{background:gray;}"
        "QScrollBar::sub-line{background:transparent;}"
        "QScrollBar::sub-page{background:transparent;}"
        "QScrollBar::add-line{background:transparent;}"
        "QScrollBar::add-page{background:transparent;}");
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    //setItemDelegate(new QLineDelegate(this));
    QStringList header;
    header<<QString("节次")<<QString("周一")<<QString("周二")<<QString("周三")<<QString("周四")<<QString("周五")<<QString("周六")<<QString("周日");
    setHorizontalHeaderLabels(header);
    //setStyleSheet("selection-background-color:pink;border-radius:0px");
    for(int i = 0; i < 14; i++) setRowHeight(i, 100), Height += 100;
    for(int i = 0; i < 8; i++) setColumnWidth(i, 140), Width += 140;
    for(int i = 0; i < 14; i++) {
        setCellWidget(i, 0, new CellWidget(new viewLog(QString::asprintf("第 %d 节", i + 1))));
    }
    resize(Width, Height);
}

void ClockTable::LoadData() {
    for(int i = 0; i < 14; i++) {
        for(int j = 1; j < 8; j++) {
            TableQuery* nowQuery = new TableQuery(j, i + 1);
            qDebug() << "QUERY" << j << i;
            connect(nowQuery, &TableQuery::receive, this, [=](QString message) {
                setCellWidget(i, j, new CellWidget(new viewLog(message, 5)));
            });
        }
    }
}
