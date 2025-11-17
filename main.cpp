#include "Login.h"
#include "Mainwindow1.h"
#include "Mainwindow2.h"
#include <QApplication>

#include "Centrale.h"
#include "Turbine.h"
#include "Reservoire.h"
#include "Capteur.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    std::cout << "=== Test de la Centrale Hydroelectrique ===\n\n";

    // Création des capteurs
    Capteur capteurT1(1, 125, "bon");
    Capteur capteurT2(2, 100, "bon");
    Capteur capteurT3(3, 60, "moyen");
    Capteur capteurT4(4, 120, "bon");
    Capteur capteurT5(5, 0, "bon");


    Capteur capteurR1Amont(6, 120, "bon");
    Capteur capteurR1Aval(7, 110, "bon");
    // Création du réservoir
    Reservoire monReservoir(1, capteurR1Amont, capteurR1Aval);

    // Création des turbines avec leurs débits
    std::vector<Turbine> lesTurbines;
    lesTurbines.push_back(Turbine(1, 158.0, true, "2020-01-15",capteurT1));
    lesTurbines.push_back(Turbine(2, 141.0, true, "2019-05-22",capteurT2));
    lesTurbines.push_back(Turbine(3, 0, false, "2021-03-10",capteurT3));
    lesTurbines.push_back(Turbine(4, 140.0, true, "2018-11-05",capteurT4));
    lesTurbines.push_back(Turbine(5, 140.0, true, "2022-07-18",capteurT5));

    // Création de la centrale
    Centrale maCentrale(1,137.90,103.77, lesTurbines, monReservoir);

    std::cout << "Centrale #" << maCentrale.getId() << " creee\n";

    // Affichage des turbines
    std::cout << "--- Liste des Turbines ---\n";
    std::vector<Turbine> turbines = maCentrale.getListeTurbine();
    for(int i = 0; i < turbines.size(); i++) {
        turbines[i].afficherInfos();
        std::cout << "Debit (debits): " << turbines[i].getdebits() << " W\n\n";
    }

    // Test de la production totale de la centrale
    std::cout << "--- Production Totale de la Centrale ---\n";
    float hc = 34.33; // Hauteur de chute en mètres

    float productionTotale = maCentrale.calculerProductionTotale();
    std::cout << "Hauteur de chute: " << hc << " m\n";
    std::cout << "Production totale (turbines actives seulement): " << productionTotale << " MW\n\n";

    // Détail par turbine
    std::cout << "--- Detail de Production par Turbine ---\n";
    for(int i = 0; i < turbines.size(); i++) {
        if(turbines[i].getEtatActivation()) {
            float du = turbines[i].getdebits();
            float prod = 0;

            switch(turbines[i].getId()) {
            case 1: prod = maCentrale.fonctionT1( du); break;
            case 2: prod = maCentrale.fonctionT2( du); break;
            case 3: prod = maCentrale.fonctionT3( du); break;
            case 4: prod = maCentrale.fonctionT4( du); break;
            case 5: prod = maCentrale.fonctionT5( du); break;
            }

            std::cout << "Turbine " << turbines[i].getId() << " (ACTIVE) - ";
            std::cout << "Debit: " << du << " m3/s - Production: " << prod << " MW\n";
        } else {
            std::cout << "Turbine " << turbines[i].getId() << " (INACTIVE) - Production: 0 MW\n";
        }
    }

    // Calcul de l'élévation aval
    float elevAval = maCentrale.elevationAval();
    std::cout << "\n--- Elevation Aval ---\n";
    std::cout << "Elevation aval: " << elevAval << " m\n";

    // Test activation/désactivation
    std::cout << "\n--- Test Activation/Desactivation ---\n";
    std::cout << "Activation de la turbine 3...\n";
    turbines[2].activer();
    std::cout << "Desactivation de la turbine 1...\n";
    turbines[0].desactiver();

    // Nouveau calcul après modification
    std::cout << "\n--- Recalcul apres modifications ---\n";
    float nouvelleProduction = maCentrale.calculerProductionTotale();
    std::cout << "Nouvelle production totale: " << nouvelleProduction << " MW\n";

    return 0;


//=========================================================================================

    QApplication a(argc, argv);

    Login loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        // Récupérer la centrale de l'utilisateur (par exemple via un getter)
        QString centrale = loginDialog.getCentraleReference(); // il faudra ajouter cette méthode

        if (centrale == "1") {
            MainWindow1 w1;
            w1.show();
            return a.exec();
        } else if (centrale == "2") {
            MainWindow2 w2;
            w2.show();
            return a.exec();
        }
    }

    return 0; // login annulé ou échoué
}
