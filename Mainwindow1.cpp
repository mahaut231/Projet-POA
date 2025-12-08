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
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <cmath>

MainWindow1::MainWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(nullptr)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
    , boutonChangerDebit(nullptr)
    , boutonDeconnexion(nullptr)

{
    ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        champsDebit[i] = nullptr;
        labelsDebit[i] = nullptr;
    }
}

MainWindow1::MainWindow1(Centrale* centrale, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow1)
    , centraleActuelle(centrale)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
    , boutonChangerDebit(nullptr)
    , boutonDeconnexion(nullptr)

{
    ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        champsDebit[i] = nullptr;
        labelsDebit[i] = nullptr;
    }

    resize(1300, 900);

    labelCentrale = new QLabel("Centrale 1", ui->centralwidget);
    QFont labelFont;
    labelFont.setPointSize(28);
    labelFont.setBold(true);
    labelCentrale->setFont(labelFont);
    labelCentrale->setAlignment(Qt::AlignCenter);
    labelCentrale->setStyleSheet("QLabel { color: white; background-color: transparent; }");
    labelCentrale->setGeometry(0, 10, 1300, 50);
    labelCentrale->show();

    initialiserDonneesHistoriques();

    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();

    creerChampsDebit();

    creerBoutonChangerDebit();
    creerBoutonDeconnexion();

    connect(timer, &QTimer::timeout, this, &MainWindow1::mettreAJourGraphique);
    timer->start(900000); // 15 minutes en millisecondes
}

MainWindow1::~MainWindow1()
{
    for (auto view : vuesGraphiques) {
        delete view;
    }
    if (vuePuissance) {
        delete vuePuissance;
    }
    if (labelCentrale) {
        delete labelCentrale;
    }
    if (boutonChangerDebit) {
        delete boutonChangerDebit;
    }
    if (boutonDeconnexion) {
        delete boutonDeconnexion;
    }
    for (int i = 0; i < 5; i++) {
        if (champsDebit[i]) delete champsDebit[i];
        if (labelsDebit[i]) delete labelsDebit[i];
    }
    delete ui;
}

void MainWindow1::initialiserDonneesHistoriques()
{
    if (!centraleActuelle) return;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        turbines[t]->getCapteur().viderHistorique();
    }

    QDateTime maintenant = QDateTime::currentDateTime();

    for (int i = 23; i >= 0; i--) {
        QDateTime timestamp = maintenant.addSecs(-i * 15 * 60);

        for (size_t t = 0; t < turbines.size() && t < 5; t++) {
            Turbine* turbine = turbines[t];
            long debitBase = 140 + (t * 5);
            long variation = (rand() % 21) - 10;
            long debit = debitBase + variation;
            turbine->getCapteur().ajouterMesure(debit, timestamp);
        }
    }
}

void MainWindow1::mettreAJourGraphique()
{
    if (!centraleActuelle) return;

    QDateTime maintenant = QDateTime::currentDateTime();
    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        Turbine* turbine = turbines[t];
        long debit = turbine->getdebits();
        turbine->getCapteur().ajouterMesure(debit, maintenant);
    }

    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();
}

void MainWindow1::afficherGraphiquesDebits()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible";
        return;
    }

    for (auto view : vuesGraphiques) {
        delete view;
    }
    vuesGraphiques.clear();

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    const int graphWidth = 400;
    const int graphHeight = 300;
    const int horizontalSpace = 20;
    const int verticalSpace = 70;
    const int startX = 20;
    const int startY = 70;

    QList<QColor> colors = {
        QColor(255, 20, 147),
        QColor(0, 0, 255),
        QColor(0, 200, 0),
        QColor(255, 165, 0),
        QColor(128, 0, 128)
    };

    for (int t = 0; t < nbTurbines; t++) {
        int row = t / 3;
        int col = t % 3;

        int x = startX + col * (graphWidth + horizontalSpace);
        int y = startY + row * (graphHeight + verticalSpace);

        QGraphicsScene* scene = creerGraphiqueTurbine(turbines[t], t + 1, colors[t]);
        QGraphicsView* view = new QGraphicsView(scene, ui->centralwidget);
        view->setRenderHint(QPainter::Antialiasing);
        view->setGeometry(x, y, graphWidth, graphHeight);
        view->setStyleSheet("border: 2px solid gray; background-color: white;");
        view->show();

        vuesGraphiques.append(view);
    }
}

