// ========== Manager.cpp ==========
#include "Manager.h"
#include "GestionnaireComptes.h"
#include <QDebug>

Manager::Manager() : Utilisateur() {
    departement = "";
    centraleReference = "TOUTES";
}

Manager::Manager(int i, const QString& log, const QString& pwd, const QString& dept)
    : Utilisateur() {
    id = i;
    login = log;
    password = pwd;
    centraleReference = "TOUTES";
    departement = dept;
    derniereConnexion = QDateTime::currentDateTime();
    estActif = true;
}

void Manager::afficherInfos() const {
    qDebug() << "[MANAGER] ID:" << getId() << "| Login:" << getLogin()
    << "| Dept:" << departement << "| Gere: Toutes les centrales";
}

bool Manager::ajouterUtilisateur(int id, const QString& login,
                                 const QString& password, const QString& centrale) {
    return GestionnaireComptes::getInstance()->ajouterUtilisateur(id, login, password, centrale);
}

bool Manager::supprimerUtilisateur(int id) {
    return GestionnaireComptes::getInstance()->supprimerUtilisateur(id);
}

void Manager::afficherTousLesComptes() {
    GestionnaireComptes::getInstance()->afficherTousLesComptes();
}

void Manager::afficherParCentrale(const QString& centrale) {
    GestionnaireComptes::getInstance()->afficherParCentrale(centrale);
}

QVector<Utilisateur*> Manager::obtenirUtilisateursParCentrale(const QString& centrale) {
    return GestionnaireComptes::getInstance()->obtenirUtilisateursParCentrale(centrale);
}

int Manager::getNombreTotalUtilisateurs() {
    return GestionnaireComptes::getInstance()->getNombreTotalUtilisateurs();
}

int Manager::getNombreUtilisateursParCentrale(const QString& centrale) {
    return GestionnaireComptes::getInstance()->getNombreUtilisateursParCentrale(centrale);
}

