#include "Turbine.h"
#include "iostream"
Turbine::Turbine(int id, long debits, bool etat, std::string date, Capteur capteur):
    id(id),
    debits(debits),
    etatActivation(etat),
    dateMiseEnPlace(date),
    debitsMax(160),
    debitsMin(0),
    capteur(capteur){}


    //getter et setter :
    int Turbine::getId(){return id;}
    long Turbine::getdebits(){return debits;}
    bool Turbine::getEtatActivation(){return etatActivation;}
    std::string Turbine::getDate() { return dateMiseEnPlace;}
    int Turbine::getpMax(){return debitsMax;}
    int Turbine::getpMin(){return debitsMin;}

    void Turbine::setId(int id){
        this->id=id;}

    void Turbine::setdebits(long debits){
        if(debits>this->debitsMax){
            std::cout<< "La valeur est superieur à la limite maximum aucune valeurs changée \n" ;

        }
        else if(debits<this->debitsMin){
            std::cout<< "La valeur est inferieur à la limite minimum aucune valeurs changée \n" ;
        }
        else{
            this->debits=debits;
        }
        }

    void Turbine::setEtatActivation(bool etat){
        this->etatActivation=etat;}

    void Turbine::setDate(std::string date) {
        this->dateMiseEnPlace=date;}

    void Turbine::setpMax(int p){
        this->debitsMax=p;}

    void Turbine::setpMin(int p){
        this->debitsMin=p;}


    // Méthode pour activer la turbine
    void Turbine::activer() {
        etatActivation = true;
        std::cout << "Turbine " << id << " activée.\n";
    }

    // Méthode pour désactiver la turbine
    void Turbine::desactiver() {
        etatActivation = false;
        std::cout << "Turbine " << id << " désactivée.\n";
    }

    // Méthode pour afficher les informations
    void Turbine::afficherInfos() const {
        std::cout << "Turbine #" << id << " (" << (etatActivation ? "Active" : "Inactive") << ")\n"
                  << "debits: " << debits << "W\n"
                  << "Date: " << dateMiseEnPlace << "\n";
    }
