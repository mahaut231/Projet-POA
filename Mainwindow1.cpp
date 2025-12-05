#include "Mainwindow1.h"
#include "ui_Mainwindow1.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QDebug>
#include <QPen>
#include <QTimer>
#include <cmath>

MainWindow1::MainWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(nullptr)
    , timer(new QTimer(this))
{
    ui->setupUi(this);
}

MainWindow1::MainWindow1(Centrale* centrale, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(centrale)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    // Initialiser les données historiques (simuler 6 heures de données)
    initialiserDonneesHistoriques();

    // Afficher le graphique initial
    afficherGraphiquesDebits();

    // Configurer le timer pour mise à jour toutes les 15 minutes
    connect(timer, &QTimer::timeout, this, &MainWindow1::mettreAJourGraphique);
    timer->start(900000); // 15 minutes en millisecondes
}

MainWindow1::~MainWindow1()
{
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
}

void MainWindow1::afficherGraphiquesDebits()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible";
        return;
    }

    // Créer la scène et la vue
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // Récupérer les turbines
    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    // Dimensions pour chaque graphique (format plus carré)
    const double graphWidth = 380;
    const double graphHeight = 300;
    const double margin = 60;
    const double horizontalSpace = 30;
    const double verticalSpace = 40;
    const double topMargin = 80;

    // Dimensions totales pour une grille 3 colonnes x 2 lignes
    const double totalWidth = 3 * graphWidth + 2 * horizontalSpace + 2 * margin;
    const double totalHeight = 2 * graphHeight + verticalSpace + topMargin + margin;

    // Déterminer les plages de données
    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime debut = maintenant.addSecs(-6 * 3600); // 6 heures en arrière

    double debitMin = 100;
    double debitMax = 200;

    // Couleurs pour chaque turbine
    QList<QColor> colors = {
        QColor(255, 0, 0),      // Rouge - Turbine 1
        QColor(0, 0, 255),      // Bleu - Turbine 2
        QColor(0, 200, 0),      // Vert - Turbine 3
        QColor(255, 165, 0),    // Orange - Turbine 4
        QColor(128, 0, 128)     // Violet - Turbine 5
    };

    // Titre principal
    QGraphicsTextItem *mainTitle = scene->addText("Évolution des Débits des Turbines (6 dernières heures)");
    QFont titleFont = mainTitle->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    mainTitle->setFont(titleFont);
    mainTitle->setPos(totalWidth/2 - mainTitle->boundingRect().width()/2, 10);

    // Dessiner les graphiques en grille 3x2
    // Ligne 1: Turbines 1, 2, 3
    // Ligne 2: Turbines 4, 5, (espace réservé)
    for (int t = 0; t < nbTurbines; t++) {
        int row = t / 3;  // 0 pour turbines 1-3, 1 pour turbines 4-5
        int col = t % 3;  // 0, 1, 2

        double xOffset = margin + col * (graphWidth + horizontalSpace);
        double yOffset = topMargin + row * (graphHeight + verticalSpace);

        dessinerGraphiqueTurbine(scene, turbines[t], t + 1, colors[t],
                                 xOffset, yOffset, graphWidth, graphHeight, margin,
                                 debut, maintenant, debitMin, debitMax);
    }

    // Configurer la scène
    scene->setSceneRect(0, 0, totalWidth, totalHeight);
    view->setScene(scene);

    // Créer un widget central avec layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(view);

    setCentralWidget(centralWidget);

    // Ajuster la taille de la fenêtre
    resize(totalWidth + 100, totalHeight + 100);
}

