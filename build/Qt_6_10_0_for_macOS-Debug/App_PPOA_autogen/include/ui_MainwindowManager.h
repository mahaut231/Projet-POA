/********************************************************************************
** Form generated from reading UI file 'MainwindowManager.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWMANAGER_H
#define UI_MAINWINDOWMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowManager
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindowManager)
    {
        if (MainWindowManager->objectName().isEmpty())
            MainWindowManager->setObjectName("MainWindowManager");
        MainWindowManager->resize(800, 600);
        centralwidget = new QWidget(MainWindowManager);
        centralwidget->setObjectName("centralwidget");
        MainWindowManager->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowManager);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 37));
        MainWindowManager->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindowManager);
        statusbar->setObjectName("statusbar");
        MainWindowManager->setStatusBar(statusbar);

        retranslateUi(MainWindowManager);

        QMetaObject::connectSlotsByName(MainWindowManager);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowManager)
    {
        MainWindowManager->setWindowTitle(QCoreApplication::translate("MainWindowManager", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowManager: public Ui_MainWindowManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWMANAGER_H
