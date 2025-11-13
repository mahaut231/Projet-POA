/********************************************************************************
** Form generated from reading UI file 'Login.ui'
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

class Ui_Login
{
public:
    QLineEdit *idText;
    QLineEdit *passwordText;
    QLabel *label;
    QLabel *label_2;
    QPushButton *LoginButton;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(400, 300);
        idText = new QLineEdit(Login);
        idText->setObjectName("idText");
        idText->setGeometry(QRect(30, 70, 331, 21));
        passwordText = new QLineEdit(Login);
        passwordText->setObjectName("passwordText");
        passwordText->setGeometry(QRect(30, 170, 331, 21));
        passwordText->setEchoMode(QLineEdit::EchoMode::Password);
        label = new QLabel(Login);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 50, 71, 16));
        label_2 = new QLabel(Login);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(30, 150, 131, 16));
        LoginButton = new QPushButton(Login);
        LoginButton->setObjectName("LoginButton");
        LoginButton->setGeometry(QRect(140, 220, 100, 32));

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Dialog", nullptr));
        idText->setText(QString());
        passwordText->setText(QString());
        label->setText(QCoreApplication::translate("Login", "Identifiant", nullptr));
        label_2->setText(QCoreApplication::translate("Login", "Mot de passe", nullptr));
        LoginButton->setText(QCoreApplication::translate("Login", "Se connecter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
