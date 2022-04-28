QT += core gui svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    activitypage.cpp \
    basicClass.cpp \
    clock.cpp \
    connect.cpp \
    customScrollContainer.cpp \
    customWidgets.cpp \
    classpage.cpp \
    graph_implement.cpp \
    graph_view.cpp \
    guidepage.cpp \
    loginpage.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    mycanvas.cpp \
    slidepage.cpp

HEADERS += \
    activitypage.h \
    basicClass.h \
    classpage.h \
    clock.h \
    connect.h \
    customScrollContainer.h \
    customWidgets.h \
    graph_implement.h \
    graph_view.h \
    guidepage.h \
    loginpage.h \
    mainpage.h \
    mainwindow.h \
    mycanvas.h \
    slidepage.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resourse.qrc
