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
    std::cout << "=== TEST DU PATRON OBSERVER ===\n\n";

    // Création des capteurs
    Capteur capteurAmont(1, 125000, "Bon");
    Capteur capteurAval(2, 100000, "Moyen");
    Capteur capteurT1(3, 50000, "Bon");
    Capteur capteurT2(4, 75000, "Bon");
    Capteur capteurT3(5, 60000, "Moyen");
    Capteur capteurT4(6, 80000, "Bon");
    Capteur capteurT5(7, 70000, "Bon");

    // Création du réservoir
    Reservoire monReservoir(1, capteurAmont, capteurAval);

    // Création des turbines (ATTENTION: on les crée comme objets maintenant)
    Turbine turbine1(1,160, true, "2020-01-15", capteurT1);
    Turbine turbine2(2, 141, true, "2019-05-22", capteurT2);
    Turbine turbine3(3, 0, false, "2021-03-10", capteurT3);
    Turbine turbine4(4, 140, true, "2018-11-05", capteurT4);
    Turbine turbine5(5, 140, true, "2022-07-18", capteurT5);

    // Création du vecteur de pointeurs vers les turbines
    std::vector<Turbine*> lesTurbines;
    lesTurbines.push_back(&turbine1);
    lesTurbines.push_back(&turbine2);
    lesTurbines.push_back(&turbine3);
    lesTurbines.push_back(&turbine4);
    lesTurbines.push_back(&turbine5);

    // Création de la centrale (elle s'enregistre automatiquement comme observer)
    Centrale maCentrale(1,137.89, 103.76, lesTurbines, monReservoir);

    std::cout << "Centrale #" << maCentrale.getId() << " creee\n";
    maCentrale.afficherEtatCentrale();

    // ===== TEST 1: Changement de débit d'une turbine =====
    std::cout << "\n*** TEST 1: Changement de debit de la turbine 1 ***\n";
    turbine1.setdebits(90);  // La centrale sera automatiquement notifiée!

    maCentrale.afficherEtatCentrale();

    // ===== TEST 2: Activation d'une turbine inactive =====
    std::cout << "\n*** TEST 2: Activation de la turbine 3 ***\n";
    turbine3.activer();  // La centrale sera automatiquement notifiée!

    maCentrale.afficherEtatCentrale();

    // ===== TEST 3: Désactivation d'une turbine active =====
    std::cout << "\n*** TEST 3: Desactivation de la turbine 2 ***\n";
    turbine2.desactiver();  // La centrale sera automatiquement notifiée!

    maCentrale.afficherEtatCentrale();

    // ===== TEST 4: Changements multiples =====
    std::cout << "\n*** TEST 4: Changements multiples ***\n";
    std::cout << "Activation turbine 5 et changement debit turbine 4...\n";
    turbine5.activer();
    turbine4.setdebits(120);

    maCentrale.afficherEtatCentrale();

    // ===== TEST 5: Tentative de débit invalide =====
    std::cout << "\n*** TEST 5: Tentative de debit invalide ***\n";
    turbine1.setdebits(200);  // Devrait être rejeté (> 160)

    maCentrale.afficherEtatCentrale();

    // ===== TEST 6: Ajout d'une nouvelle turbine =====
    std::cout << "\n*** TEST 6: Ajout d'une nouvelle turbine ***\n";
    Capteur capteurT6(8, 85000, "Bon");
    Turbine nouvelleTurbine(6, 95, true, "2024-01-01", capteurT6);

    std::cout << "Ajout de la turbine 6...\n";
    maCentrale.addTurbine(&nouvelleTurbine);

    maCentrale.afficherEtatCentrale();

    // ===== Affichage détaillé des turbines =====
    std::cout << "\n===== DETAIL DES TURBINES =====\n";
    std::vector<Turbine*> turbines = maCentrale.getListeTurbine();
    for(int i = 0; i < turbines.size(); i++) {
        turbines[i]->afficherInfos();
        std::cout << "\n";
    }

    std::cout << "\n=== FIN DU TEST ===\n";

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
