#ifndef CAPTEURRESERVOIR_H
#define CAPTEURRESERVOIR_H

#include "DecorateurCapteur.h"

// Décorateur pour capteur de réservoir (mesure de niveau d'eau)
class CapteurReservoir : public DecorateurCapteur {
private:
    std::string position; // "amont" ou "aval"

public:
    CapteurReservoir(Capteur* capteur, std::string pos);

    void afficherMesure() const override;
    void afficherHistorique(int nombreMesures = 10) const override;
    std::string getUnite() const override { return "m"; }
    std::string getType() const override;

    std::string getPosition() const { return position; }
    double getNiveauEnMetres() const { return getMesure(); }
};

#endif
