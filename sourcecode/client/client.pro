QT       += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basicClass.cpp \
    clock.cpp \
    customScrollContainer.cpp \
    customWidgets.cpp \
    loginpage.cpp \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    slidepage.cpp \
    specifiedWidgets.cpp

HEADERS += \
    basicClass.h \
    clock.h \
    customScrollContainer.h \
    customWidgets.h \
    loginpage.h \
    mainpage.h \
    mainwindow.h \
    slidepage.h \
    specifiedWidgets.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
