#include "turbine.h"
#include "iostream"
turbine::turbine(int id, long puissance, bool etat, std::string date, int pMax, int pMin)
        : id(id), puissance(puissance), etatActivation(etat),
        dateMiseEnPlace(date), puissanceMax(pMax), puissanceMin(pMin) {}



    // Méthode pour activer la turbine
    void turbine::activer() {
        etatActivation = true;
        std::cout << "Turbine " << id << " activée.\n";
    }

    // Méthode pour désactiver la turbine
    void turbine::desactiver() {
        etatActivation = false;
        std::cout << "Turbine " << id << " désactivée.\n";
    }

    // Méthode pour afficher les informations
    void turbine::afficherInfos() const {
        std::cout << "Turbine #" << id << " (" << (etatActivation ? "Active" : "Inactive") << ")\n"
                  << "Puissance: " << puissance << "W\n"
                  << "Date: " << dateMiseEnPlace << "\n";
    }
