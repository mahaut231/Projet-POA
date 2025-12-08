#ifndef MAINWINDOWMANAGER_H
#define MAINWINDOWMANAGER_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "Centrale.h"

namespace Ui {
class MainWindowManager;
}

class MainWindowManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowManager(Centrale* centrale1, Centrale* centrale2, QWidget *parent = nullptr);
    ~MainWindowManager();

signals:
    void deconnexionDemandee();

private:
    Ui::MainWindowManager *ui;
    Centrale* centrale1;
    Centrale* centrale2;

    QLabel* labelTitre;
    QLabel* labelIdentifiant;
    QLabel* labelMotDePasse;
    QLabel* labelCentrale;
    QLineEdit* champIdentifiant;
    QLineEdit* champMotDePasse;
    QComboBox* comboCentrale;
    QPushButton* boutonAjouter;
    QPushButton* boutonDeconnexion;

    void creerInterface();
    void creerBoutonDeconnexion();

private slots:
    void ajouterUtilisateur();
    void deconnecter();
};

#endif // MAINWINDOWMANAGER_H
