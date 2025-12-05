#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDateTime>
#include <QColor>
#include <QList>
#include "Centrale.h"

namespace Ui {
class MainWindow1;
}

class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow1(QWidget *parent = nullptr);
    explicit MainWindow1(Centrale* centrale, QWidget *parent = nullptr);
    ~MainWindow1();

private slots:
    void mettreAJourGraphique();

private:
    Ui::MainWindow1 *ui;
    Centrale* centraleActuelle;
    QTimer* timer;
    QList<QGraphicsView*> vuesGraphiques; // Liste des vues de graphiques

    // Méthodes pour le graphique
    void afficherGraphiquesDebits();
    void initialiserDonneesHistoriques();
    QGraphicsScene* creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur);

    // Fonctions de production (conservées pour compatibilité)
    float fonctionT1(float du, float hc);
    float fonctionT2(float du, float hc);
    float fonctionT3(float du, float hc);
    float fonctionT4(float du, float hc);
    float fonctionT5(float du, float hc);
};

#endif // MAINWINDOW1_H
