#include "Login.h"
#include "ui_Login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // Bouton de connexion
    connect(ui->LoginButton, &QPushButton::clicked, this, &Login::handleLogin);
}

Login::~Login()
{
    delete ui;
}

void Login::handleLogin()
{
    // Ici, tu peux faire une vérification de mot de passe
    // Pour l'instant, on accepte directement
    accept();  // <-- important : marque le QDialog comme Accepted
}
