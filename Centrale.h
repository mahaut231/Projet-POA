#ifndef CENTRALE_H
#define CENTRALE_H
#include <iostream>
#include <vector>
#include "Turbine.h"
#include"Reservoire.h"

class Centrale
{
private:
    //Attributs
    int id;
    float niveauAmont;
    float niveauAval;
    float hauteurDeChute;
    long debitsTotal;
    std::vector<Turbine> listeDesTurbines;
    Reservoire reservoire;

public:
    Centrale(int id, float niveauAmont, float niveauAval, std::vector<Turbine> listeTurbine,Reservoire reservoire);

    int getId();
    long getVolume();
    std::vector<Turbine> getListeTurbine();
    Reservoire getreservoire();


    void setid(int id);

    void addTurbine(Turbine turbine);
    void supprTurbine(Turbine turbine);
    void setReservoire(Reservoire reservoire);

    std::string voirDonnesAllTurbines();
    std::string voirDonnesOneTurbine(int idTurbine);
    std::string voirDonnesReservoire();
    // voir pour changée les sortie afin de renvoyée se dont le front a besoin

    //fonction de calcule du débit
    float fonctionT1(float debitUnitaire);
    float fonctionT2(float debitUnitaire);
    float fonctionT3(float debitUnitaire);
    float fonctionT4(float debitUnitaire);
    float fonctionT5(float debitUnitaire);
    float elevationAval();
    float hauteurDeChuteNet();
    float calculerProductionTotale();
};

#endif // CENTRALE_H
