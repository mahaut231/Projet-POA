#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>

class QLineSeries;
class Centrale;

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

    void afficherGraphiquesTurbines();

private:
    Ui::MainWindow1 *ui;
    Centrale* centraleActuelle;

    // Méthodes vides pour compatibilité
    QLineSeries* creerCourbeTurbine1();
    QLineSeries* creerCourbeTurbine2();
    QLineSeries* creerCourbeTurbine3();
    QLineSeries* creerCourbeTurbine4();
    QLineSeries* creerCourbeTurbine5();

    // Calcul des fonctions de production
    float fonctionT1(float du, float hc);
    float fonctionT2(float du, float hc);
    float fonctionT3(float du, float hc);
    float fonctionT4(float du, float hc);
    float fonctionT5(float du, float hc);
};

#endif // MAINWINDOW1_H
