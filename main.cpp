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

#include <QCoreApplication>
#include "ChargeurCSV.h"
#include "Capteur.h"
#include "CapteurTurbine.h"
#include "Centrale.h"
#include "Reservoire.h"
#include "Turbine.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "\n========================================";
    qDebug() << "     TEST CHARGEMENT ET COMPARAISON     ";
    qDebug() << "========================================\n";

    QString fichier = "C:/Users/chanudet/Documents/Polytech/uqac/trimestre1/POO/Projet-POA/DataProjet2025.csv";

    // 1. CREER LES CAPTEURS DES TURBINES
    Capteur* capteurT1Base = new Capteur(20, 0, "Bon");
    Capteur* capteurT2Base = new Capteur(21, 0, "Bon");
    Capteur* capteurT3Base = new Capteur(22, 0, "Bon");
    Capteur* capteurT4Base = new Capteur(23, 0, "Bon");
    Capteur* capteurT5Base = new Capteur(24, 0, "Bon");

    // 2. CREER LES 5 TURBINES AVEC LEURS CAPTEURS
    Turbine* turbine1 = new Turbine(1,158, false, "2020-01-01", *capteurT1Base);
    Turbine* turbine2 = new Turbine(2, 141, false, "2020-01-01", *capteurT2Base);
    Turbine* turbine3 = new Turbine(3, 0, false, "2020-01-01", *capteurT3Base);
    Turbine* turbine4 = new Turbine(4, 140, false, "2020-01-01", *capteurT4Base);
    Turbine* turbine5 = new Turbine(5, 140, false, "2020-01-01", *capteurT5Base);

    // 3. AJOUTER LES TURBINES A LA LISTE
    std::vector<Turbine*> turbines;
    turbines.push_back(turbine1);
    turbines.push_back(turbine2);
    turbines.push_back(turbine3);
    turbines.push_back(turbine4);
    turbines.push_back(turbine5);

    // 4. CREER LA CENTRALE AVEC LES TURBINES
    Capteur capteurAmont(1, 137890, "Bon");
    Capteur capteurAval(2, 103550, "Bon");
    Reservoire reservoire(1, capteurAmont, capteurAval);
    Centrale* centrale = new Centrale(1, 137.90, 103.77, turbines, reservoire);

    qDebug() << "Centrale creee avec 5 turbines";
    qDebug() << "Hauteur de chute:" << (137.90 - 103.77) << "m\n";

    // 5. CHARGER LES DEBITS (Q1-Q5) DANS LES CAPTEURS
    qDebug() << "Chargement des debits du fichier CSV...";
    bool ok = ChargeurCSV::chargerDansCapteurs(fichier, capteurT1Base, capteurT2Base,
                                               capteurT3Base, capteurT4Base, capteurT5Base);

    if (!ok) {
        qDebug() << "\nERREUR: Impossible de charger le fichier!";
        return 1;
    }

    int nbLignes = ChargeurCSV::getNombreLignes(fichier);
    qDebug() << "Nombre total de lignes:" << nbLignes << "\n";

    // 6. AFFICHER NOMBRE DE MESURES
    qDebug() << "--- MESURES CHARGEES DANS LES CAPTEURS ---";
    qDebug() << "Capteur Turbine 1:" << capteurT1Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 2:" << capteurT2Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 3:" << capteurT3Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 4:" << capteurT4Base->getNombreMesures() << "mesures";
    qDebug() << "Capteur Turbine 5:" << capteurT5Base->getNombreMesures() << "mesures";

    // 7. CHOISIR UNE LIGNE A COMPARER (par exemple ligne 0)
    int ligneAComparer = 0;

    qDebug() << "\n========================================";
    qDebug() << "   COMPARAISON LIGNE" << ligneAComparer;
    qDebug() << "========================================\n";

    // 8. OBTENIR LES VALEURS DE CETTE LIGNE
    ComparaisonLigne comp = ChargeurCSV::comparerLigne(fichier, ligneAComparer, centrale);

    // Variables pour stocker les puissances théoriques (du fichier)
    double p1Theorique = comp.p1Fichier;
    double p2Theorique = comp.p2Fichier;
    double p3Theorique = comp.p3Fichier;
    double p4Theorique = comp.p4Fichier;
    double p5Theorique = comp.p5Fichier;
    double totalTheorique = comp.totalFichier;

    // Variables pour les puissances calculées
    double p1Calculee = comp.p1Calculee;
    double p2Calculee = comp.p2Calculee;
    double p3Calculee = comp.p3Calculee;
    double p4Calculee = comp.p4Calculee;
    double p5Calculee = comp.p5Calculee;
    double totalCalcule = comp.totalCalcule;

    qDebug() << "=== COMPARAISON PAR TURBINE ===\n";

    // TURBINE 1
    qDebug() << "TURBINE 1:";
    qDebug() << "  Puissance theorique (fichier):" << p1Theorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << p1Calculee << "MW";
    qDebug() << "  Difference:" << (p1Calculee - p1Theorique) << "MW";
    qDebug() << "  Erreur:" << QString::number(((p1Calculee - p1Theorique) / p1Theorique) * 100, 'f', 2) << "%\n";

    // TURBINE 2
    qDebug() << "TURBINE 2:";
    qDebug() << "  Puissance theorique (fichier):" << p2Theorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << p2Calculee << "MW";
    qDebug() << "  Difference:" << (p2Calculee - p2Theorique) << "MW";
    qDebug() << "  Erreur:" << QString::number(((p2Calculee - p2Theorique) / p2Theorique) * 100, 'f', 2) << "%\n";

    // TURBINE 3
    qDebug() << "TURBINE 3:";
    qDebug() << "  Puissance theorique (fichier):" << p3Theorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << p3Calculee << "MW";
    qDebug() << "  Difference:" << (p3Calculee - p3Theorique) << "MW";
    qDebug() << "  Erreur:" << QString::number(((p3Calculee - p3Theorique) / p3Theorique) * 100, 'f', 2) << "%\n";

    // TURBINE 4
    qDebug() << "TURBINE 4:";
    qDebug() << "  Puissance theorique (fichier):" << p4Theorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << p4Calculee << "MW";
    qDebug() << "  Difference:" << (p4Calculee - p4Theorique) << "MW";
    qDebug() << "  Erreur:" << QString::number(((p4Calculee - p4Theorique) / p4Theorique) * 100, 'f', 2) << "%\n";

    // TURBINE 5
    qDebug() << "TURBINE 5:";
    qDebug() << "  Puissance theorique (fichier):" << p5Theorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << p5Calculee << "MW";
    qDebug() << "  Difference:" << (p5Calculee - p5Theorique) << "MW";
    if (p5Theorique != 0) {
        qDebug() << "  Erreur:" << QString::number(((p5Calculee - p5Theorique) / p5Theorique) * 100, 'f', 2) << "%\n";
    } else {
        qDebug() << "  Erreur: N/A (turbine inactive)\n";
    }

    // TOTAL
    qDebug() << "========================================";
    qDebug() << "PRODUCTION TOTALE:";
    qDebug() << "  Puissance theorique (fichier):" << totalTheorique << "MW";
    qDebug() << "  Puissance calculee (logiciel):" << totalCalcule << "MW";
    qDebug() << "  Difference:" << (totalCalcule - totalTheorique) << "MW";
    qDebug() << "  Erreur:" << QString::number(((totalCalcule - totalTheorique) / totalTheorique) * 100, 'f', 2) << "%";
    qDebug() << "========================================\n";

    // Afficher le tableau de comparaison
    ChargeurCSV::afficherComparaison(comp, ligneAComparer);


    qDebug() << "\n========================================";
    qDebug() << "              FIN DU TEST               ";
    qDebug() << "========================================\n";

    // Nettoyage
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
