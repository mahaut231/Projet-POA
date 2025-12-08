#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include "Centrale.h"

namespace Ui {
class MainWindow2;
}

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(Centrale* centrale, QWidget *parent = nullptr);
    explicit MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();

signals:
    void deconnexionDemandee();

private:
    Ui::MainWindow2 *ui;
    Centrale* centraleActuelle;

    QList<QGraphicsView*> vuesGraphiques;
    QGraphicsView* vuePuissance;
    QLabel* labelCentrale;
    QPushButton* boutonChangerDebit;
    QPushButton* boutonDeconnexion;

    QLineEdit* champsDebit[5];
    QLabel* labelsDebit[5];

    QTimer* timer;

    std::map<int, long> modificationsDebits;
    bool debitsModifies;

    void initialiserDonneesHistoriques();
    void afficherGraphiquesDebits();
    void afficherGraphiquePuissanceTotale();
    void creerChampsDebit();
    void creerBoutonChangerDebit();
    void creerBoutonDeconnexion();

    QGraphicsScene* creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur);
    QGraphicsScene* creerGraphiquePuissanceTotale();

    // Fonctions de production (mêmes que MainWindow1)
    float fonctionT1(float du, float hc);
    float fonctionT2(float du, float hc);
    float fonctionT3(float du, float hc);
    float fonctionT4(float du, float hc);
    float fonctionT5(float du, float hc);

private slots:
    void mettreAJourGraphique();
    void appliquerChangementsDebit();
    void deconnecter();
};

#endif // MAINWINDOW2_H
