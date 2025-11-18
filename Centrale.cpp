#include "Centrale.h"
#include <vector>
#include <iostream>
#include <cmath>

Centrale::Centrale(int id,float niveauAmont, float niveauAval,std::vector<Turbine*> listeTurbine,Reservoire reservoire):
    id(id),
    niveauAmont(niveauAmont),
    niveauAval(niveauAval),
    hauteurDeChute(niveauAmont-niveauAval),
    listeDesTurbines(listeTurbine),
    reservoire(reservoire),
    debitsTotal(0),
    productionTotaleActuelle(0) {

    // S'enregistrer comme observer de toutes les turbines
    for(int i = 0; i < listeDesTurbines.size(); i++) {
        listeDesTurbines[i]->ajouterObserver(this);
    }

    // Calculer les valeurs initiales
    update();
}
void Centrale::update() {
    std::cout << "\n=== MISE A JOUR DE LA CENTRALE ===\n";

    // Recalculer le débit total
    debitsTotal = calculerdebitTotale();

    // Recalculer la production totale
    productionTotaleActuelle = calculerProductionTotale();

    std::cout << "Nouveau debit total: " << debitsTotal << " m3/s\n";
    std::cout << "Nouvelle production totale: " << productionTotaleActuelle << " MW\n";
    std::cout << "===================================\n\n";
}

int Centrale::getId(){return id;}
std::vector<Turbine*>Centrale:: getListeTurbine(){return listeDesTurbines;}
Reservoire Centrale::getreservoire(){return reservoire;}


void Centrale::setid(int id){
    this->id=id;
};

void Centrale::addTurbine(Turbine* turbine) {
    this->listeDesTurbines.push_back(turbine);
    turbine->ajouterObserver(this);  // S'enregistrer comme observer
    update();  // Recalculer
}

void Centrale::setReservoire(Reservoire reservoire){
    this->reservoire=reservoire;
};

float Centrale::fonctionT1(float du){
    float hc =hauteurDeChute;
    float p00 = 1.102;
    float p10 = -0.03187;
    float p01 = -0.04866;
    float p11 = 0.003308;
    float p02 = 0.002182;
    float p12 = 3.638*pow(10,-5);
    float p03 = -1.277*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    std::cout << "Valeur prod turbine 1: " << res << "\n";
    return res;

}

float Centrale::fonctionT2 (float du){
    float hc =hauteurDeChute;
    float p00 = -1.382;
    float p10 = 0.09969;
    float p01 = -1.945;
    float p11 = 0.09224;
    float p20 = -0.001724;
    float p02 = 0.007721;
    float p12 = -6.622*pow(10,-5);
    float p21 = -0.001096;
    float p03 = -1.933*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc,2) + p02*pow(du,2) + p21*du*pow(hc,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    std::cout << "Valeur prod turbine 2: " << res << "\n";
    return res;

}

float Centrale::fonctionT3( float du){
    float hc =hauteurDeChute;
    float p00 = 0.7799;
    float p10 = -0.02261;
    float p01 = 0.1995;
    float p11 = -0.001695;
    float p02 = -3.519* pow(10,-5);
    float p12 = 7.235 * pow(10,-5);
    float p03 = -9.338 * pow(10,-6);

    float res = p00 + p10*hc + p01*du + p11*hc* du + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    std::cout << "Valeur prod turbine 3: " << res << "\n";
    return res;
}

float Centrale::fonctionT4(float du){
    float hc =hauteurDeChute;
    float p00 = 20.22;
    float p10 = -0.5777;
    float p01 = -0.4586;
    float p11 = 0.01151;
    float p02 = 0.004886;
    float p12 = 1.379 * pow(10,-5);
    float p03 = -1.882 * pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*du*hc + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    std::cout << "Valeur prod turbine 4: " << res << "\n";
    return res;
}

float Centrale::fonctionT5( float du){
    float hc =hauteurDeChute;
    float p00 = -212.1;
    float p10 = 12.17;
    float p01 = 0.004397;
    float p11 = -0.006808;
    float p20 = -0.1746;
    float p02 = 0.004529;
    float p12 = -4.211*pow(10,-5);
    float p21 = 0.0002936;
    float p03 = -1.176*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc,2) + p02*pow(du,2) + p21*du*pow(hc,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    std::cout << "Valeur prod turbine 5: " << res << "\n";
    return res;
}



float Centrale::elevationAval(){
    float p1 = -1.453*exp(-6);
    float p2 = 0.007022;
    float p3 = 99.98;

    float res = p1*pow(debitsTotal,2) + p2*debitsTotal + p3;
    return res;
}

float Centrale::calculerdebitTotale() {
    float debitTot = 0.0;

    // Parcourir toutes les turbines
    for(int i = 0; i < listeDesTurbines.size(); i++) {
        Turbine* turbine = listeDesTurbines[i];  // Pointeur maintenant

        // Vérifier si la turbine est active
        if(turbine->getEtatActivation()) {  // -> au lieu de .
            float du = turbine->getdebits();
            debitTot +=du;
        }
    }
    return debitTot;
}

float Centrale::calculerProductionTotale() {
    float productionTotale = 0.0;

    // Parcourir toutes les turbines
    for(int i = 0; i < listeDesTurbines.size(); i++) {
        Turbine* turbine = listeDesTurbines[i];  // Pointeur maintenant

        // Vérifier si la turbine est active
        if(turbine->getEtatActivation()) {  // -> au lieu de .
            float du = turbine->getdebits();  // -> au lieu de .
            float production = 0.0;

            // Utiliser la fonction correspondant à l'ID de la turbine
            switch(turbine->getId()) {  // -> au lieu de .
            case 1:
                production = fonctionT1(du);
                break;
            case 2:
                production = fonctionT2(du);
                break;
            case 3:
                production = fonctionT3(du);
                break;
            case 4:
                production = fonctionT4(du);
                break;
            case 5:
                production = fonctionT5(du);
                break;
            default:
                production = 0.0;
                break;
            }

            productionTotale += production;
        }
    }
    std::cout << "Valeur prod toute les turbine: " << productionTotale << "\n";
    return productionTotale;
}

void Centrale::afficherEtatCentrale() {
    std::cout << "\n===== ETAT DE LA CENTRALE #" << id << " =====\n";
    std::cout << "Hauteur de chute: " << hauteurDeChute << " m\n";
    std::cout << "Debit total: " << debitsTotal << " m3/s\n";
    std::cout << "Production totale: " << productionTotaleActuelle << " MW\n";
    std::cout << "Elevation aval: " << elevationAval() << " m\n";
    std::cout << "Nombre de turbines: " << listeDesTurbines.size() << "\n";

    int nbActives = 0;
    for(int i = 0; i < listeDesTurbines.size(); i++) {
        if(listeDesTurbines[i]->getEtatActivation()) {
            nbActives++;
        }
    }
    std::cout << "Turbines actives: " << nbActives << "/" << listeDesTurbines.size() << "\n";
    std::cout << "=========================================\n\n";
}
