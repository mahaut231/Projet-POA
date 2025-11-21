#include "Capteur.h"
#include <iostream>
#include <iomanip>

Capteur::Capteur(int id, long mesure, std::string niveauDusure)
    : id(id), mesure(mesure), niveauDusure(niveauDusure) {
    // Ajouter la première mesure à l'historique
    ajouterMesure(mesure);
}

void Capteur::setMesure(long mesure) {
    this->mesure = mesure;
    ajouterMesure(mesure);
}

void Capteur::ajouterMesure(long mesure) {
    ajouterMesure(mesure, QDateTime::currentDateTime());
}

void Capteur::ajouterMesure(long mesure, QDateTime timestamp) {
    historique.push_back(MesureHistorique(mesure, timestamp));
}

void Capteur::afficherMesure() const {
    std::cout << "Capteur #" << id << " - Mesure: " << mesure
              << " - Usure: " << niveauDusure << std::endl;
}

void Capteur::afficherHistorique(int nombreMesures) const {
    std::cout << "Historique du capteur #" << id << " (" << historique.size() << " mesures)" << std::endl;

    int debut = (historique.size() > nombreMesures) ? historique.size() - nombreMesures : 0;

    for(int i = debut; i < historique.size(); i++) {
        std::cout << "  [" << historique[i].timestamp.toString("yyyy-MM-dd HH:mm:ss").toStdString()
        << "] " << historique[i].valeur << std::endl;
    }
}
