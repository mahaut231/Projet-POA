#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <QString>

class Utilisateur
{
public:
    // Constructeur par défaut
    Utilisateur();

    // Constructeur paramétré pour initialiser tous les attributs
    Utilisateur(int i, const QString& log, const QString& pwd, const QString& centrale)
        : id(i), login(log), password(pwd), centraleReference(centrale) {}

    // Attributs
    int id;
    QString login;
    QString password;             // Mot de passe
    QString centraleReference;
};

#endif // UTILISATEUR_H
