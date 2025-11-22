#ifndef DECORATEURCAPTEUR_H
#define DECORATEURCAPTEUR_H

#include "Capteur.h"

// Classe de base pour les décorateurs
class DecorateurCapteur : public Capteur {
protected:
    Capteur* capteurDecore;

public:
    DecorateurCapteur(Capteur* capteur);
    virtual ~DecorateurCapteur() {}


    int getId() const { return capteurDecore->getId(); }
    long getMesure() const { return capteurDecore->getMesure(); }
    std::string getNiveau() const { return capteurDecore->getNiveau(); }

    // Méthodes à redéfinir dans les décorateurs concrets
    void afficherMesure() const override = 0;
    void afficherHistorique(int nombreMesures = 10) const override = 0;
    std::string getUnite() const override = 0;
    std::string getType() const override = 0;
};

#endif
