#include "MainwindowManager.h"
#include "ui_MainwindowManager.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindowManager::MainWindowManager(Centrale* c1, Centrale* c2, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowManager)
    , centrale1(c1)
    , centrale2(c2)
    , labelTitre(nullptr)
    , labelIdentifiant(nullptr)
    , labelMotDePasse(nullptr)
    , labelCentrale(nullptr)
    , champIdentifiant(nullptr)
    , champMotDePasse(nullptr)
    , comboCentrale(nullptr)
    , boutonAjouter(nullptr)
    , boutonDeconnexion(nullptr)
{
    ui->setupUi(this);

    resize(800, 600);
    setWindowTitle("Gestion Utilisateurs - Manager");

    creerInterface();
    creerBoutonDeconnexion();
}

MainWindowManager::~MainWindowManager()
{
    if (labelTitre) delete labelTitre;
    if (labelIdentifiant) delete labelIdentifiant;
    if (labelMotDePasse) delete labelMotDePasse;
    if (labelCentrale) delete labelCentrale;
    if (champIdentifiant) delete champIdentifiant;
    if (champMotDePasse) delete champMotDePasse;
    if (comboCentrale) delete comboCentrale;
    if (boutonAjouter) delete boutonAjouter;
    if (boutonDeconnexion) delete boutonDeconnexion;
    delete ui;
}

void MainWindowManager::creerInterface()
{
    // Titre
    labelTitre = new QLabel("Gestion Utilisateurs", ui->centralwidget);
    QFont fontTitre;
    fontTitre.setPointSize(32);
    fontTitre.setBold(true);
    labelTitre->setFont(fontTitre);
    labelTitre->setAlignment(Qt::AlignCenter);
    labelTitre->setStyleSheet("QLabel { color: white; }");
    labelTitre->setGeometry(0, 30, 800, 60);
    labelTitre->show();

    // Label Identifiant
    labelIdentifiant = new QLabel("Identifiant :", ui->centralwidget);
    labelIdentifiant->setGeometry(200, 150, 150, 30);
    labelIdentifiant->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    labelIdentifiant->show();

    // Champ Identifiant
    champIdentifiant = new QLineEdit(ui->centralwidget);
    champIdentifiant->setGeometry(350, 150, 250, 40);
    champIdentifiant->setPlaceholderText("Entrez l'identifiant");
    champIdentifiant->setStyleSheet(
        "QLineEdit { "
        "background-color: white; "
        "color: black; "
        "border: 2px solid #3498db; "
        "border-radius: 5px; "
        "padding: 5px; "
        "font-size: 14px; "
        "}"
        );
    champIdentifiant->show();

    // Label Mot de passe
    labelMotDePasse = new QLabel("Mot de passe :", ui->centralwidget);
    labelMotDePasse->setGeometry(200, 220, 150, 30);
    labelMotDePasse->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    labelMotDePasse->show();

    // Champ Mot de passe
    champMotDePasse = new QLineEdit(ui->centralwidget);
    champMotDePasse->setGeometry(350, 220, 250, 40);
    champMotDePasse->setPlaceholderText("Entrez le mot de passe");
    champMotDePasse->setEchoMode(QLineEdit::Password);
    champMotDePasse->setStyleSheet(
        "QLineEdit { "
        "background-color: white; "
        "color: black; "
        "border: 2px solid #3498db; "
        "border-radius: 5px; "
        "padding: 5px; "
        "font-size: 14px; "
        "}"
        );
    champMotDePasse->show();

    // Label Centrale
    labelCentrale = new QLabel("Centrale :", ui->centralwidget);
    labelCentrale->setGeometry(200, 290, 150, 30);
    labelCentrale->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; }");
    labelCentrale->show();

    // ComboBox Centrale
    comboCentrale = new QComboBox(ui->centralwidget);
    comboCentrale->setGeometry(350, 290, 250, 40);
    comboCentrale->addItem("Centrale 1", "1");
    comboCentrale->addItem("Centrale 2", "2");
    comboCentrale->setStyleSheet(
        "QComboBox { "
        "background-color: white; "
        "color: black; "
        "border: 2px solid #3498db; "
        "border-radius: 5px; "
        "padding: 5px; "
        "font-size: 14px; "
        "}"
        );
    comboCentrale->show();

    // Bouton Ajouter
    boutonAjouter = new QPushButton("Ajouter utilisateur", ui->centralwidget);
    boutonAjouter->setGeometry(300, 370, 200, 50);
    boutonAjouter->setStyleSheet(
        "QPushButton { "
        "background-color: #27ae60; "
        "color: white; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 16px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #229954; "
        "}"
        "QPushButton:pressed { "
        "background-color: #1e8449; "
        "}"
        );
    boutonAjouter->show();

    connect(boutonAjouter, &QPushButton::clicked, this, &MainWindowManager::ajouterUtilisateur);
}

void MainWindowManager::creerBoutonDeconnexion()
{
    boutonDeconnexion = new QPushButton("Se déconnecter", ui->centralwidget);
    boutonDeconnexion->setGeometry(300, 480, 200, 50);
    boutonDeconnexion->setStyleSheet(
        "QPushButton { "
        "background-color: #e74c3c; "
        "color: white; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "background-color: #a93226; "
        "}"
        );
    boutonDeconnexion->show();

    connect(boutonDeconnexion, &QPushButton::clicked, this, &MainWindowManager::deconnecter);
}

void MainWindowManager::ajouterUtilisateur()
{
    QString identifiant = champIdentifiant->text().trimmed();
    QString motDePasse = champMotDePasse->text().trimmed();
    QString centrale = comboCentrale->currentData().toString();

    // Validation
    if (identifiant.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'identifiant ne peut pas être vide !");
        return;
    }

    if (motDePasse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide !");
        return;
    }

    QString fichier = "/Users/mahautgalice/Desktop/Cours UQAC/S1/POO/ProjetFinal/App_PPOA/ListeUtilisateurs.txt";

    // ⭐ ÉTAPE 1 : LIRE le fichier pour trouver le dernier ID
    int dernierID = 0;
    QFile fileRead(fichier);
    if (fileRead.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileRead);
        while (!in.atEnd()) {
            QString ligne = in.readLine();
            QStringList champs = ligne.split(";");
            if (!champs.isEmpty()) {
                int id = champs[0].toInt();
                if (id > dernierID) dernierID = id;
            }
        }
        fileRead.close();
    } else {
        qDebug() << "Fichier introuvable, création avec ID 1";
    }

    int nouvelID = dernierID + 1;

    // ⭐ ÉTAPE 2 : ÉCRIRE le nouvel utilisateur
    QFile fileWrite(fichier);
    if (!fileWrite.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier utilisateurs !");
        return;
    }

    QTextStream out(&fileWrite);
    out << nouvelID << ";" << identifiant << ";" << motDePasse << ";" << centrale << ";Utilisateur\n";
    fileWrite.close();

    QMessageBox::information(this, "Succès",
                             QString("Utilisateur '%1' ajouté avec succès !\nCentrale: %2\nID: %3")
                                 .arg(identifiant).arg(centrale).arg(nouvelID));

    // Vider les champs
    champIdentifiant->clear();
    champMotDePasse->clear();
    comboCentrale->setCurrentIndex(0);
}

void MainWindowManager::deconnecter()
{
    qDebug() << "Déconnexion demandée depuis MainWindowManager";
    emit deconnexionDemandee();
    close();
}
