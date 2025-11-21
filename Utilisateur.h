// ========== Utilisateur.h ==========
#ifndef UTILISATEUR_H
#define UTILISATEUR_H
#include <QString>
#include <QDateTime>

class Utilisateur {
protected:
    int id;
    QString login;
    QString password;
    QString centraleReference;
    QDateTime derniereConnexion;
    bool estActif;

public:
    Utilisateur();
    Utilisateur(int i, const QString& log, const QString& pwd, const QString& centrale);
    virtual ~Utilisateur() {}

    // Getters
    int getId() const { return id; }
    QString getLogin() const { return login; }
    QString getPassword() const { return password; }
    QString getCentraleReference() const { return centraleReference; }
    QDateTime getDerniereConnexion() const { return derniereConnexion; }
    bool getEstActif() const { return estActif; }

    // Setters
    void setId(int i) { id = i; }
    void setLogin(const QString& log) { login = log; }
    void setPassword(const QString& pwd) { password = pwd; }
    void setCentraleReference(const QString& centrale) { centraleReference = centrale; }
    void setDerniereConnexion(const QDateTime& dt) { derniereConnexion = dt; }
    void setEstActif(bool actif) { estActif = actif; }

    // Méthodes
    virtual void afficherInfos() const;
    virtual bool estManager() const { return false; }
};

#endif
