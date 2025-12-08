#include "Login.h"
#include "ui_Login.h"
#include "Utilisateur.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

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
        if (champs.size() >= 4) {
            int id = champs[0].toInt();
            QString login = champs[1];
            QString password = champs[2];
            QString centrale = champs[3];

            Utilisateur u(id, login, password, centrale);
            liste.append(u);
        }
    }
    file.close();
    return liste;
}

QList<Utilisateur> listeUtilisateurs;

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    listeUtilisateurs = chargerUtilisateursTxt("/Users/mahautgalice/Desktop/Cours UQAC/S1/POO/ProjetFinal/App_PPOA/ListeUtilisateurs.txt");

    connect(ui->LoginButton, &QPushButton::clicked, this, &Login::handleLogin);
}

Login::~Login()
{
    delete ui;
}

void Login::handleLogin()
{
    QString saisieLogin = ui->idText->text();
    QString saisiePassword = ui->passwordText->text();

    for (const Utilisateur& u : listeUtilisateurs) {
        if (u.getLogin() == saisieLogin && u.getPassword() == saisiePassword) {
            centraleConnectee = u.getCentraleReference();

            if (u.getCentraleReference() == "TOUTES") {
                roleConnecte = "Manager";
            } else {
                roleConnecte = "Utilisateur";
            }

            qDebug() << "Login reussi:" << saisieLogin << "Role:" << roleConnecte << "Centrale:" << centraleConnectee;

            accept();
            return;
        }
    }

    QMessageBox::warning(this, "Erreur", "Login ou mot de passe incorrect !");
}

QString Login::getCentraleReference() const
{
    return centraleConnectee;
}

QString Login::getRole() const
{
    return roleConnecte;
}
