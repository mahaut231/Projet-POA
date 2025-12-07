#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "Centrale.h"
#include "Turbine.h"

class QTimer;
class QGraphicsScene;

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
    void appliquerChangementsDebit();

private:
    Ui::MainWindow1 *ui;
    Centrale* centraleActuelle;
    QTimer* timer;
    QLabel* labelCentrale;
    QList<QGraphicsView*> vuesGraphiques;
    QGraphicsView* vuePuissance = nullptr;

    // Champs de saisie pour les débits
    QLineEdit* champsDebit[5];
    QLabel* labelsDebit[5];
    QPushButton* boutonChangerDebit;

    void initialiserDonneesHistoriques();
    void afficherGraphiquesDebits();
    void afficherGraphiquePuissanceTotale();
    void creerChampsDebit();
    void creerBoutonChangerDebit();

    QGraphicsScene* creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur);
    QGraphicsScene* creerGraphiquePuissanceTotale();

    // Fonctions de production
    float fonctionT1(float du, float hc);
    float fonctionT2(float du, float hc);
    float fonctionT3(float du, float hc);
    float fonctionT4(float du, float hc);
    float fonctionT5(float du, float hc);
};

#endif // MAINWINDOW1_H
