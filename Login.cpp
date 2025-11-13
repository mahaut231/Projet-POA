#include "Login.h"
#include "ui_Login.h"
#include "Utilisateur.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

// Fonction pour charger les utilisateurs depuis le fichier txt
QList<Utilisateur> chargerUtilisateursTxt(const QString& fichier)
{
    QList<Utilisateur> liste;
    QFile file(fichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossible d'ouvrir le fichier pour lecture :" << fichier;
        return liste;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty())
            continue;

        QStringList champs = ligne.split(";");
        if (champs.size() == 4) {
            Utilisateur u;
            u.id = champs[0].toInt();
            u.login = champs[1];
            u.password = champs[2];
            u.centraleReference = champs[3];
            liste.append(u);
        }
    }

    file.close();
    return liste;
}

// Variable globale pour stocker les utilisateurs
QList<Utilisateur> listeUtilisateurs;

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    // Charger les utilisateurs depuis le fichier txt
    listeUtilisateurs = chargerUtilisateursTxt("../../../ListeUtilisateurs.txt");

    // Connexion du bouton login
    connect(ui->LoginButton, &QPushButton::clicked, this, &Login::handleLogin);
}

Login::~Login()
{
    delete ui;
}

// Fonction pour gérer le login
void Login::handleLogin()
{
    QString saisieLogin = ui->idText->text();
    QString saisiePassword = ui->passwordText->text();

    for (const Utilisateur& u : listeUtilisateurs) {
        if (u.login == saisieLogin && u.password == saisiePassword) {
            // Stocker la centrale de l'utilisateur pour récupérer après
            centraleConnectee = u.centraleReference;

            accept(); // ferme le dialogue et retourne QDialog::Accepted
            return;
        }
    }

    // Si aucun utilisateur ne correspond
    QMessageBox::warning(this, "Erreur", "Login ou mot de passe incorrect !");
}

// Getter pour récupérer la centrale de l'utilisateur connecté
QString Login::getCentraleReference() const
{
    return centraleConnectee;
}
