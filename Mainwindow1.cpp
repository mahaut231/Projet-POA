#include "Mainwindow1.h"
#include "ui_Mainwindow1.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QPen>
#include <QTimer>
#include <QLabel>
#include <cmath>

MainWindow1::MainWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(nullptr)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
{
    ui->setupUi(this);
}

MainWindow1::MainWindow1(Centrale* centrale, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(centrale)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
{
    ui->setupUi(this);

    // Agrandir la fenêtre pour accueillir les graphiques en 2x3
    resize(1300, 750);

    // Créer le label "Centrale 1" - simple texte blanc sur fond
    labelCentrale = new QLabel("Centrale 1", ui->centralwidget);
    QFont labelFont;
    labelFont.setPointSize(28);
    labelFont.setBold(true);
    labelCentrale->setFont(labelFont);
    labelCentrale->setAlignment(Qt::AlignCenter);
    labelCentrale->setStyleSheet("QLabel { color: white; background-color: transparent; }");
    labelCentrale->setGeometry(0, 20, 1300, 60);
    labelCentrale->show();

    // Initialiser les données historiques (simuler 6 heures de données)
    initialiserDonneesHistoriques();

    // Afficher les graphiques
    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();

    // Configurer le timer pour mise à jour toutes les 15 minutes
    connect(timer, &QTimer::timeout, this, &MainWindow1::mettreAJourGraphique);
    timer->start(900000); // 15 minutes en millisecondes
}

MainWindow1::~MainWindow1()
{
    // Nettoyer les vues
    for (auto view : vuesGraphiques) {
        delete view;
    }
    if (vuePuissance) {
        delete vuePuissance;
    }
    if (labelCentrale) {
        delete labelCentrale;
    }
    delete ui;
}

void MainWindow1::initialiserDonneesHistoriques()
{
    if (!centraleActuelle) return;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    // Vider l'historique existant pour repartir à zéro
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        turbines[t]->getCapteur().viderHistorique();
    }

    // Simuler 6 heures de données (une mesure toutes les 15 minutes = 24 mesures)
    QDateTime maintenant = QDateTime::currentDateTime();

    for (int i = 23; i >= 0; i--) {
        // Calculer le timestamp (en remontant dans le temps)
        QDateTime timestamp = maintenant.addSecs(-i * 15 * 60);

        // Pour chaque turbine, ajouter une mesure avec variation réaliste
        for (size_t t = 0; t < turbines.size() && t < 5; t++) {
            Turbine* turbine = turbines[t];

            // Générer un débit avec variation autour d'une valeur de base
            long debitBase = 140 + (t * 5); // Débits de base différents par turbine
            long variation = (rand() % 21) - 10; // Variation de -10 à +10
            long debit = debitBase + variation;

            // Ajouter la mesure à l'historique du capteur
            turbine->getCapteur().ajouterMesure(debit, timestamp);
        }
    }
}

void MainWindow1::mettreAJourGraphique()
{
    if (!centraleActuelle) return;

    // Ajouter une nouvelle mesure pour chaque turbine
    QDateTime maintenant = QDateTime::currentDateTime();
    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        Turbine* turbine = turbines[t];

        // Générer un nouveau débit avec variation
        long debitBase = 140 + (t * 5);
        long variation = (rand() % 21) - 10;
        long debit = debitBase + variation;

        turbine->getCapteur().ajouterMesure(debit, maintenant);
    }

    // Rafraîchir l'affichage
    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();
}

void MainWindow1::afficherGraphiquesDebits()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible";
        return;
    }

    // Nettoyer les anciennes vues
    for (auto view : vuesGraphiques) {
        delete view;
    }
    vuesGraphiques.clear();

    // Récupérer les turbines
    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    // Dimensions pour chaque graphique
    const int graphWidth = 400;
    const int graphHeight = 300;
    const int horizontalSpace = 20;
    const int verticalSpace = 20;
    const int startX = 20;
    const int startY = 100; // Sous le label "Centrale 1"

    // Couleurs pour chaque turbine
    QList<QColor> colors = {
        QColor(255, 20, 147),   // Rose fuschia - Turbine 1
        QColor(0, 0, 255),      // Bleu - Turbine 2
        QColor(0, 200, 0),      // Vert - Turbine 3
        QColor(255, 165, 0),    // Orange - Turbine 4
        QColor(128, 0, 128)     // Violet - Turbine 5
    };

    // Créer un graphique pour chaque turbine en grille 2x3
    for (int t = 0; t < nbTurbines; t++) {
        int row = t / 3;  // 0 pour turbines 1-3, 1 pour turbines 4-5
        int col = t % 3;  // 0, 1, 2

        int x = startX + col * (graphWidth + horizontalSpace);
        int y = startY + row * (graphHeight + verticalSpace);

        // Créer la scène et la vue pour ce graphique
        QGraphicsScene* scene = creerGraphiqueTurbine(turbines[t], t + 1, colors[t]);
        QGraphicsView* view = new QGraphicsView(scene, ui->centralwidget);
        view->setRenderHint(QPainter::Antialiasing);
        view->setGeometry(x, y, graphWidth, graphHeight);
        view->setStyleSheet("border: 2px solid gray; background-color: white;");
        view->show();

        // Stocker la vue
        vuesGraphiques.append(view);
    }
}

