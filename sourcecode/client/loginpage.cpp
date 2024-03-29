#include "loginpage.h"
#include "global.h"

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
            textItem* logo = new textItem("教 学 云 平 台", "华文行楷", 30, logoWidget);
            logoLayout->addWidget(logo);
        loginLayout->addWidget(logoWidget);

        QWidget* checkWidget = new QWidget(this);
        checkWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        checkWidget->setStyleSheet("background:transparent;");
        QVBoxLayout* checkLayout = new QVBoxLayout(checkWidget);
        checkWidget->setLayout(checkLayout);
        checkLayout->setContentsMargins(50, 50, 50, 50);
        checkLayout->setSpacing(5);
        checkLayout->setAlignment(Qt::AlignCenter);
            singleSelectGroupVertical* identitySel = new singleSelectGroupVertical("请选择身份", checkWidget);
                selectionItemVertical* student = new selectionItemVertical("我是学生", checkWidget);
                selectionItemVertical* teacher = new selectionItemVertical("我是老师", checkWidget);
                identitySel->AddItem(student);
                identitySel->AddItem(teacher);
            textInputItem* username = new textInputItem("用户名", checkWidget);
            textInputItem* password = new textInputItem("密码", checkWidget);
            textButton* login = new textButton("登录", checkWidget);
            connect(login, &textButton::clicked, [=]{
                //for debugging
                //emit logined();
                LoginQuery* loginQuery = new LoginQuery(identitySel->value(), username->value(), password->value());
                connect(loginQuery, &LoginQuery::Id, this, [=](int id) {
                   if(id == 255) {
                   } else {
                       if(identitySel->value() == 0) {
                           type = 0;
                           studentId = id;
                       } else {
                           type = 1;
                           teacherId = id;
                       }
                       emit logined();
                       username->setValue("");
                       password->setValue("");
                   }
                });
            });
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
