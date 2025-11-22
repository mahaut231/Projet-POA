#include "ChargeurCSV.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

bool ChargeurCSV::chargerDansCapteurs(const QString& fichierCSV,
                                      Capteur* capteurT1, Capteur* capteurT2,
                                      Capteur* capteurT3, Capteur* capteurT4,
                                      Capteur* capteurT5) {

    QFile file(fichierCSV);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: impossible d'ouvrir" << fichierCSV;
        return false;
    }

    QTextStream in(&file);

    // Lire l'en-tête
    if (!in.atEnd()) {
        in.readLine();
    }

    int compteur = 0;
    char sep = ';';

    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty()) continue;

        // Détecter le séparateur
        if (ligne.contains(';')) sep = ';';
        else if (ligne.contains(',')) sep = ',';

        QStringList cols = ligne.split(sep);

        if (cols.size() >= 15) {
            // Colonnes: 5:Q1, 7:Q2, 9:Q3, 11:Q4, 13:Q5
            double q1 = cols[5].replace(',', '.').toDouble();
            double q2 = cols[7].replace(',', '.').toDouble();
            double q3 = cols[9].replace(',', '.').toDouble();
            double q4 = cols[11].replace(',', '.').toDouble();
            double q5 = cols[13].replace(',', '.').toDouble();

            // Ajouter aux capteurs (m3/s -> valeur brute *1000)
            if (capteurT1) capteurT1->ajouterMesure(static_cast<long>(q1 * 1000));
            if (capteurT2) capteurT2->ajouterMesure(static_cast<long>(q2 * 1000));
            if (capteurT3) capteurT3->ajouterMesure(static_cast<long>(q3 * 1000));
            if (capteurT4) capteurT4->ajouterMesure(static_cast<long>(q4 * 1000));
            if (capteurT5) capteurT5->ajouterMesure(static_cast<long>(q5 * 1000));

            compteur++;
        }
    }

    file.close();
    qDebug() << "Chargement termine:" << compteur << "lignes chargees";
    return true;
}

ComparaisonLigne ChargeurCSV::comparerLigne(const QString& fichierCSV,
                                            int numeroLigne,
                                            Centrale* centrale) {

    ComparaisonLigne comp;

    QFile file(fichierCSV);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: impossible d'ouvrir" << fichierCSV;
        return comp;
    }

    QTextStream in(&file);

    // Lire l'en-tête
    if (!in.atEnd()) {
        in.readLine();
    }

    int ligneActuelle = 0;
    char sep = ';';

    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (ligne.isEmpty()) continue;

        if (ligneActuelle == numeroLigne) {
            // On a trouvé la ligne !
            if (ligne.contains(';')) sep = ';';
            else if (ligne.contains(',')) sep = ',';

            QStringList cols = ligne.split(sep);

            if (cols.size() >= 15) {
                // Lire les débits
                double q1 = cols[5].replace(',', '.').toDouble();
                double q2 = cols[7].replace(',', '.').toDouble();
                double q3 = cols[9].replace(',', '.').toDouble();
                double q4 = cols[11].replace(',', '.').toDouble();
                double q5 = cols[13].replace(',', '.').toDouble();

                // Lire les puissances du fichier
                comp.p1Fichier = cols[6].replace(',', '.').toDouble();
                comp.p2Fichier = cols[8].replace(',', '.').toDouble();
                comp.p3Fichier = cols[10].replace(',', '.').toDouble();
                comp.p4Fichier = cols[12].replace(',', '.').toDouble();
                comp.p5Fichier = cols[14].replace(',', '.').toDouble();

                // Calculer les puissances avec le logiciel
                if (centrale) {
                    comp.p1Calculee = centrale->fonctionT1(q1);
                    comp.p2Calculee = centrale->fonctionT2(q2);
                    comp.p3Calculee = centrale->fonctionT3(q3);
                    comp.p4Calculee = centrale->fonctionT4(q4);
                    comp.p5Calculee = centrale->fonctionT5(q5);
                }

                // Calculer les différences pour chaque turbine
                comp.diff1 = comp.p1Calculee - comp.p1Fichier;
                comp.diff2 = comp.p2Calculee - comp.p2Fichier;
                comp.diff3 = comp.p3Calculee - comp.p3Fichier;
                comp.diff4 = comp.p4Calculee - comp.p4Fichier;
                comp.diff5 = comp.p5Calculee - comp.p5Fichier;

                // Calculer les totaux
                comp.totalFichier = comp.p1Fichier + comp.p2Fichier + comp.p3Fichier +
                                    comp.p4Fichier + comp.p5Fichier;
                comp.totalCalcule = comp.p1Calculee + comp.p2Calculee + comp.p3Calculee +
                                    comp.p4Calculee + comp.p5Calculee;
                comp.diffTotal = comp.totalCalcule - comp.totalFichier;
            }

            break;
        }

        ligneActuelle++;
    }

    file.close();
    return comp;
}

void ChargeurCSV::afficherComparaison(const ComparaisonLigne& comp, int numeroLigne) {
    qDebug() << "\n========================================";
    qDebug() << "   COMPARAISON LIGNE" << numeroLigne;
    qDebug() << "========================================\n";

    qDebug() << "Turbine |  Fichier (MW) | Calculee (MW) |  Diff (MW)";
    qDebug() << "--------|---------------|---------------|------------";
    qDebug().noquote() << QString("   T1   |    %1    |    %2    |   %3")
                              .arg(comp.p1Fichier, 7, 'f', 2)
                              .arg(comp.p1Calculee, 7, 'f', 2)
                              .arg(comp.diff1, 6, 'f', 2);
    qDebug().noquote() << QString("   T2   |    %1    |    %2    |   %3")
                              .arg(comp.p2Fichier, 7, 'f', 2)
                              .arg(comp.p2Calculee, 7, 'f', 2)
                              .arg(comp.diff2, 6, 'f', 2);
    qDebug().noquote() << QString("   T3   |    %1    |    %2    |   %3")
                              .arg(comp.p3Fichier, 7, 'f', 2)
                              .arg(comp.p3Calculee, 7, 'f', 2)
                              .arg(comp.diff3, 6, 'f', 2);
    qDebug().noquote() << QString("   T4   |    %1    |    %2    |   %3")
                              .arg(comp.p4Fichier, 7, 'f', 2)
                              .arg(comp.p4Calculee, 7, 'f', 2)
                              .arg(comp.diff4, 6, 'f', 2);
    qDebug().noquote() << QString("   T5   |    %1    |    %2    |   %3")
                              .arg(comp.p5Fichier, 7, 'f', 2)
                              .arg(comp.p5Calculee, 7, 'f', 2)
                              .arg(comp.diff5, 6, 'f', 2);
    qDebug() << "--------|---------------|---------------|------------";
    qDebug().noquote() << QString(" TOTAL  |    %1   |    %2   |   %3")
                              .arg(comp.totalFichier, 7, 'f', 2)
                              .arg(comp.totalCalcule, 7, 'f', 2)
                              .arg(comp.diffTotal, 6, 'f', 2);
    qDebug() << "========================================\n";
}

int ChargeurCSV::getNombreLignes(const QString& fichierCSV) {
    QFile file(fichierCSV);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QTextStream in(&file);

    // Sauter l'en-tête
    if (!in.atEnd()) {
        in.readLine();
    }

    int compteur = 0;
    while (!in.atEnd()) {
        QString ligne = in.readLine().trimmed();
        if (!ligne.isEmpty()) {
            compteur++;
        }
    }

    file.close();
    return compteur;
}