void MainWindow1::afficherGraphiquePuissanceTotale()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible pour puissance totale";
        return;
    }

    // Nettoyer l'ancienne vue
    if (vuePuissance) {
        delete vuePuissance;
        vuePuissance = nullptr;
    }

    // Créer la scène du graphique de puissance totale
    QGraphicsScene* scene = creerGraphiquePuissanceTotale();

    // Créer la vue - Position à droite de la turbine 5 (ligne 2, colonne 3)
    const int startX = 20;
    const int startY = 100;
    const int graphWidth = 400;
    const int graphHeight = 300;
    const int horizontalSpace = 20;
    const int verticalSpace = 20;

    // Ligne 2 (row = 1), colonne 3 (col = 2)
    int x = startX + 2 * (graphWidth + horizontalSpace);
    int y = startY + 1 * (graphHeight + verticalSpace);

    vuePuissance = new QGraphicsView(scene, ui->centralwidget);
    vuePuissance->setRenderHint(QPainter::Antialiasing);
    vuePuissance->setGeometry(x, y, graphWidth, graphHeight);
    vuePuissance->setStyleSheet("border: 2px solid gray; background-color: white;");
    vuePuissance->show();
}

QGraphicsScene* MainWindow1::creerGraphiquePuissanceTotale()
{
    QGraphicsScene* scene = new QGraphicsScene();

    // Dimensions du graphique (mêmes que les graphiques de turbines)
    const double width = 380;
    const double height = 280;
    const double margin = 70;  // Augmenté pour plus d'espace pour les valeurs Y
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    // Déterminer les plages de données
    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime debut = maintenant.addSecs(-6 * 3600); // 6 heures en arrière

    float hc = 35.0; // Hauteur de chute

    // Calculer la plage de puissance totale
    double puissanceMin = 0;
    double puissanceMax = 100;

    // Fond blanc pour la zone de tracé
    scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));

    // Grille
    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
    for (int i = 1; i < 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin, y, margin + plotWidth, y, gridPen);
    }
    for (int i = 1; i < 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin, x, margin + plotHeight, gridPen);
    }

    // Dessiner les axes
    QPen axisPen(Qt::black, 2);
    scene->addLine(margin, margin + plotHeight, margin + plotWidth, margin + plotHeight, axisPen);
    scene->addLine(margin, margin, margin, margin + plotHeight, axisPen);

    // Titre du graphique
    QString titre = "Puissance Totale";
    QGraphicsTextItem *title = scene->addText(titre);
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(QColor(231, 76, 60));
    title->setPos(width/2 - title->boundingRect().width()/2, 5);

    // Label Y (puissance)
    QGraphicsTextItem *yLabel = scene->addText("Puissance (MW)");
    QFont labelFont = yLabel->font();
    labelFont.setPointSize(10);
    labelFont.setBold(true);
    yLabel->setFont(labelFont);
    yLabel->setRotation(-90);
    yLabel->setPos(5, margin + plotHeight/2 + yLabel->boundingRect().width()/2);

    // Label X (temps)
    QGraphicsTextItem *xLabel = scene->addText("Temps (heures)");
    xLabel->setFont(labelFont);
    xLabel->setPos(width/2 - xLabel->boundingRect().width()/2, height - 15);

    // Graduations sur l'axe X
    QFont tickFont;
    tickFont.setPointSize(9);

    for (int i = 0; i <= 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin + plotHeight, x, margin + plotHeight + 8, axisPen);

        QDateTime temps = debut.addSecs(i * 3600);
        QString label = temps.toString("hh:mm");
        QGraphicsTextItem *text = scene->addText(label);
        text->setFont(tickFont);
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width()/2, margin + plotHeight + 10);
    }

    // Graduations sur l'axe Y
    for (int i = 0; i <= 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin - 8, y, margin, y, axisPen);

        double puissance = puissanceMin + (puissanceMax - puissanceMin) * i / 4.0;
        QString label = QString::number(puissance, 'f', 1);
        QGraphicsTextItem *text = scene->addText(label);
        text->setFont(tickFont);
        text->setDefaultTextColor(Qt::black);
        double textX = margin - text->boundingRect().width() - 10;
        double textY = y - text->boundingRect().height() / 2;
        text->setPos(textX, textY);
    }

    // Calculer et dessiner la courbe de puissance totale
    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    // Collecter tous les timestamps uniques
    std::set<QDateTime> allTimestamps;
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();
        for (const auto& mesure : historique) {
            if (mesure.timestamp >= debut) {
                allTimestamps.insert(mesure.timestamp);
            }
        }
    }

    // Pour chaque timestamp, calculer la puissance totale
    QPen curvePen(QColor(231, 76, 60), 3);  // Rouge
    QPointF lastPoint;
    bool first = true;
    double puissanceActuelle = 0;

    for (const QDateTime& timestamp : allTimestamps) {
        qint64 secondesDepuisDebut = debut.secsTo(timestamp);
        if (secondesDepuisDebut < 0 || secondesDepuisDebut > 6 * 3600) continue;

        double puissanceTotale = 0;

        // Sommer la puissance de toutes les turbines à ce timestamp
        for (size_t t = 0; t < turbines.size() && t < 5; t++) {
            std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();

            // Trouver la mesure la plus proche de ce timestamp
            for (const auto& mesure : historique) {
                if (mesure.timestamp == timestamp) {
                    float debit = mesure.valeur;
                    float puissance = 0;

                    // Calculer la puissance selon la turbine
                    switch(t) {
                    case 0: puissance = fonctionT1(debit, hc); break;
                    case 1: puissance = fonctionT2(debit, hc); break;
                    case 2: puissance = fonctionT3(debit, hc); break;
                    case 3: puissance = fonctionT4(debit, hc); break;
                    case 4: puissance = fonctionT5(debit, hc); break;
                    }

                    puissanceTotale += puissance;
                    break;
                }
            }
        }

        // Coordonnées du point
        double x = margin + (plotWidth * secondesDepuisDebut / (6.0 * 3600.0));

        if (puissanceTotale < puissanceMin) puissanceTotale = puissanceMin;
        if (puissanceTotale > puissanceMax) puissanceMax = puissanceTotale;

        double y = margin + plotHeight - ((puissanceTotale - puissanceMin) / (puissanceMax - puissanceMin) * plotHeight);

        QPointF currentPoint(x, y);

        if (!first) {
            scene->addLine(QLineF(lastPoint, currentPoint), curvePen);
        }

        // Dessiner un point
        QPen pointPen(QColor(192, 57, 43), 1);
        QBrush pointBrush(QColor(231, 76, 60));
        scene->addEllipse(x - 3, y - 3, 6, 6, pointPen, pointBrush);

        lastPoint = currentPoint;
        first = false;
        puissanceActuelle = puissanceTotale;
    }

    // Afficher la puissance actuelle
    if (puissanceActuelle > 0) {
        QString valeurText = QString("Actuel: %1 MW").arg(puissanceActuelle, 0, 'f', 2);
        QGraphicsTextItem *valeur = scene->addText(valeurText);
        QFont valeurFont = valeur->font();
        valeurFont.setPointSize(10);
        valeurFont.setBold(true);
        valeur->setFont(valeurFont);
        valeur->setDefaultTextColor(QColor(192, 57, 43));
        valeur->setPos(margin + 5, margin + 5);
    }

    scene->setSceneRect(0, 0, width, height);
    return scene;
}

