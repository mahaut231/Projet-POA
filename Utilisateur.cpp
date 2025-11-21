// ========== Utilisateur.cpp ==========
#include "Utilisateur.h"
#include <QDebug>

Utilisateur::Utilisateur()
    : id(0), login(""), password(""), centraleReference(""), estActif(true) {
    derniereConnexion = QDateTime::currentDateTime();
}

Utilisateur::Utilisateur(int i, const QString& log, const QString& pwd, const QString& centrale)
    : id(i), login(log), password(pwd), centraleReference(centrale), estActif(true) {
    derniereConnexion = QDateTime::currentDateTime();
}

void Utilisateur::afficherInfos() const {
    qDebug() << "ID:" << id << "| Login:" << login << "| Centrale:" << centraleReference
             << "| Actif:" << (estActif ? "Oui" : "Non");
}
