#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "Utilisateur.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    QString getCentraleReference() const;
    QString getRole() const;

private slots:
    void handleLogin();

private:
    Ui::Login *ui;
    QString centraleConnectee;
    QString roleConnecte;
};

#endif // LOGIN_H