QGraphicsScene* MainWindow1::creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur)
{
    QGraphicsScene* scene = new QGraphicsScene();

    // Dimensions du graphique
    const double width = 380;
    const double height = 280;
    const double margin = 70;  // Augmenté pour plus d'espace pour les valeurs Y
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    // Déterminer les plages de données
    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime debut = maintenant.addSecs(-6 * 3600);

    double debitMin = 100;
    double debitMax = 200;

    // Fond blanc pour la zone de tracé
    scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));

    // Grille
    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
    for (int i = 1; i < 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin, y, margin + plotWidth, y, gridPen);
    }
    for (int i = 1; i < 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin, x, margin + plotHeight, gridPen);
    }

    // Dessiner les axes
    QPen axisPen(Qt::black, 2);
    scene->addLine(margin, margin + plotHeight, margin + plotWidth, margin + plotHeight, axisPen);
    scene->addLine(margin, margin, margin, margin + plotHeight, axisPen);

    // Titre
    QString titre = QString("Turbine %1 - Débit sur 6h").arg(numeroTurbine);
    QGraphicsTextItem *title = scene->addText(titre);
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(couleur);
    title->setPos(width/2 - title->boundingRect().width()/2, 5);

    // Labels
    QGraphicsTextItem *yLabel = scene->addText("Débit (m³/s)");
    QFont labelFont = yLabel->font();
    labelFont.setPointSize(10);
    labelFont.setBold(true);
    yLabel->setFont(labelFont);
    yLabel->setRotation(-90);
    yLabel->setPos(5, margin + plotHeight/2 + yLabel->boundingRect().width()/2);

    QGraphicsTextItem *xLabel = scene->addText("Temps (heures)");
    xLabel->setFont(labelFont);
    xLabel->setPos(width/2 - xLabel->boundingRect().width()/2, height - 15);

    // Graduations X
    QFont tickFont;
    tickFont.setPointSize(9);

    for (int i = 0; i <= 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin + plotHeight, x, margin + plotHeight + 8, axisPen);

        QDateTime temps = debut.addSecs(i * 3600);
        QString label = temps.toString("hh:mm");
        QGraphicsTextItem *text = scene->addText(label);
        text->setFont(tickFont);
        text->setDefaultTextColor(Qt::black);
        text->setPos(x - text->boundingRect().width()/2, margin + plotHeight + 10);
    }

    // Graduations Y
    for (int i = 0; i <= 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin - 8, y, margin, y, axisPen);

        double debit = debitMin + (debitMax - debitMin) * i / 4.0;
        QString label = QString::number(debit, 'f', 0);
        QGraphicsTextItem *text = scene->addText(label);
        text->setFont(tickFont);
        text->setDefaultTextColor(Qt::black);
        double textX = margin - text->boundingRect().width() - 10;
        double textY = y - text->boundingRect().height() / 2;
        text->setPos(textX, textY);
    }

    // Dessiner la courbe
    QPen curvePen(couleur, 3);
    std::vector<MesureHistorique> historique = turbine->getCapteur().getHistorique();

    if (!historique.empty()) {
        QPointF lastPoint;
        bool first = true;

        for (const auto& mesure : historique) {
            if (mesure.timestamp < debut) continue;

            qint64 secondesDepuisDebut = debut.secsTo(mesure.timestamp);
            if (secondesDepuisDebut < 0 || secondesDepuisDebut > 6 * 3600) continue;

            double x = margin + (plotWidth * secondesDepuisDebut / (6.0 * 3600.0));

            double debit = mesure.valeur;
            if (debit < debitMin) debit = debitMin;
            if (debit > debitMax) debit = debitMax;

            double y = margin + plotHeight - ((debit - debitMin) / (debitMax - debitMin) * plotHeight);

            QPointF currentPoint(x, y);

            if (!first) {
                scene->addLine(QLineF(lastPoint, currentPoint), curvePen);
            }

            QPen pointPen(couleur, 1);
            QBrush pointBrush(couleur);
            scene->addEllipse(x - 3, y - 3, 6, 6, pointPen, pointBrush);

            lastPoint = currentPoint;
            first = false;
        }

        if (!historique.empty()) {
            double valeurActuelle = historique.back().valeur;
            QString valeurText = QString("Actuel: %1 m³/s").arg(valeurActuelle, 0, 'f', 1);
            QGraphicsTextItem *valeur = scene->addText(valeurText);
            QFont valeurFont = valeur->font();
            valeurFont.setPointSize(10);
            valeurFont.setBold(true);
            valeur->setFont(valeurFont);
            valeur->setDefaultTextColor(couleur);
            valeur->setPos(margin + 5, margin + 5);
        }
    }

    scene->setSceneRect(0, 0, width, height);
    return scene;
}

