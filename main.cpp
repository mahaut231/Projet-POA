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

#include <QCoreApplication>
#include "Manager.h"
#include "Utilisateur.h"
#include <QDebug>

#include "Capteur.h"
#include "CapteurReservoir.h"
#include "CapteurTurbine.h"
#include <iostream>
#include <QThread>

int main(int argc, char *argv[]) {
    std::cout << "=== TEST PATRON DECORATEUR AVEC HISTORIQUE ===\n\n";

    // ===== TEST 1: Capteur Réservoir Amont =====
    std::cout << "--- Test Capteur Reservoir Amont ---\n";
    Capteur* capteurBaseAmont = new Capteur(10, 145000, "Bon");
    CapteurReservoir* capteurAmont = new CapteurReservoir(capteurBaseAmont, "amont");

    capteurAmont->afficherMesure();

    // Simulation de mesures au fil du temps
    std::cout << "\nAjout de mesures...\n";
    QThread::msleep(100);
    capteurBaseAmont->ajouterMesure(146000);
    QThread::msleep(100);
    capteurBaseAmont->ajouterMesure(147500);
    QThread::msleep(100);
    capteurBaseAmont->ajouterMesure(148000);

    std::cout << "\nMesure actuelle:\n";
    capteurAmont->afficherMesure();

    std::cout << "\nHistorique complet:\n";
    capteurAmont->afficherHistorique();

    // ===== TEST 2: Capteur Réservoir Aval =====
    std::cout << "\n--- Test Capteur Reservoir Aval ---\n";
    Capteur* capteurBaseAval = new Capteur(11, 102000, "Moyen");
    CapteurReservoir* capteurAval = new CapteurReservoir(capteurBaseAval, "aval");

    capteurAval->afficherMesure();

    std::cout << "\nAjout de mesures...\n";
    QThread::msleep(100);
    capteurBaseAval->ajouterMesure(101500);
    QThread::msleep(100);
    capteurBaseAval->ajouterMesure(101000);

    std::cout << "\nHistorique (2 dernieres mesures):\n";
    capteurAval->afficherHistorique(2);

    // ===== TEST 3: Capteurs de Turbines =====
    std::cout << "\n--- Test Capteurs de Turbines ---\n";

    Capteur* capteurBaseT1 = new Capteur(20, 50000, "Bon");
    CapteurTurbine* capteurT1 = new CapteurTurbine(capteurBaseT1, 1);

    Capteur* capteurBaseT2 = new Capteur(21, 75000, "Bon");
    CapteurTurbine* capteurT2 = new CapteurTurbine(capteurBaseT2, 2);

    capteurT1->afficherMesure();
    std::cout << "\n";
    capteurT2->afficherMesure();

    // Simulation de variation de débit sur Turbine 1
    std::cout << "\nVariation de debit sur Turbine 1...\n";
    QThread::msleep(100);
    capteurBaseT1->ajouterMesure(55000);
    QThread::msleep(100);
    capteurBaseT1->ajouterMesure(60000);
    QThread::msleep(100);
    capteurBaseT1->ajouterMesure(58000);

    std::cout << "\nHistorique Turbine 1:\n";
    capteurT1->afficherHistorique();

    // ===== TEST 4: Récupération historique pour graphique =====
    std::cout << "\n--- Test Recuperation pour Graphique ---\n";
    std::vector<MesureHistorique> hist = capteurBaseT1->getHistorique();
    std::cout << "Nombre de points de donnees: " << hist.size() << "\n";
    std::cout << "Donnees pour tracer un graphique:\n";
    for(int i = 0; i < hist.size(); i++) {
        double debit = hist[i].valeur / 1000.0;
        std::cout << "  Point " << (i+1) << ": "
                  << hist[i].timestamp.toString("HH:mm:ss").toStdString()
                  << " -> " << debit << " m3/s\n";
    }

    // ===== TEST 5: Simulation complète d'une centrale =====
    std::cout << "\n--- Simulation Complete Centrale ---\n";
    std::cout << "\nETAT ACTUEL DU RESERVOIR:\n";
    capteurAmont->afficherMesure();
    capteurAval->afficherMesure();

    std::cout << "\nETAT ACTUEL DES TURBINES:\n";
    capteurT1->afficherMesure();
    capteurT2->afficherMesure();

    std::cout << "\n--- Historiques disponibles pour graphiques ---\n";
    std::cout << "Reservoir amont: " << capteurBaseAmont->getNombreMesures() << " mesures\n";
    std::cout << "Reservoir aval: " << capteurBaseAval->getNombreMesures() << " mesures\n";
    std::cout << "Turbine 1: " << capteurBaseT1->getNombreMesures() << " mesures\n";
    std::cout << "Turbine 2: " << capteurBaseT2->getNombreMesures() << " mesures\n";

    // Nettoyage
    delete capteurAmont;
    delete capteurAval;
    delete capteurT1;
    delete capteurT2;
    delete capteurBaseAmont;
    delete capteurBaseAval;
    delete capteurBaseT1;
    delete capteurBaseT2;

    std::cout << "\n=== FIN DU TEST ===\n";

    return 0;

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



//=========================================================================================


}