void MainWindow1::afficherGraphiquePuissanceTotale()
{
    if (!centraleActuelle) {
        qDebug() << "Pas de centrale disponible pour puissance totale";
        return;
    }

    if (vuePuissance) {
        delete vuePuissance;
        vuePuissance = nullptr;
    }

    QGraphicsScene* scene = creerGraphiquePuissanceTotale();

    const int startX = 20;
    const int startY = 70;
    const int graphWidth = 400;
    const int graphHeight = 300;
    const int horizontalSpace = 20;
    const int verticalSpace = 70;

    int x = startX + 2 * (graphWidth + horizontalSpace);
    int y = startY + 1 * (graphHeight + verticalSpace);

    vuePuissance = new QGraphicsView(scene, ui->centralwidget);
    vuePuissance->setRenderHint(QPainter::Antialiasing);
    vuePuissance->setGeometry(x, y, graphWidth, graphHeight);
    vuePuissance->setStyleSheet("border: 2px solid gray; background-color: white;");
    vuePuissance->show();
}

void MainWindow1::creerChampsDebit()
{
    if (!centraleActuelle) return;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    const int graphWidth = 400;
    const int graphHeight = 300;
    const int horizontalSpace = 20;
    const int verticalSpace = 70;
    const int startX = 20;
    const int startY = 70;
    const int inputHeight = 30;
    const int inputYOffset = 10;

    for (int t = 0; t < nbTurbines; t++) {
        int row = t / 3;
        int col = t % 3;

        int x = startX + col * (graphWidth + horizontalSpace);
        int y = startY + row * (graphHeight + verticalSpace) + graphHeight + inputYOffset;

        labelsDebit[t] = new QLabel(QString("Débit T%1 (m³/s):").arg(t+1), ui->centralwidget);
        labelsDebit[t]->setGeometry(x + 40, y, 120, inputHeight);
        labelsDebit[t]->setStyleSheet("QLabel { color: white; font-size: 11px; font-weight: bold; }");
        labelsDebit[t]->show();

        champsDebit[t] = new QLineEdit(ui->centralwidget);
        champsDebit[t]->setGeometry(x + 165, y, 100, inputHeight);
        champsDebit[t]->setPlaceholderText("");
        champsDebit[t]->setStyleSheet(
            "QLineEdit { "
            "background-color: black; "
            "color: white; "
            "border: 2px solid #3498db; "
            "border-radius: 5px; "
            "padding: 5px; "
            "font-size: 12px; "
            "font-weight: bold; "
            "}"
            "QLineEdit:focus { "
            "border: 2px solid #2ecc71; "
            "}"
            "QLineEdit::placeholder { "
            "color: #7f8c8d; "
            "}"
            );
        champsDebit[t]->show();
    }
}

void MainWindow1::creerBoutonChangerDebit()
{
    boutonChangerDebit = new QPushButton("Changer débit des turbines", ui->centralwidget);
    boutonChangerDebit->setGeometry(500, 790, 300, 50);
    boutonChangerDebit->setStyleSheet(
        "QPushButton { "
        "background-color: #3498db; "
        "color: white; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "padding: 10px; "
        "}"
        "QPushButton:hover { "
        "background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "background-color: #21618c; "
        "}"
        );
    boutonChangerDebit->show();

    connect(boutonChangerDebit, &QPushButton::clicked, this, &MainWindow1::appliquerChangementsDebit);
}

