#include "Mainwindow2.h"
#include "ui_Mainwindow2.h"
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
#include <map>


MainWindow2::MainWindow2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow2)
    , centraleActuelle(nullptr)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
    , boutonChangerDebit(nullptr)
    , vuePuissance(nullptr)
    , debitsModifies(false)

{
    ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        champsDebit[i] = nullptr;
        labelsDebit[i] = nullptr;
    }
}

MainWindow2::MainWindow2(Centrale* centrale, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow2)
    , centraleActuelle(centrale)
    , timer(new QTimer(this))
    , labelCentrale(nullptr)
    , boutonChangerDebit(nullptr)
    , vuePuissance(nullptr)
    , debitsModifies(false)
{
    ui->setupUi(this);

    for (int i = 0; i < 5; i++) {
        champsDebit[i] = nullptr;
        labelsDebit[i] = nullptr;
    }

    // DIAGNOSTIC
    if (!centrale) {
        qDebug() << "ERREUR CRITIQUE: centrale est NULL dans MainWindow2!";
        return;
    }

    std::vector<Turbine*> turbines = centrale->getListeTurbine();
    qDebug() << "\n=== INITIALISATION MAINWINDOW2 ===";
    qDebug() << "Nombre de turbines:" << turbines.size();
    for (size_t i = 0; i < turbines.size(); i++) {
        int nbMesures = turbines[i]->getCapteur().getHistorique().size();
        qDebug() << "  Turbine" << (i+1) << ":" << nbMesures << "mesures dans l'historique";
    }
    qDebug() << "===================================\n";

    bool donneesPresentes = false;
    for (size_t i = 0; i < turbines.size() && i < 5; i++) {
        if (!turbines[i]->getCapteur().getHistorique().empty()) {
            donneesPresentes = true;
            break;
        }
    }

    if (!donneesPresentes) {
        qDebug() << "ERREUR: Aucune donnée disponible, arrêt de l'initialisation";
        QMessageBox::warning(this, "Erreur", "Aucune donnée disponible pour MainWindow2");
        return;
    }

    resize(1300, 900);

    // Création du label
    try {
        labelCentrale = new QLabel("Centrale 2", ui->centralwidget);
        QFont labelFont;
        labelFont.setPointSize(28);
        labelFont.setBold(true);
        labelCentrale->setFont(labelFont);
        labelCentrale->setAlignment(Qt::AlignCenter);
        labelCentrale->setStyleSheet("QLabel { color: white; background-color: transparent; }");
        labelCentrale->setGeometry(0, 10, 1300, 50);
        labelCentrale->show();
    } catch (...) {
        qDebug() << "ERREUR lors de la création du label";
        return;
    }

    initialiserDonneesHistoriques();

    // Afficher les graphiques avec protection
    try {
        qDebug() << "Début afficherGraphiquesDebits()...";
        afficherGraphiquesDebits();
        qDebug() << "Fin afficherGraphiquesDebits()";

        qDebug() << "Début afficherGraphiquePuissanceTotale()...";
        afficherGraphiquePuissanceTotale();
        qDebug() << "Fin afficherGraphiquePuissanceTotale()";
    } catch (const std::exception& e) {
        qDebug() << "EXCEPTION lors de l'affichage des graphiques:" << e.what();
        return;
    } catch (...) {
        qDebug() << "EXCEPTION INCONNUE lors de l'affichage des graphiques";
        return;
    }

    creerChampsDebit();
    creerBoutonChangerDebit();

    connect(timer, &QTimer::timeout, this, &MainWindow2::mettreAJourGraphique);
    timer->start(900000);

    qDebug() << "=== FIN INITIALISATION MAINWINDOW2 ===\n";
}

MainWindow2::~MainWindow2()
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
    for (int i = 0; i < 5; i++) {
        if (champsDebit[i]) delete champsDebit[i];
        if (labelsDebit[i]) delete labelsDebit[i];
    }
    delete ui;
}

