cmake_minimum_required(VERSION 3.16)
project(App_PPOA VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPORTANT : Chercher QtCharts + PrintSupport
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts PrintSupport)

set(PROJECT_SOURCES
        main.cpp
        Mainwindow.cpp
        Mainwindow.h
        Mainwindow.ui
        Login.h
        Login.cpp
        Login.ui
        Turbine.h
        Turbine.cpp
        Capteur.h
        Capteur.cpp
        Reservoire.h
        Reservoire.cpp
        Centrale.h
        Centrale.cpp
        Utilisateur.h
        Utilisateur.cpp
        Mainwindow1.h
        Mainwindow1.cpp
        Mainwindow1.ui
        Mainwindow2.h
        Mainwindow2.cpp
        Mainwindow2.ui
        observer.h
        subject.h
        manager.h
        manager.cpp
        gestionnairecomptes.h
        gestionnairecomptes.cpp
        decorateurcapteur.h
        decorateurcapteur.cpp
        capteurreservoir.h
        capteurreservoir.cpp
        capteurturbine.h
        capteurturbine.cpp
        chargeurcsv.h
        chargeurcsv.cpp
        qcustomplot.h
        qcustomplot.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(App_PPOA
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(App_PPOA SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(App_PPOA
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(App_PPOA PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
    Qt6::PrintSupport   # <-- nécessaire pour QPrinter / savePdf()
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.App_PPOA)
endif()

set_target_properties(App_PPOA PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS App_PPOA
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(App_PPOA)
endif()
