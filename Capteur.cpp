#include "Capteur.h"
#include "iostream"
Capteur::Capteur(int id, long mesure, std::string niveauDusure):
    id(id),
    mesure(mesure),
    niveauDusure(niveauDusure){}

    //getter et setter

    int Capteur::getId(){return id;}
    long Capteur::getMesure(){return mesure;}
    std :: string Capteur :: getNiveau(){return niveauDusure;}

    void Capteur::setid(int id){
        this->id=id;
    }

    void Capteur::setMesure(long mesure){
        this->mesure=mesure;
    }

    void Capteur::setNiveau(std::string niveau){
        this->niveauDusure=niveau;
    }

    //Méthode


