# Generated from client.pro.

cmake_minimum_required(VERSION 3.16)
project(client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()
set(CMAKE_PREFIX_PATH "/Users/go2heart/Qt/6.2.4/macos/lib/cmake")

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Svg)
find_package(Qt6 COMPONENTS Widgets)

qt_add_executable(client WIN32 MACOSX_BUNDLE
    basicClass.cpp basicClass.h
    clock.cpp clock.h
    customScrollContainer.cpp customScrollContainer.h
    customWidgets.cpp customWidgets.h
    loginpage.cpp loginpage.h
    main.cpp
    mainpage.cpp mainpage.h
    mainwindow.cpp mainwindow.h mainwindow.ui
    slidepage.cpp slidepage.h
)
target_link_libraries(client PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Svg
    Qt::Widgets
)


# Resources:
set(icon_resource_files
    "icons/user.svg"
)

qt6_add_resources(client "icon"
    PREFIX
        "/icons"
    FILES
        ${icon_resource_files}
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
