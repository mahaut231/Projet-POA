#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    // Bouton de connexion
    connect(ui->loginButton, &QPushButton::clicked, this, &login::handleLogin);
}

login::~login()
{
    delete ui;
}

void login::handleLogin()
{
    // Ici, tu peux faire une vérification de mot de passe
    // Pour l'instant, on accepte directement
    accept();  // <-- important : marque le QDialog comme Accepted
}
