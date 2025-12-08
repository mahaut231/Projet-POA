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

#include "Manager.h"
#include "Utilisateur.h"
#include <QDebug>

#include "CapteurReservoir.h"
#include "CapteurTurbine.h"
#include <QThread>

#include "ChargeurCSV.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);  // <-- Une seule instance QApplication

    // ================== CODE TERMINAL EXISTANT ==================
    qDebug() << "\n========================================";
    qDebug() << "     TEST CHARGEMENT ET COMPARAISON     ";
    qDebug() << "========================================\n";

    QString fichier = "/Users/mahautgalice/Desktop/Cours UQAC/S1/POO/ProjetFinal/App_PPOA/DataProjet2025.csv";

    // Création des capteurs
    Capteur* capteurT1Base = new Capteur(20, 150, "Bon");
    Capteur* capteurT2Base = new Capteur(21, 151, "Bon");
    Capteur* capteurT3Base = new Capteur(22, 150, "Bon");
    Capteur* capteurT4Base = new Capteur(23, 147, "Bon");
    Capteur* capteurT5Base = new Capteur(24, 160, "Bon");

    // Création des turbines SANS débits initiaux (0 par défaut)
    Turbine* turbine1 = new Turbine(1, 0, false, "2020-01-01", *capteurT1Base);
    Turbine* turbine2 = new Turbine(2, 0, false, "2020-01-01", *capteurT2Base);
    Turbine* turbine3 = new Turbine(3, 0, false, "2020-01-01", *capteurT3Base);
    Turbine* turbine4 = new Turbine(4, 0, false, "2020-01-01", *capteurT4Base);
    Turbine* turbine5 = new Turbine(5, 0, false, "2020-01-01", *capteurT5Base);

    std::vector<Turbine*> turbines = {turbine1, turbine2, turbine3, turbine4, turbine5};

    Capteur capteurAmont(1, 137890, "Bon");
    Capteur capteurAval(2, 103550, "Bon");
    Reservoire reservoire(1, capteurAmont, capteurAval);
    Centrale* centrale = new Centrale(1, 137.90, 103.77, turbines, reservoire);

    qDebug() << "Centrale creee avec 5 turbines";
    qDebug() << "Hauteur de chute:" << (137.90 - 103.77) << "m\n";

    // Charger les débits
    qDebug() << "Chargement des debits du fichier CSV...";
    bool ok = ChargeurCSV::chargerDansCapteurs(fichier, capteurT1Base, capteurT2Base,
                                               capteurT3Base, capteurT4Base, capteurT5Base);

    if (!ok) {
        qDebug() << "\nERREUR: Impossible de charger le fichier!";
        return 1;
    }

    int nbLignes = ChargeurCSV::getNombreLignes(fichier);
    qDebug() << "Nombre total de lignes:" << nbLignes << "\n";

    // NOUVEAU : Synchroniser les débits des turbines avec la dernière valeur du CSV
    qDebug() << "--- INITIALISATION DES DEBITS DEPUIS LE CSV ---";
    for (size_t t = 0; t < turbines.size(); t++) {
        std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();
        if (!historique.empty()) {
            long dernierDebit = historique.back().valeur;
            turbines[t]->setdebits(dernierDebit);
            qDebug() << "Turbine" << (t+1) << "initialisee avec debit:" << dernierDebit << "m³/s";
        }
    }
    qDebug() << "";

    // Afficher mesures par capteur
    qDebug() << "--- MESURES CHARGEES DANS LES CAPTEURS ---";
    qDebug() << "Capteur Turbine 1:" << capteurT1Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 2:" << capteurT2Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 3:" << capteurT3Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 4:" << capteurT4Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 5:" << capteurT5Base->getNombreMesures() << "mesures";

    // Comparaison ligne 0
    int ligneAComparer = 0;
    qDebug() << "\n========================================";
    qDebug() << "   COMPARAISON LIGNE" << ligneAComparer;
    qDebug() << "========================================\n";

    ComparaisonLigne comp = ChargeurCSV::comparerLigne(fichier, ligneAComparer, centrale);

    ChargeurCSV::afficherComparaison(comp, ligneAComparer);

    qDebug() << "\n========================================";
    qDebug() << "              FIN DU TEST               ";
    qDebug() << "========================================\n";

    // ================== AFFICHER LA FENETRE ==================
    //MainWindow1 w1(centrale);  // important : passer la centrale
     //w1.show();
    MainWindow2 w2(centrale);
     w2.show();

    // NE PAS supprimer les objets ici, faire le cleanup après app.exec()
    int ret = app.exec();

    // ----- nettoyage après fermeture de la fenêtre -----
    delete turbine1;
    delete turbine2;
    delete turbine3;
    delete turbine4;
    delete turbine5;
    delete capteurT1Base;
    delete capteurT2Base;
    delete capteurT3Base;
    delete capteurT4Base;
    delete capteurT5Base;
    delete centrale;

    return ret;
}
