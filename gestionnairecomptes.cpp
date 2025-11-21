// ========== GestionnaireComptes.cpp ==========
#include "GestionnaireComptes.h"
#include <QDebug>

GestionnaireComptes* GestionnaireComptes::instance = nullptr;

GestionnaireComptes::GestionnaireComptes() {
    qDebug() << "[Gestionnaire] Initialise";
}

GestionnaireComptes::~GestionnaireComptes() {
    for(auto it = utilisateurs.begin(); it != utilisateurs.end(); ++it) {
        delete it.value();
    }
    utilisateurs.clear();
}

GestionnaireComptes* GestionnaireComptes::getInstance() {
    if(instance == nullptr) {
        instance = new GestionnaireComptes();
    }
    return instance;
}

bool GestionnaireComptes::ajouterUtilisateur(int id, const QString& login,
                                             const QString& password, const QString& centrale) {
    if(utilisateurs.contains(id)) {
        qDebug() << "[Erreur] ID" << id << "existe deja";
        return false;
    }

    Utilisateur* nouvel = new Utilisateur(id, login, password, centrale);
    utilisateurs[id] = nouvel;
    qDebug() << "[Ajoute]" << login << "(ID:" << id << ")";
    return true;
}

bool GestionnaireComptes::supprimerUtilisateur(int id) {
    if(!utilisateurs.contains(id)) {
        qDebug() << "[Erreur] ID" << id << "introuvable";
        return false;
    }

    QString login = utilisateurs[id]->getLogin();
    delete utilisateurs[id];
    utilisateurs.remove(id);
    qDebug() << "[Supprime]" << login << "(ID:" << id << ")";
    return true;
}

Utilisateur* GestionnaireComptes::rechercherUtilisateur(int id) {
    if(utilisateurs.contains(id)) {
        return utilisateurs[id];
    }
    return nullptr;
}

void GestionnaireComptes::afficherTousLesComptes() {
    qDebug() << "\n=== Liste des comptes ===" << utilisateurs.size() << "utilisateur(s)";
    for(auto it = utilisateurs.begin(); it != utilisateurs.end(); ++it) {
        it.value()->afficherInfos();
    }
    qDebug() << "========================\n";
}

void GestionnaireComptes::afficherParCentrale(const QString& centrale) {
    qDebug() << "\n=== Utilisateurs de" << centrale << "===";
    int count = 0;
    for(auto it = utilisateurs.begin(); it != utilisateurs.end(); ++it) {
        if(it.value()->getCentraleReference() == centrale) {
            it.value()->afficherInfos();
            count++;
        }
    }
    qDebug() << "Total:" << count << "utilisateur(s)\n";
}

QVector<Utilisateur*> GestionnaireComptes::obtenirUtilisateursParCentrale(const QString& centrale) {
    QVector<Utilisateur*> liste;
    for(auto it = utilisateurs.begin(); it != utilisateurs.end(); ++it) {
        if(it.value()->getCentraleReference() == centrale) {
            liste.append(it.value());
        }
    }
    return liste;
}

int GestionnaireComptes::getNombreTotalUtilisateurs() const {
    return utilisateurs.size();
}

int GestionnaireComptes::getNombreUtilisateursParCentrale(const QString& centrale) const {
    int count = 0;
    for(auto it = utilisateurs.begin(); it != utilisateurs.end(); ++it) {
        if(it.value()->getCentraleReference() == centrale) {
            count++;
        }
    }
    return count;
}
