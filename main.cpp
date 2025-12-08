#include "Login.h"
#include "Mainwindow1.h"
#include "Mainwindow2.h"
#include "MainwindowManager.h"
#include <QApplication>
#include <QMessageBox>

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

    QApplication app(argc, argv);

    // ================== INITIALISATION DES CENTRALES ==================
    qDebug() << "\n========================================";
    qDebug() << "     INITIALISATION DES CENTRALES       ";
    qDebug() << "========================================\n";

    QString fichier = "/Users/mahautgalice/Desktop/Cours UQAC/S1/POO/ProjetFinal/App_PPOA/DataProjet2025.csv";

    // Création des capteurs
    Capteur* capteurT1Base = new Capteur(20, 150, "Bon");
    Capteur* capteurT2Base = new Capteur(21, 151, "Bon");
    Capteur* capteurT3Base = new Capteur(22, 150, "Bon");
    Capteur* capteurT4Base = new Capteur(23, 147, "Bon");
    Capteur* capteurT5Base = new Capteur(24, 160, "Bon");

    // Création des turbines pour Centrale 1
    Turbine* turbine1 = new Turbine(1, 0, false, "2020-01-01", *capteurT1Base);
    Turbine* turbine2 = new Turbine(2, 0, false, "2020-01-01", *capteurT2Base);
    Turbine* turbine3 = new Turbine(3, 0, false, "2020-01-01", *capteurT3Base);
    Turbine* turbine4 = new Turbine(4, 0, false, "2020-01-01", *capteurT4Base);
    Turbine* turbine5 = new Turbine(5, 0, false, "2020-01-01", *capteurT5Base);

    // Création des turbines pour Centrale 2 (partage les mêmes capteurs)
    Turbine* turbine1b = new Turbine(1, 0, false, "2020-01-01", *capteurT1Base);
    Turbine* turbine2b = new Turbine(2, 0, false, "2020-01-01", *capteurT2Base);
    Turbine* turbine3b = new Turbine(3, 0, false, "2020-01-01", *capteurT3Base);
    Turbine* turbine4b = new Turbine(4, 0, false, "2020-01-01", *capteurT4Base);
    Turbine* turbine5b = new Turbine(5, 0, false, "2020-01-01", *capteurT5Base);

    std::vector<Turbine*> turbines = {turbine1, turbine2, turbine3, turbine4, turbine5};
    std::vector<Turbine*> turbines2 = {turbine1b, turbine2b, turbine3b, turbine4b, turbine5b};

    Capteur capteurAmont(1, 137890, "Bon");
    Capteur capteurAval(2, 103550, "Bon");
    Reservoire reservoire(1, capteurAmont, capteurAval);
    Reservoire reservoire2(2, capteurAmont, capteurAval);

    Centrale* centrale1 = new Centrale(1, 137.90, 103.77, turbines, reservoire);
    Centrale* centrale2 = new Centrale(2, 137.90, 103.77, turbines2, reservoire2);

    qDebug() << "Centrales creees avec 5 turbines chacune";
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

    // Synchroniser les débits des turbines avec la dernière valeur du CSV
    qDebug() << "--- INITIALISATION DES DEBITS DEPUIS LE CSV ---";
    for (size_t t = 0; t < turbines.size(); t++) {
        std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();
        if (!historique.empty()) {
            long dernierDebit = historique.back().valeur;
            turbines[t]->setdebits(dernierDebit);
            turbines2[t]->setdebits(dernierDebit);
            qDebug() << "Turbine" << (t+1) << "initialisee avec debit:" << dernierDebit << "m³/s";
        }
    }

    qDebug() << "\n========================================";
    qDebug() << "        INITIALISATION TERMINEE         ";
    qDebug() << "========================================\n";

    // ================== BOUCLE DE LOGIN ==================
    while (true) {
        Login loginDialog;

        if (loginDialog.exec() == QDialog::Accepted) {
            QString centraleRef = loginDialog.getCentraleReference();
            QString role = loginDialog.getRole();

            qDebug() << "Connexion reussie !";
            qDebug() << "Centrale assignee:" << centraleRef;
            qDebug() << "Role:" << role;

            // ========== GESTION DES MANAGERS ==========
            if (role == "Manager") {
                qDebug() << "Manager connecte - Affichage du menu de choix";

                // Créer la boîte de dialogue de choix
                QMessageBox choixManager;
                choixManager.setWindowTitle("Menu Manager");
                choixManager.setText("À quelle page voulez-vous vous connecter ?");
                choixManager.setIcon(QMessageBox::Question);

                QPushButton* btnCentrale1 = choixManager.addButton("Centrale 1", QMessageBox::ActionRole);
                QPushButton* btnCentrale2 = choixManager.addButton("Centrale 2", QMessageBox::ActionRole);
                QPushButton* btnGestion = choixManager.addButton("Gestion des Utilisateurs", QMessageBox::ActionRole);
                QPushButton* btnAnnuler = choixManager.addButton("Annuler", QMessageBox::RejectRole);

                choixManager.exec();

                QPushButton* choix = (QPushButton*)choixManager.clickedButton();

                bool seDeconnecter = false;

                if (choix == btnCentrale1) {
                    // Ouvrir Centrale 1
                    qDebug() << "Manager a choisi Centrale 1";
                    MainWindow1 w1(centrale1);

                    QObject::connect(&w1, &MainWindow1::deconnexionDemandee, [&]() {
                        qDebug() << "Deconnexion demandee";
                        seDeconnecter = true;
                        QApplication::quit();
                    });

                    w1.show();
                    app.exec();

                    if (seDeconnecter) {
                        qDebug() << "Retour au login...";
                        continue;
                    } else {
                        qDebug() << "Fermeture de l'application";
                        break;
                    }

                } else if (choix == btnCentrale2) {
                    // Ouvrir Centrale 2
                    qDebug() << "Manager a choisi Centrale 2";
                    MainWindow2 w2(centrale2);

                    QObject::connect(&w2, &MainWindow2::deconnexionDemandee, [&]() {
                        qDebug() << "Deconnexion demandee";
                        seDeconnecter = true;
                        QApplication::quit();
                    });

                    w2.show();
                    app.exec();

                    if (seDeconnecter) {
                        qDebug() << "Retour au login...";
                        continue;
                    } else {
                        qDebug() << "Fermeture de l'application";
                        break;
                    }

                } else if (choix == btnGestion) {
                    // Ouvrir Gestion des Utilisateurs
                    qDebug() << "Manager a choisi Gestion des Utilisateurs";
                    MainWindowManager wManager(centrale1, centrale2);

                    QObject::connect(&wManager, &MainWindowManager::deconnexionDemandee, [&]() {
                        qDebug() << "Deconnexion demandee";
                        seDeconnecter = true;
                        QApplication::quit();
                    });

                    wManager.show();
                    app.exec();

                    if (seDeconnecter) {
                        qDebug() << "Retour au login...";
                        continue;
                    } else {
                        qDebug() << "Fermeture de l'application";
                        break;
                    }

                } else {
                    // Annuler - retour au login
                    qDebug() << "Manager a annule le choix - retour au login";
                    continue;
                }
            }
            // ========== GESTION DES UTILISATEURS NORMAUX ==========
            else if (centraleRef == "1") {
                qDebug() << "Ouverture de MainWindow1 (Centrale 1)";
                MainWindow1 w1(centrale1);

                bool seDeconnecter = false;

                QObject::connect(&w1, &MainWindow1::deconnexionDemandee, [&]() {
                    qDebug() << "Deconnexion demandee";
                    seDeconnecter = true;
                    QApplication::quit();
                });

                w1.show();
                app.exec();

                if (seDeconnecter) {
                    qDebug() << "Retour au login...";
                    continue;
                } else {
                    qDebug() << "Fermeture de l'application";
                    break;
                }

            } else if (centraleRef == "2") {
                qDebug() << "Ouverture de MainWindow2 (Centrale 2)";
                MainWindow2 w2(centrale2);

                bool seDeconnecter = false;

                QObject::connect(&w2, &MainWindow2::deconnexionDemandee, [&]() {
                    qDebug() << "Deconnexion demandee";
                    seDeconnecter = true;
                    QApplication::quit();
                });

                w2.show();
                app.exec();

                if (seDeconnecter) {
                    qDebug() << "Retour au login...";
                    continue;
                } else {
                    qDebug() << "Fermeture de l'application";
                    break;
                }

            } else {
                qDebug() << "Centrale inconnue, ouverture de Centrale 1 par defaut";
                MainWindow1 w1(centrale1);

                bool seDeconnecter = false;

                QObject::connect(&w1, &MainWindow1::deconnexionDemandee, [&]() {
                    qDebug() << "Deconnexion demandee";
                    seDeconnecter = true;
                    QApplication::quit();
                });

                w1.show();
                app.exec();

                if (seDeconnecter) {
                    qDebug() << "Retour au login...";
                    continue;
                } else {
                    qDebug() << "Fermeture de l'application";
                    break;
                }
            }

        } else {
            // Login annulé
            qDebug() << "Login annule";
            break;
        }
    }

    // Cleanup final
    qDebug() << "Nettoyage des ressources...";
    delete turbine1;
    delete turbine2;
    delete turbine3;
    delete turbine4;
    delete turbine5;
    delete turbine1b;
    delete turbine2b;
    delete turbine3b;
    delete turbine4b;
    delete turbine5b;
    delete capteurT1Base;
    delete capteurT2Base;
    delete capteurT3Base;
    delete capteurT4Base;
    delete capteurT5Base;
    delete centrale1;
    delete centrale2;

    qDebug() << "Application terminee";
    return 0;
}