void MainWindow2::initialiserDonneesHistoriques()
{
    if (!centraleActuelle) {
        qDebug() << "ERREUR: Pas de centrale dans MainWindow2";
        return;
    }

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    // VÉRIFICATION CRITIQUE : Y a-t-il des données CSV ?
    bool donneesCsvPresentes = false;
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        if (!turbines[t]->getCapteur().getHistorique().empty()) {
            donneesCsvPresentes = true;
            break;
        }
    }

    if (!donneesCsvPresentes) {
        qDebug() << "ATTENTION: Aucune donnée CSV trouvée pour MainWindow2!";
        qDebug() << "MainWindow2 nécessite des données CSV pour fonctionner.";
        return;
    }

    // Vérifier que toutes les turbines ont assez de données
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        int nbMesures = turbines[t]->getCapteur().getHistorique().size();
        qDebug() << "MainWindow2 - Turbine" << (t+1) << ":" << nbMesures << "mesures disponibles";

        if (nbMesures < 24) {
            qDebug() << "ATTENTION: Turbine" << (t+1) << "n'a pas assez de données (minimum 24 requis)";
        }
    }
}

void MainWindow2::mettreAJourGraphique()
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

void MainWindow2::afficherGraphiquesDebits()
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

void MainWindow2::afficherGraphiquePuissanceTotale()
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

void MainWindow2::creerChampsDebit()
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

void MainWindow2::creerBoutonChangerDebit()
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

    connect(boutonChangerDebit, &QPushButton::clicked, this, &MainWindow2::appliquerChangementsDebit);
}

