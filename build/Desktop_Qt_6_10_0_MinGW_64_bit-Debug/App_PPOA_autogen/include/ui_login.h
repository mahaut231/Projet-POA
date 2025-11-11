/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QLineEdit *idText;
    QLineEdit *passwordText;
    QLabel *label;
    QLabel *label_2;
    QPushButton *loginButton;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName("login");
        login->resize(400, 300);
        idText = new QLineEdit(login);
        idText->setObjectName("idText");
        idText->setGeometry(QRect(30, 70, 331, 21));
        passwordText = new QLineEdit(login);
        passwordText->setObjectName("passwordText");
        passwordText->setGeometry(QRect(30, 170, 331, 21));
        passwordText->setEchoMode(QLineEdit::EchoMode::Password);
        label = new QLabel(login);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 50, 71, 16));
        label_2 = new QLabel(login);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 150, 131, 16));
        loginButton = new QPushButton(login);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(140, 220, 100, 32));

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Dialog", nullptr));
        idText->setText(QCoreApplication::translate("login", "jgpygg", nullptr));
        passwordText->setText(QCoreApplication::translate("login", "oiigo", nullptr));
        label->setText(QCoreApplication::translate("login", "Identifiant", nullptr));
        label_2->setText(QCoreApplication::translate("login", "Mot de passe", nullptr));
        loginButton->setText(QCoreApplication::translate("login", "Se connecter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
