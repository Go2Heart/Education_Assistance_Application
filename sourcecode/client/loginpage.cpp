#include "loginpage.h"

loginPage::loginPage(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* loginLayout = new QHBoxLayout(this);
    setLayout(loginLayout);
    loginLayout->setContentsMargins(5, 5, 5, 5);
    loginLayout->setSpacing(0);
    loginLayout->setAlignment(Qt::AlignCenter);
        QWidget* logoWidget = new QWidget(this);
        logoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        logoWidget->setStyleSheet("background:transparent;");
        QVBoxLayout* logoLayout = new QVBoxLayout(logoWidget);
        logoWidget->setLayout(logoLayout);
        logoLayout->setContentsMargins(0, 0, 0, 0);
        logoLayout->setSpacing(0);
        logoLayout->setAlignment(Qt::AlignCenter);
        //这里加入logo,使用qpixmap
        //logoLayout->addItem
        loginLayout->addWidget(logoWidget);

        QWidget* checkWidget = new QWidget(this);
        checkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        checkWidget->setStyleSheet("background:transparent;");
        QVBoxLayout* checkLayout = new QVBoxLayout(checkWidget);
        checkWidget->setLayout(checkLayout);
        checkLayout->setContentsMargins(0, 50, 0, 50);
        checkLayout->setSpacing(5);
        checkLayout->setAlignment(Qt::AlignCenter);
            singleSelectGroupVertical* identitySel = new singleSelectGroupVertical("请选择身份", checkWidget);
                selectionItem* student = new selectionItem("我是学生", "", checkWidget);
                selectionItem* teacher = new selectionItem("我是老师", "", checkWidget);
                identitySel->AddItem(student);
                identitySel->AddItem(teacher);
            textInputItem* username = new textInputItem("用户名", checkWidget);
            textInputItem* password = new textInputItem("密码", checkWidget);
            textButton* login = new textButton("登录", checkWidget);
            connect(login, &textButton::clicked, [=]{emit logined();});
            checkLayout->addWidget(identitySel);
            checkLayout->addWidget(username);
            checkLayout->addWidget(password);
            checkLayout->addWidget(login);
        loginLayout->addWidget(checkWidget);
}

void loginPage::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(splitColor);
    painter.drawEllipse(width() / 2 - splitWidth / 2, margin / 2, splitWidth, height() - margin);
}