void MainWindow2::appliquerChangementsDebit()
{
    if (!centraleActuelle) return;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();
    int nbTurbines = std::min((int)turbines.size(), 5);

    bool changementEffectue = false;
    QString messagesErreur;
    bool auMoinsUnChampRempli = false;

    for (int t = 0; t < nbTurbines; t++) {
        if (champsDebit[t] && !champsDebit[t]->text().trimmed().isEmpty()) {
            auMoinsUnChampRempli = true;
            break;
        }
    }

    if (!auMoinsUnChampRempli) {
        return;
    }

    for (int t = 0; t < nbTurbines; t++) {
        if (!champsDebit[t]) continue;

        QString texte = champsDebit[t]->text().trimmed();
        if (texte.isEmpty()) {
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


        turbine->setdebits(nouveauDebit);
        champsDebit[t]->clear();
        champsDebit[t]->setPlaceholderText(QString::number(nouveauDebit));

        changementEffectue = true;
    }

    if (changementEffectue) {
        for (int t = 0; t < nbTurbines; t++) {
            Turbine* turbine = turbines[t];
            long nouveauDebit = turbine->getdebits();

            // Accéder à l'historique et modifier les valeurs au milieu
            std::vector<MesureHistorique>& historique = turbine->getCapteur().getHistoriqueModifiable();

            if (!historique.empty()) {
                int totalMesures = historique.size();
                int mesuresA6h = std::min(24, totalMesures);
                int indexMilieu = totalMesures / 2;
                int indexDebut = std::max(0, indexMilieu - mesuresA6h / 2);
                int indexFin = std::min(totalMesures, indexDebut + mesuresA6h);

                // Modifier TOUTES les valeurs dans la fenêtre du milieu
                for (int i = indexDebut; i < indexFin && i < totalMesures; i++) {
                    historique[i].valeur = nouveauDebit;
                }

                qDebug() << "Turbine" << (t+1) << ": Modifié les mesures de"
                         << indexDebut << "à" << indexFin << "avec débit" << nouveauDebit;
            }
        }
    }

    if (!messagesErreur.isEmpty()) {
        QMessageBox::warning(this, "Erreurs de validation", messagesErreur);
    }

    afficherGraphiquesDebits();
    afficherGraphiquePuissanceTotale();

    if (changementEffectue) {
        QMessageBox::information(this, "Succès",
                                 "Les débits ont été mis à jour avec succès !\n"
                                 "Les valeurs au milieu du CSV ont été modifiées.");
    }
}

QGraphicsScene* MainWindow2::creerGraphiqueTurbine(Turbine* turbine, int numeroTurbine, QColor couleur)
{
    QGraphicsScene* scene = new QGraphicsScene();

    const double width = 380;
    const double height = 280;
    const double margin = 70;
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    std::vector<MesureHistorique> historique = turbine->getCapteur().getHistorique();

    // VÉRIFICATION CRITIQUE
    if (historique.empty()) {
        qDebug() << "ERREUR: Turbine" << numeroTurbine << "n'a pas de données!";
        scene->setSceneRect(0, 0, width, height);
        scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));
        QGraphicsTextItem *erreur = scene->addText("Pas de données disponibles");
        QFont errorFont = erreur->font();
        errorFont.setPointSize(12);
        errorFont.setBold(true);
        erreur->setFont(errorFont);
        erreur->setDefaultTextColor(Qt::red);
        erreur->setPos(width/2 - 100, height/2);
        return scene;
    }

    int totalMesures = historique.size();

    // VÉRIFICATION: Assez de mesures pour afficher 6h au milieu ?
    if (totalMesures < 24) {
        qDebug() << "ATTENTION: Turbine" << numeroTurbine << "seulement" << totalMesures << "mesures (24 recommandées)";
    }

    int mesuresA6h = std::min(24, totalMesures);
    int indexMilieu = totalMesures / 2;
    int indexDebut = std::max(0, indexMilieu - mesuresA6h / 2);
    int indexFin = std::min(totalMesures, indexDebut + mesuresA6h);

    // VÉRIFICATION: Indices valides ?
    if (indexDebut >= totalMesures || indexFin > totalMesures || indexDebut >= indexFin) {
        qDebug() << "ERREUR: Indices invalides pour Turbine" << numeroTurbine;
        qDebug() << "  Total:" << totalMesures << "Début:" << indexDebut << "Fin:" << indexFin;
        scene->setSceneRect(0, 0, width, height);
        scene->addRect(margin, margin, plotWidth, plotHeight, QPen(Qt::black, 1), QBrush(Qt::white));
        return scene;
    }

    QDateTime debut, fin;
    if (indexDebut < (int)historique.size() && indexFin > 0) {
        debut = historique[indexDebut].timestamp;
        fin = historique[std::min(indexFin - 1, (int)historique.size() - 1)].timestamp;
    } else {
        qDebug() << "ERREUR: Impossible d'accéder aux timestamps";
        debut = QDateTime::currentDateTime().addSecs(-6 * 3600);
        fin = QDateTime::currentDateTime();
    }

    double debitMin = turbine->getpMin();
    double debitMax = turbine->getpMax();

    if (indexDebut < indexFin && indexFin <= totalMesures) {
        double minObserve = debitMax;
        double maxObserve = debitMin;

        for (int i = indexDebut; i < indexFin; i++) {
            if (historique[i].valeur < minObserve) minObserve = historique[i].valeur;
            if (historique[i].valeur > maxObserve) maxObserve = historique[i].valeur;
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

    QString titre = QString("Turbine %1 - Milieu CSV").arg(numeroTurbine);
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

    qint64 dureeSecondes = debut.secsTo(fin);
    for (int i = 0; i <= 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin + plotHeight, x, margin + plotHeight + 8, axisPen);

        QDateTime temps = debut.addSecs(dureeSecondes * i / 6);
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

    // CORRECTION dans creerGraphiqueTurbine - Ligne ~570

    QPen curvePen(couleur, 3);

    if (indexDebut < indexFin) {
        QPointF lastPoint;
        bool first = true;

        // ⚠️ CORRECTION ICI : Vérifier AVANT d'accéder à historique[i]
        for (int i = indexDebut; i < indexFin && i < (int)historique.size(); i++) {
            const auto& mesure = historique[i];
            qint64 secondesDepuisDebut = debut.secsTo(mesure.timestamp);

            double x = margin + (plotWidth * secondesDepuisDebut / (double)dureeSecondes);

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

        // ⚠️ CORRECTION ICI AUSSI : Vérifier avant d'accéder
        if (indexMilieu >= 0 && indexMilieu < (int)historique.size()) {
            double valeurMilieu = historique[indexMilieu].valeur;
            QString valeurText = QString("Milieu: %1 m³/s").arg(valeurMilieu, 0, 'f', 1);
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

QGraphicsScene* MainWindow2::creerGraphiquePuissanceTotale()
{
    QGraphicsScene* scene = new QGraphicsScene();

    const double width = 380;
    const double height = 280;
    const double margin = 70;
    const double plotWidth = width - 2 * margin;
    const double plotHeight = height - 2 * margin;

    std::vector<Turbine*> turbines = centraleActuelle->getListeTurbine();

    if (turbines.empty()) {
        scene->setSceneRect(0, 0, width, height);
        return scene;
    }

    std::vector<MesureHistorique> historique = turbines[0]->getCapteur().getHistorique();

    if (historique.empty()) {
        qDebug() << "ERREUR: Pas de données pour le graphique de puissance totale";
        scene->setSceneRect(0, 0, width, height);
        return scene;
    }

    int totalMesures = historique.size();
    int mesuresA6h = std::min(24, totalMesures);
    int indexMilieu = totalMesures / 2;
    int indexDebut = std::max(0, indexMilieu - mesuresA6h / 2);
    int indexFin = std::min(totalMesures, indexDebut + mesuresA6h);

    QDateTime debut, fin;
    if (indexDebut < (int)historique.size() && indexFin > 0) {
        debut = historique[indexDebut].timestamp;
        fin = historique[std::min(indexFin - 1, (int)historique.size() - 1)].timestamp;
    } else {
        debut = QDateTime::currentDateTime().addSecs(-6 * 3600);
        fin = QDateTime::currentDateTime();
    }

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

    QString titre = "Puissance Totale - Milieu CSV";
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

    qint64 dureeSecondes = debut.secsTo(fin);
    for (int i = 0; i <= 6; i++) {
        double x = margin + (plotWidth * i / 6.0);
        scene->addLine(x, margin + plotHeight, x, margin + plotHeight + 8, axisPen);

        QDateTime temps = debut.addSecs(dureeSecondes * i / 6);
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

    // Collecter tous les timestamps uniques dans la fenêtre
    std::set<QDateTime> allTimestamps;
    for (size_t t = 0; t < turbines.size() && t < 5; t++) {
        std::vector<MesureHistorique> hist = turbines[t]->getCapteur().getHistorique();
        for (int i = indexDebut; i < indexFin && i < (int)hist.size(); i++) {
            allTimestamps.insert(hist[i].timestamp);
        }
    }

    QPen curvePen(QColor(231, 76, 60), 3);
    QPointF lastPoint;
    bool first = true;
    double puissanceActuelle = 0;

    for (const QDateTime& timestamp : allTimestamps) {
        qint64 secondesDepuisDebut = debut.secsTo(timestamp);

        double puissanceTotale = 0;

        for (size_t t = 0; t < turbines.size() && t < 5; t++) {
            std::vector<MesureHistorique> hist = turbines[t]->getCapteur().getHistorique();

            for (const auto& mesure : hist) {
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

        double x = margin + (plotWidth * secondesDepuisDebut / (double)dureeSecondes);

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
        QString valeurText = QString("Milieu: %1 MW").arg(puissanceActuelle, 0, 'f', 2);
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

// Fonctions de production (identiques à MainWindow1)
float MainWindow2::fonctionT1(float du, float hc)
{
    float p00 = 1.102, p10 = -0.03187, p01 = -0.04866, p11 = 0.003308;
    float p02 = 0.002182, p12 = 3.638 * pow(10, -5), p03 = -1.277 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow2::fonctionT2(float du, float hc)
{
    float p00 = -1.382, p10 = 0.09969, p01 = -1.945, p11 = 0.09224, p20 = -0.001724;
    float p02 = 0.007721, p12 = -6.622 * pow(10, -5), p21 = -0.001096, p03 = -1.933 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) + p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow2::fonctionT3(float du, float hc)
{
    float p00 = 0.7799, p10 = -0.02261, p01 = 0.1995, p11 = -0.001695;
    float p02 = -3.519 * pow(10, -5), p12 = 7.235 * pow(10, -5), p03 = -9.338 * pow(10, -6);
    return p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow2::fonctionT4(float du, float hc)
{
    float p00 = 20.22, p10 = -0.5777, p01 = -0.4586, p11 = 0.01151;
    float p02 = 0.004886, p12 = 1.379 * pow(10, -5), p03 = -1.882 * pow(10, -5);
    return p00 + p10*hc + p01*du + p11*du*hc + p02*pow(du, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}

float MainWindow2::fonctionT5(float du, float hc)
{
    float p00 = -212.1, p10 = 12.17, p01 = 0.004397, p11 = -0.006808, p20 = -0.1746;
    float p02 = 0.004529, p12 = -4.211*pow(10, -5), p21 = 0.0002936, p03 = -1.176*pow(10, -5);
    return p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc, 2) + p02*pow(du, 2) + p21*du*pow(hc, 2) + p12*hc*pow(du, 2) + p03*pow(du, 3);
}
