# Generated from client.pro.

cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "F:/Qt_software_6.2/6.2.4/mingw_64/lib/cmake")

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Svg)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Network)

qt_add_executable(client WIN32 MACOSX_BUNDLE
    activitypage.cpp activitypage.h
    basicClass.cpp basicClass.h
    clock.cpp clock.h
    connect.cpp connect.h
    customScrollContainer.cpp customScrollContainer.h
    customWidgets.cpp customWidgets.h
    graph_implement.cpp graph_implement.h
    graph_view.cpp graph_view.h
        guidepage.cpp guidepage.h
    loginpage.cpp loginpage.h
    main.cpp
    mainpage.cpp mainpage.h
    mainwindow.cpp mainwindow.h mainwindow.ui
    mycanvas.cpp mycanvas.h
    slidepage.cpp slidepage.h
)
target_link_libraries(client PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Svg
    Qt::Widgets
)


# Resources:
set(resourse_resource_files
    "icons/add.svg"
    "icons/alarm_off.svg"
    "icons/alarm_on.svg"
    "icons/arrow-down.svg"
    "icons/arrow-left.svg"
    "icons/find-path.svg"
    "icons/group-activity.svg"
    "icons/personal-activity.svg"
    "icons/user.svg"
)

qt6_add_resources(client "resourse"
    PREFIX
        "/icons"
    FILES
        ${resourse_resource_files}
)
set(resourse1_resource_files
    "combostyle.qss"
)

qt6_add_resources(client "resourse1"
    PREFIX
        "/style"
    FILES
        ${resourse1_resource_files}
)
set(resourse2_resource_files
    "test.map"
)

qt6_add_resources(client "resourse2"
    PREFIX
        "/map"
    FILES
        ${resourse2_resource_files}
)

if((QT_MAJOR_VERSION GREATER 4))
    target_link_libraries(client PUBLIC
        Qt::Widgets
    )
endif()

install(TARGETS client
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