void MainWindow1::appliquerChangementsDebit()
{
    if (!centraleActuelle) return;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    QDateTime maintenant = QDateTime::currentDateTime();
    bool changementEffectue = false;
    QString messagesErreur;
    bool auMoinsUnChampRempli = false;

    // D'abord, vérifier s'il y a au moins un champ rempli
    for (int t = 0; t < nbTurbines; t++) {
        if (champsDebit[t] && !champsDebit[t]->text().trimmed().isEmpty()) {
            auMoinsUnChampRempli = true;
            break;
        }
    }

    // Si aucun champ n'est rempli, ne rien faire
    if (!auMoinsUnChampRempli) {
        return;
    }

    // AVANT toute modification - DIAGNOSTIC
    qDebug() << "=== AVANT MODIFICATIONS ===";
    for (int t = 0; t < nbTurbines; t++) {
        qDebug() << "Turbine" << (t+1) << "débit:" << turbines[t]->getdebits();
    }

    // Valider et appliquer les changements
    for (int t = 0; t < nbTurbines; t++) {
        if (!champsDebit[t]) continue;

        QString texte = champsDebit[t]->text().trimmed();

        if (texte.isEmpty()) {
            qDebug() << "Turbine" << (t+1) << ": AUCUN changement (champ vide)";
            continue;
        }

        bool ok;
        long nouveauDebit = texte.toLong(&ok);

        if (!ok) {
            messagesErreur += QString("Turbine %1: Valeur invalide\n").arg(t + 1);
            continue;
        }

        Turbine* turbine = turbines[t];
        long debitMin = turbine->getpMin();
        long debitMax = turbine->getpMax();

        if (nouveauDebit < debitMin || nouveauDebit > debitMax) {
            messagesErreur += QString("Turbine %1: Débit hors plage (%2-%3)\n")
                                  .arg(t + 1).arg(debitMin).arg(debitMax);
            continue;
        }

        qDebug() << "Turbine" << (t+1) << ": CHANGEMENT"
                 << turbine->getdebits() << "→" << nouveauDebit;

        // Appliquer le nouveau débit
        turbine->setdebits(nouveauDebit);
        champsDebit[t]->clear();
        champsDebit[t]->setPlaceholderText(QString("").arg(nouveauDebit));

        changementEffectue = true;
    }

    // APRÈS modifications - DIAGNOSTIC
    qDebug() << "=== APRÈS MODIFICATIONS (avant ajout mesures) ===";
    for (int t = 0; t < nbTurbines; t++) {
        qDebug() << "Turbine" << (t+1) << "débit:" << turbines[t]->getdebits();
    }

    // Ajouter une mesure pour TOUTES les turbines au même instant
    qDebug() << "=== AJOUT MESURES ===";
    for (int t = 0; t < nbTurbines; t++) {
        Turbine* turbine = turbines[t];
        long debitActuel = turbine->getdebits();
        qDebug() << "Ajout mesure Turbine" << (t+1) << ":" << debitActuel;
        turbine->getCapteur().ajouterMesure(debitActuel, maintenant);
    }

    if (!messagesErreur.isEmpty()) {
        QMessageBox::warning(this, "Erreurs de validation", messagesErreur);
    }

    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();

    if (changementEffectue) {
        QMessageBox::information(this, "Succès", "Les débits ont été mis à jour avec succès !");
    }
}

QGraphicsScene* MainWindow1::creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur)
{
    QGraphicsScene* scene = new QGraphicsScene();

    const double width = 380;
    const double height = 280;
    const double margin = 70;
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime debut = maintenant.addSecs(-6 * 3600);

    std::vector<MesureHistorique> historique = turbine->getCapteur().getHistorique();
    double debitMin = turbine->getpMin();
    double debitMax = turbine->getpMax();

    if (!historique.empty()) {
        double minObserve = debitMax;
        double maxObserve = debitMin;

        for (const auto& mesure : historique) {
            if (mesure.timestamp >= debut) {
                if (mesure.valeur < minObserve) minObserve = mesure.valeur;
                if (mesure.valeur > maxObserve) maxObserve = mesure.valeur;
            }
        }

        if (minObserve < debitMax && maxObserve > debitMin) {
            double marge = (maxObserve - minObserve) * 0.1;
            debitMin = std::max((double)turbine->getpMin(), minObserve - marge);
            debitMax = std::min((double)turbine->getpMax(), maxObserve + marge);
        }
    }

    scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));

    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
    for (int i = 1; i < 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin, y, margin + plotWidth, y, gridPen);
    }
    for (int i = 1; i < 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin, x, margin + plotHeight, gridPen);
    }

    QPen axisPen(Qt::black, 2);
    scene->addLine(margin, margin + plotHeight, margin + plotWidth, margin + plotHeight, axisPen);
    scene->addLine(margin, margin, margin, margin + plotHeight, axisPen);

    QString titre = QString("Turbine %1 - Débit sur 6h").arg(numeroTurbine);
    QGraphicsTextItem *title = scene->addText(titre);
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(couleur);
    title->setPos(width/2 - title->boundingRect().width()/2, 5);

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

    QPen curvePen(couleur, 3);

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

