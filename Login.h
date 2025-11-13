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

    // Getter pour récupérer la centrale de l'utilisateur connecté
    QString getCentraleReference() const;

private slots:
    void handleLogin();  // fonction appelée quand on clique sur le bouton

private:
    Ui::Login *ui;
    QString centraleConnectee; // centrale de l'utilisateur connecté
};

#endif // LOGIN_H
