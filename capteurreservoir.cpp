#include "CapteurReservoir.h"
#include <iostream>
#include <iomanip>

CapteurReservoir::CapteurReservoir(Capteur* capteur, std::string pos)
    : DecorateurCapteur(capteur), position(pos) {}

void CapteurReservoir::afficherMesure() const {
    std::cout << "[CAPTEUR RESERVOIR " << position << "] ID: " << getId() << std::endl;
    std::cout << "  Niveau d'eau: " << std::fixed << std::setprecision(2)
              << getNiveauEnMetres() << " " << getUnite() << std::endl;
    std::cout << "  Etat d'usure: " << getNiveau() << std::endl;
    std::cout << "  Nombre de mesures: " << capteurDecore->getNombreMesures() << std::endl;
}

void CapteurReservoir::afficherHistorique(int nombreMesures) const {
    std::cout << "[HISTORIQUE RESERVOIR " << position << "] ID: " << getId() << std::endl;
    std::vector<MesureHistorique> hist = capteurDecore->getHistorique();

    int debut = (hist.size() > nombreMesures) ? hist.size() - nombreMesures : 0;

    for(int i = debut; i < hist.size(); i++) {
        double niveauM = hist[i].valeur;
        std::cout << "  [" << hist[i].timestamp.toString("yyyy-MM-dd HH:mm:ss").toStdString()
                  << "] " << std::fixed << std::setprecision(2) << niveauM << " m" << std::endl;
    }
}

std::string CapteurReservoir::getType() const {
    return "Capteur de niveau - Reservoir " + position;
}
