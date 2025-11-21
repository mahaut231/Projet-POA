#include "CapteurTurbine.h"
#include <iostream>
#include <iomanip>

CapteurTurbine::CapteurTurbine(Capteur* capteur, int numTurbine)
    : DecorateurCapteur(capteur), numeroTurbine(numTurbine) {}

void CapteurTurbine::afficherMesure() const {
    std::cout << "[CAPTEUR TURBINE #" << numeroTurbine << "] ID: " << getId() << std::endl;
    std::cout << "  Debit: " << std::fixed << std::setprecision(2)
              << getDebitEnM3parS() << " " << getUnite() << std::endl;
    std::cout << "  Etat d'usure: " << getNiveau() << std::endl;
    std::cout << "  Nombre de mesures: " << capteurDecore->getNombreMesures() << std::endl;
}

void CapteurTurbine::afficherHistorique(int nombreMesures) const {
    std::cout << "[HISTORIQUE TURBINE #" << numeroTurbine << "] ID: " << getId() << std::endl;
    std::vector<MesureHistorique> hist = capteurDecore->getHistorique();

    int debut = (hist.size() > nombreMesures) ? hist.size() - nombreMesures : 0;

    for(int i = debut; i < hist.size(); i++) {
        double debitM3 = hist[i].valeur ;
        std::cout << "  [" << hist[i].timestamp.toString("yyyy-MM-dd HH:mm:ss").toStdString()
                  << "] " << std::fixed << std::setprecision(2) << debitM3 << " m3/s" << std::endl;
    }
}

std::string CapteurTurbine::getType() const {
    return "Capteur de debit - Turbine #" + std::to_string(numeroTurbine);
}