QGraphicsScene* MainWindow1::creerGraphiquePuissanceTotale()
{
    QGraphicsScene* scene = new QGraphicsScene();

    const double width = 380;
    const double height = 280;
    const double margin = 70;
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime debut = maintenant.addSecs(-6 * 3600);

    float hc = 35.0;

    double puissanceMin = 0;
    double puissanceMax = 100;

    scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));

    QPen gridPen(Qt::lightGray, 1, Qt::DotLine);
    for (int i = 1; i < 4; i++) {
        double y = margin + plotHeight - (plotHeight * i / 4.0);
        scene->addLine(margin, y, margin + plotWidth, y, gridPen);
    }
    for (int i = 1; i < 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin, x, margin + plotHeight, gridPen);
    }

    QPen axisPen(Qt::black, 2);
    scene->addLine(margin, margin + plotHeight, margin + plotWidth, margin + plotHeight, axisPen);
    scene->addLine(margin, margin, margin, margin + plotHeight, axisPen);

    QString titre = "Puissance Totale";
    QGraphicsTextItem *title = scene->addText(titre);
    QFont titleFont = title->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(QColor(231, 76, 60));
    title->setPos(width/2 - title->boundingRect().width()/2, 5);

    QGraphicsTextItem *yLabel = scene->addText("Puissance (MW)");
    QFont labelFont = yLabel->font();
    labelFont.setPointSize(10);
    labelFont.setBold(true);
    yLabel->setFont(labelFont);
    yLabel->setRotation(-90);
    yLabel->setPos(5, margin + plotHeight/2 + yLabel->boundingRect().width()/2);

    QGraphicsTextItem *xLabel = scene->addText("Temps (heures)");
    xLabel->setFont(labelFont);
    xLabel->setPos(width/2 - xLabel->boundingRect().width()/2, height - 15);

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

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    std::set<QDateTime> allTimestamps;
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();
        for (const auto& mesure : historique) {
            if (mesure.timestamp >= debut) {
                allTimestamps.insert(mesure.timestamp);
            }
        }
    }

    QPen curvePen(QColor(231, 76, 60), 3);
    QPointF lastPoint;
    bool first = true;
    double puissanceActuelle = 0;

    for (const QDateTime& timestamp : allTimestamps) {
        qint64 secondesDepuisDebut = debut.secsTo(timestamp);
        if (secondesDepuisDebut < 0 || secondesDepuisDebut > 6 * 3600) continue;

        double puissanceTotale = 0;

        for (size_t t = 0; t < turbines.size() && t < 5; t++) {
            std::vector<MesureHistorique> historique = turbines[t]->getCapteur().getHistorique();

            for (const auto& mesure : historique) {
                if (mesure.timestamp == timestamp) {
                    float debit = mesure.valeur;
                    float puissance = 0;

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

        double x = margin + (plotWidth * secondesDepuisDebut / (6.0 * 3600.0));

        if (puissanceTotale < puissanceMin) puissanceTotale = puissanceMin;
        if (puissanceTotale > puissanceMax) puissanceMax = puissanceTotale;

        double y = margin + plotHeight - ((puissanceTotale - puissanceMin) / (puissanceMax - puissanceMin) * plotHeight);

        QPointF currentPoint(x, y);

        if (!first) {
            scene->addLine(QLineF(lastPoint, currentPoint), curvePen);
        }

        QPen pointPen(QColor(192, 57, 43), 1);
        QBrush pointBrush(QColor(231, 76, 60));
        scene->addEllipse(x - 3, y - 3, 6, 6, pointPen, pointBrush);

        lastPoint = currentPoint;
        first = false;
        puissanceActuelle = puissanceTotale;
    }

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

void MainWindow1::creerBoutonDeconnexion()
{
    boutonDeconnexion = new QPushButton("Se déconnecter", ui->centralwidget);
    boutonDeconnexion->setGeometry(850, 790, 200, 50);
    boutonDeconnexion->setStyleSheet(
        "QPushButton { "
        "background-color: #e74c3c; "
        "color: white; "
        "border: none; "
        "border-radius: 8px; "
        "font-size: 14px; "
        "font-weight: bold; "
        "padding: 10px; "
        "}"
        "QPushButton:hover { "
        "background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "background-color: #a93226; "
        "}"
        );
    boutonDeconnexion->show();

    connect(boutonDeconnexion, &QPushButton::clicked, this, &MainWindow1::deconnecter);
}

void MainWindow1::deconnecter()
{
    qDebug() << "Déconnexion demandée depuis MainWindow1";
    emit deconnexionDemandee();
    close();
}
