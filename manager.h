// ========== Manager.h ==========
#ifndef MANAGER_H
#define MANAGER_H
#include "Utilisateur.h"
#include <QVector>

// Forward declaration
class GestionnaireComptes;

class Manager : public Utilisateur {
private:
    QString departement;

public:
    Manager();
    Manager(int i, const QString& log, const QString& pwd, const QString& dept);

    QString getDepartement() const { return departement; }
    void setDepartement(const QString& dept) { departement = dept; }

    void afficherInfos() const override;
    bool estManager() const override { return true; }

    // Méthodes de gestion (accès simplifié au Singleton)
    bool ajouterUtilisateur(int id, const QString& login, const QString& password,
                            const QString& centrale);
    bool supprimerUtilisateur(int id);

    // Affichage global et par centrale
    void afficherTousLesComptes();
    void afficherParCentrale(const QString& centrale);
    QVector<Utilisateur*> obtenirUtilisateursParCentrale(const QString& centrale);

    // Statistiques
    int getNombreTotalUtilisateurs();
    int getNombreUtilisateursParCentrale(const QString& centrale);
};

#endif
