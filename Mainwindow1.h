#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsScene>
#include <QDateTime>
#include <QColor>
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

    // Méthodes pour le graphique
    void afficherGraphiquesDebits();
    void initialiserDonneesHistoriques();
    void dessinerGraphiqueTurbine(QGraphicsScene* scene, Turbine* turbine,
                                  int numeroTurbine, QColor couleur,
                                  double xOffset, double yOffset,
                                  double width, double height,
                                  double margin, QDateTime debut,
                                  QDateTime fin, double debitMin,
                                  double debitMax);

    // Fonctions de production (conservées pour compatibilité)
    float fonctionT1(float du, float hc);
    float fonctionT2(float du, float hc);
    float fonctionT3(float du, float hc);
    float fonctionT4(float du, float hc);
    float fonctionT5(float du, float hc);
};

#endif // MAINWINDOW1_H
