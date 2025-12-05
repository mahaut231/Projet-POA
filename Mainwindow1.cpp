#include "Mainwindow1.h"
#include "ui_Mainwindow1.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QDebug>
#include <QPen>
#include <cmath>

MainWindow1::MainWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(nullptr)
{
    ui->setupUi(this);
}

MainWindow1::MainWindow1(Centrale* centrale, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(centrale)
{
    ui->setupUi(this);
    afficherGraphiquesTurbines();
}

MainWindow1::~MainWindow1()
{
    delete ui;
}

void MainWindow1::afficherGraphiquesTurbines()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible";
        return;
    }

    // Créer la scène et la vue
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // Dimensions du graphique
    const double width = 800;
    const double height = 600;
    const double margin = 60;
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    // Plages de données
    const double xMin = 0, xMax = 160;
    const double yMin = 0, yMax = 25;

    // Dessiner les axes
    QPen axisPen(Qt::black, 2);
    scene->addLine(margin, height - margin, width - margin, height - margin, axisPen); // Axe X
    scene->addLine(margin, margin, margin, height - margin, axisPen); // Axe Y

    // Titre du graphique
    QGraphicsTextItem *title = scene->addText("Fonctions de Production des 5 Turbines");
    QFont titleFont = title->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setPos(width/2 - title->boundingRect().width()/2, 10);

    // Labels des axes
    QGraphicsTextItem *xLabel = scene->addText("Débit unitaire (m³/s)");
    xLabel->setPos(width/2 - xLabel->boundingRect().width()/2, height - 20);

    QGraphicsTextItem *yLabel = scene->addText("Production (MW)");
    yLabel->setRotation(-90);
    yLabel->setPos(10, height/2 + yLabel->boundingRect().width()/2);

    // Graduations sur l'axe X
    for (int i = 0; i <= 8; i++) {
        double x = margin + (plotWidth * i / 8.0);
        scene->addLine(x, height - margin, x, height - margin + 5, axisPen);
        QString label = QString::number(xMin + (xMax - xMin) * i / 8.0, 'f', 0);
        QGraphicsTextItem *text = scene->addText(label);
        text->setPos(x - text->boundingRect().width()/2, height - margin + 10);
    }

    // Graduations sur l'axe Y
    for (int i = 0; i <= 5; i++) {
        double y = height - margin - (plotHeight * i / 5.0);
        scene->addLine(margin - 5, y, margin, y, axisPen);
        QString label = QString::number(yMin + (yMax - yMin) * i / 5.0, 'f', 1);
        QGraphicsTextItem *text = scene->addText(label);
        text->setPos(margin - text->boundingRect().width() - 10, y - text->boundingRect().height()/2);
    }

    // Couleurs pour chaque turbine
    QList<QColor> colors = {
        QColor(255, 0, 0),      // Rouge - Turbine 1
        QColor(0, 0, 255),      // Bleu - Turbine 2
        QColor(0, 200, 0),      // Vert - Turbine 3
        QColor(255, 165, 0),    // Orange - Turbine 4
        QColor(128, 0, 128)     // Violet - Turbine 5
    };

    QStringList names = {"Turbine 1", "Turbine 2", "Turbine 3", "Turbine 4", "Turbine 5"};

    float hc = 35.0; // Hauteur de chute

    // Dessiner les courbes pour chaque turbine
    for (int turbine = 1; turbine <= 5; turbine++) {
        QPen pen(colors[turbine - 1], 2);

        QPointF lastPoint;
        bool first = true;

        for (float debit = 0; debit <= 160; debit += 2.0) {
            float production = 0;

            // Calculer la production selon la turbine
            switch(turbine) {
            case 1: production = fonctionT1(debit, hc); break;
            case 2: production = fonctionT2(debit, hc); break;
            case 3: production = fonctionT3(debit, hc); break;
            case 4: production = fonctionT4(debit, hc); break;
            case 5: production = fonctionT5(debit, hc); break;
            }

            // Convertir en coordonnées d'écran
            double x = margin + (debit - xMin) / (xMax - xMin) * plotWidth;
            double y = height - margin - (production - yMin) / (yMax - yMin) * plotHeight;

            QPointF currentPoint(x, y);

            if (!first) {
                scene->addLine(QLineF(lastPoint, currentPoint), pen);
            }

            lastPoint = currentPoint;
            first = false;
        }
    }

    // Ajouter une légende
    double legendX = width - margin - 120;
    double legendY = margin + 20;

    for (int i = 0; i < 5; i++) {
        // Ligne colorée
        QPen pen(colors[i], 3);
        scene->addLine(legendX, legendY + i * 25, legendX + 30, legendY + i * 25, pen);

        // Texte
        QGraphicsTextItem *text = scene->addText(names[i]);
        text->setPos(legendX + 35, legendY + i * 25 - 10);
    }

    // Configurer la scène
    scene->setSceneRect(0, 0, width, height);
    view->setScene(scene);

    // Créer un widget central avec layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(view);

    setCentralWidget(centralWidget);

    // Ajuster la taille de la fenêtre
    resize(900, 700);
}

QLineSeries* MainWindow1::creerCourbeTurbine1() { return nullptr; }
QLineSeries* MainWindow1::creerCourbeTurbine2() { return nullptr; }
QLineSeries* MainWindow1::creerCourbeTurbine3() { return nullptr; }
QLineSeries* MainWindow1::creerCourbeTurbine4() { return nullptr; }
QLineSeries* MainWindow1::creerCourbeTurbine5() { return nullptr; }

// Fonctions de production pour chaque turbine
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