// Fonctions de production
float MainWindow1::fonctionT1(float du, float hc)
{
    float p00 = 1.102, p10 = -0.03187, p01 = -0.04866, p11 = 0.003308;
    float p02 = 0.002182, p12 = 3.638 * pow(10, -5), p03 = -1.277 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT2(float du, float hc)
{
    float p00 = -1.382, p10 = 0.09969, p01 = -1.945, p11 = 0.09224, p20 = -0.001724;
    float p02 = 0.007721, p12 = -6.622 * pow(10, -5), p21 = -0.001096, p03 = -1.933 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) + p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT3(float du, float hc)
{
    float p00 = 0.7799, p10 = -0.02261, p01 = 0.1995, p11 = -0.001695;
    float p02 = -3.519 * pow(10, -5), p12 = 7.235 * pow(10, -5), p03 = -9.338 * pow(10, -6);
    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT4(float du, float hc)
{
    float p00 = 20.22, p10 = -0.5777, p01 = -0.4586, p11 = 0.01151;
    float p02 = 0.004886, p12 = 1.379 * pow(10, -5), p03 = -1.882 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*du*hc + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT5(float du, float hc)
{
    float p00 = -212.1, p10 = 12.17, p01 = 0.004397, p11 = -0.006808, p20 = -0.1746;
    float p02 = 0.004529, p12 = -4.211*pow(10, -5), p21 = 0.0002936, p03 = -1.176*pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) + p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}
