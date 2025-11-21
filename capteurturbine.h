#ifndef CAPTEURTURBINE_H
#define CAPTEURTURBINE_H

#include "DecorateurCapteur.h"

// Décorateur pour capteur de turbine (mesure de débit)
class CapteurTurbine : public DecorateurCapteur {
private:
    int numeroTurbine;

public:
    CapteurTurbine(Capteur* capteur, int numTurbine);

    void afficherMesure() const override;
    void afficherHistorique(int nombreMesures = 10) const override;
    std::string getUnite() const override { return "m3/s"; }
    std::string getType() const override;

    int getNumeroTurbine() const { return numeroTurbine; }
    double getDebitEnM3parS() const { return getMesure() ; }
};

#endif
