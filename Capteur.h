#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <string>
#include <vector>
#include <QString>
#include <QDateTime>

struct MesureHistorique {
    long valeur;
    QDateTime timestamp;

    MesureHistorique(long v, QDateTime t) : valeur(v), timestamp(t) {}
};

class Capteur {
protected:
    int id;
    long mesure;
    std::string niveauDusure;
    std::vector<MesureHistorique> historique;

public:
    Capteur(int id, long mesure, std::string niveauDusure);
    virtual ~Capteur() {}

    // Getters
    int getId() const { return id; }
    long getMesure() const { return mesure; }
    std::string getNiveau() const { return niveauDusure; }
    std::vector<MesureHistorique> getHistorique() const { return historique; }

    // Setters
    void setid(int id) { this->id = id; }
    void setMesure(long mesure);  // Modifié pour ajouter à l'historique
    void setNiveau(std::string niveau) { this->niveauDusure = niveau; }

    // Gestion de l'historique
    void ajouterMesure(long mesure);
    void ajouterMesure(long mesure, QDateTime timestamp);
    int getNombreMesures() const { return historique.size(); }
    void viderHistorique() { historique.clear(); }

    // Méthode virtuelle pour l'affichage (sera décorée)
    virtual void afficherMesure() const;
    virtual void afficherHistorique(int nombreMesures = 10) const;
    virtual std::string getUnite() const { return ""; }
    virtual std::string getType() const { return "Capteur de base"; }
};

#endif
