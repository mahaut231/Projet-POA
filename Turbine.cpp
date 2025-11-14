#include "Turbine.h"
#include "iostream"
Turbine::Turbine(int id, long puissance, bool etat, std::string date):
    id(id),
    puissance(puissance),
    etatActivation(etat),
    dateMiseEnPlace(date),
    puissanceMax(160),
    puissanceMin(0) {}


    //getter et setter :
    int Turbine::getId(){return id;}
    long Turbine::getPuissance(){return puissance;}
    bool Turbine::getEtatActivation(){return etatActivation;}
    std::string Turbine::getDate() { return dateMiseEnPlace;}
    int Turbine::getpMax(){return puissanceMax;}
    int Turbine::getpMin(){return puissanceMin;}

    void Turbine::setId(int id){
        this->id=id;}

    void Turbine::setPuissance(long puissance){
        this->puissance=puissance;}

    void Turbine::setEtatActivation(bool etat){
        this->etatActivation=etat;}

    void Turbine::setDate(std::string date) {
        this->dateMiseEnPlace=date;}

    void Turbine::setpMax(int p){
        this->puissanceMax=p;}

    void Turbine::setpMin(int p){
        this->puissanceMin=p;}


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
                  << "Puissance: " << puissance << "W\n"
                  << "Date: " << dateMiseEnPlace << "\n";
    }