void MainWindow1::dessinerGraphiqueTurbine(QGraphicsScene* scene, Turbine* turbine,
                                           int numeroTurbine, QColor couleur,
                                           double xOffset, double yOffset,
                                           double width, double height, double margin,
                                           QDateTime debut, QDateTime fin,
                                           double debitMin, double debitMax)
{
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    // Dessiner le cadre
    QPen framePen(Qt::gray, 1);
    scene->addRect(xOffset + margin, yOffset + margin, plotWidth, plotHeight, framePen);

    // Dessiner les axes
    QPen axisPen(Qt::black, 2);
    scene->addLine(xOffset + margin, yOffset + margin + plotHeight,
                   xOffset + margin + plotWidth, yOffset + margin + plotHeight, axisPen); // Axe X
    scene->addLine(xOffset + margin, yOffset + margin,
                   xOffset + margin, yOffset + margin + plotHeight, axisPen); // Axe Y

    // Titre du graphique
    QString titre = QString("Turbine %1").arg(numeroTurbine);
    QGraphicsTextItem *title = scene->addText(titre);
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(couleur);
    title->setPos(xOffset + width/2 - title->boundingRect().width()/2, yOffset + 5);

    // Label Y (débit)
    QGraphicsTextItem *yLabel = scene->addText("Débit (m³/s)");
    QFont labelFont = yLabel->font();
    labelFont.setPointSize(8);
    yLabel->setFont(labelFont);
    yLabel->setRotation(-90);
    yLabel->setPos(xOffset + 10, yOffset + margin + plotHeight/2 + yLabel->boundingRect().width()/2);

    // Label X (temps) - seulement pour la ligne du bas
    if (numeroTurbine >= 4) {
        QGraphicsTextItem *xLabel = scene->addText("Temps (heures)");
        QFont xLabelFont = xLabel->font();
        xLabelFont.setPointSize(8);
        xLabel->setFont(xLabelFont);
        xLabel->setPos(xOffset + width/2 - xLabel->boundingRect().width()/2,
                       yOffset + height - 15);
    }

    // Graduations sur l'axe X (temps)
    for (int i = 0; i <= 6; i += 2) { // Toutes les 2 heures
        double x = xOffset + margin + (plotWidth * i / 6.0);
        scene->addLine(x, yOffset + margin + plotHeight,
                       x, yOffset + margin + plotHeight + 5, axisPen);

        QDateTime temps = debut.addSecs(i * 3600);
        QString label = temps.toString("hh:mm");
        QGraphicsTextItem *text = scene->addText(label);
        QFont textFont = text->font();
        textFont.setPointSize(7);
        text->setFont(textFont);
        text->setPos(x - text->boundingRect().width()/2, yOffset + margin + plotHeight + 8);
    }

    // Graduations sur l'axe Y (débit)
    for (int i = 0; i <= 4; i++) {
        double y = yOffset + margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(xOffset + margin - 5, y, xOffset + margin, y, axisPen);

        double debit = debitMin + (debitMax - debitMin) * i / 4.0;
        QString label = QString::number(debit, 'f', 0);
        QGraphicsTextItem *text = scene->addText(label);
        QFont textFont = text->font();
        textFont.setPointSize(7);
        text->setFont(textFont);
        text->setPos(xOffset + margin - text->boundingRect().width() - 10,
                     y - text->boundingRect().height()/2);
    }

    // Dessiner la courbe
    QPen pen(couleur, 2);
    std::vector<MesureHistorique> historique = turbine->getCapteur().getHistorique();

    if (historique.empty()) return;

    QPointF lastPoint;
    bool first = true;
    int pointCount = 0;

    for (const auto& mesure : historique) {
        if (mesure.timestamp < debut) continue;

        qint64 secondesDepuisDebut = debut.secsTo(mesure.timestamp);
        if (secondesDepuisDebut < 0 || secondesDepuisDebut > 6 * 3600) continue;

        double x = xOffset + margin + (plotWidth * secondesDepuisDebut / (6.0 * 3600.0));

        double debit = mesure.valeur;
        if (debit < debitMin) debit = debitMin;
        if (debit > debitMax) debit = debitMax;

        double y = yOffset + margin + plotHeight - ((debit - debitMin) / (debitMax - debitMin) * plotHeight);

        QPointF currentPoint(x, y);

        if (!first) {
            scene->addLine(QLineF(lastPoint, currentPoint), pen);
        }

        // Dessiner un petit point
        scene->addEllipse(x - 2, y - 2, 4, 4, pen, QBrush(couleur));

        lastPoint = currentPoint;
        first = false;
        pointCount++;
    }

    qDebug() << "Turbine" << numeroTurbine << ":" << pointCount << "points affichés";
}

// Fonctions de production pour chaque turbine (conservées pour compatibilité)
float MainWindow1::fonctionT1(float du, float hc)
{
    float p00 = 1.102;
    float p10 = -0.03187;
    float p01 = -0.04866;
    float p11 = 0.003308;
    float p02 = 0.002182;
    float p12 = 3.638 * pow(10, -5);
    float p03 = -1.277 * pow(10, -5);

    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) +
           p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT2(float du, float hc)
{
    float p00 = -1.382;
    float p10 = 0.09969;
    float p01 = -1.945;
    float p11 = 0.09224;
    float p20 = -0.001724;
    float p02 = 0.007721;
    float p12 = -6.622 * pow(10, -5);
    float p21 = -0.001096;
    float p03 = -1.933 * pow(10, -5);

    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) +
           p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) +
           p03*pow(du, 3);
}

float MainWindow1::fonctionT3(float du, float hc)
{
    float p00 = 0.7799;
    float p10 = -0.02261;
    float p01 = 0.1995;
    float p11 = -0.001695;
    float p02 = -3.519 * pow(10, -5);
    float p12 = 7.235 * pow(10, -5);
    float p03 = -9.338 * pow(10, -6);

    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) +
           p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT4(float du, float hc)
{
    float p00 = 20.22;
    float p10 = -0.5777;
    float p01 = -0.4586;
    float p11 = 0.01151;
    float p02 = 0.004886;
    float p12 = 1.379 * pow(10, -5);
    float p03 = -1.882 * pow(10, -5);

    return p00 + p10*hc + p01*du + p11*du*hc + p02*pow(du, 2) +
           p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow1::fonctionT5(float du, float hc)
{
    float p00 = -212.1;
    float p10 = 12.17;
    float p01 = 0.004397;
    float p11 = -0.006808;
    float p20 = -0.1746;
    float p02 = 0.004529;
    float p12 = -4.211 * pow(10, -5);
    float p21 = 0.0002936;
    float p03 = -1.176 * pow(10, -5);

    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) +
           p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) +
           p03*pow(du, 3);
}
