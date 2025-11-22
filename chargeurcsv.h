#ifndef CHARGEURCSV_H
#define CHARGEURCSV_H

#include <QString>
#include <QVector>
#include "Capteur.h"
#include "Centrale.h"

struct ComparaisonLigne {
    // Turbines individuelles
    double p1Fichier;
    double p1Calculee;
    double diff1;

    double p2Fichier;
    double p2Calculee;
    double diff2;

    double p3Fichier;
    double p3Calculee;
    double diff3;

    double p4Fichier;
    double p4Calculee;
    double diff4;

    double p5Fichier;
    double p5Calculee;
    double diff5;

    // Total
    double totalFichier;
    double totalCalcule;
    double diffTotal;

    ComparaisonLigne() : p1Fichier(0), p1Calculee(0), diff1(0),
        p2Fichier(0), p2Calculee(0), diff2(0),
        p3Fichier(0), p3Calculee(0), diff3(0),
        p4Fichier(0), p4Calculee(0), diff4(0),
        p5Fichier(0), p5Calculee(0), diff5(0),
        totalFichier(0), totalCalcule(0), diffTotal(0) {}
};

class ChargeurCSV {
public:
    // Charger le CSV et remplir les capteurs
    static bool chargerDansCapteurs(const QString& fichierCSV,
                                    Capteur* capteurT1, Capteur* capteurT2,
                                    Capteur* capteurT3, Capteur* capteurT4,
                                    Capteur* capteurT5);

    // Comparer une ligne spécifique
    static ComparaisonLigne comparerLigne(const QString& fichierCSV,
                                          int numeroLigne,
                                          Centrale* centrale);

    // Afficher le résultat de la comparaison
    static void afficherComparaison(const ComparaisonLigne& comp, int numeroLigne);

    // Obtenir le nombre de lignes du fichier
    static int getNombreLignes(const QString& fichierCSV);
};

#endif
