// ========== GestionnaireComptes.h ==========
#ifndef GESTIONNAIRECOMPTES_H
#define GESTIONNAIRECOMPTES_H

#include "Utilisateur.h"
#include <QMap>
#include <QVector>
#include <QString>

// SINGLETON simplifié pour la gestion des comptes
class GestionnaireComptes {
private:
    static GestionnaireComptes* instance;
    QMap<int, Utilisateur*> utilisateurs;

    // Constructeur privé
    GestionnaireComptes();
    GestionnaireComptes(const GestionnaireComptes&) = delete;
    GestionnaireComptes& operator=(const GestionnaireComptes&) = delete;

public:
    ~GestionnaireComptes();

    // Accès à l'instance unique
    static GestionnaireComptes* getInstance();

    // Gestion basique
    bool ajouterUtilisateur(int id, const QString& login, const QString& password,
                            const QString& centrale);
    bool supprimerUtilisateur(int id);
    Utilisateur* rechercherUtilisateur(int id);

    // Affichage global et par centrale
    void afficherTousLesComptes();
    void afficherParCentrale(const QString& centrale);
    QVector<Utilisateur*> obtenirUtilisateursParCentrale(const QString& centrale);

    // Statistiques
    int getNombreTotalUtilisateurs() const;
    int getNombreUtilisateursParCentrale(const QString& centrale) const;
};

#endif
