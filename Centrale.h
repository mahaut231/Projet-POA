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
    long volumeDeProduction;
    std::vector<Turbine> listeDesTurbines;
    Reservoire reservoire;

public:
    Centrale(int id, long volume,std::vector<Turbine> listeTurbine,Reservoire reservoire);

    int getId();
    long getVolume();
    std::vector<Turbine> getListeTurbine();
    Reservoire getreservoire();


    void setid(int id);
    void setVolume(long volume);
    void addTurbine(Turbine turbine);
    void supprTurbine(Turbine turbine);
    void setReservoire(Reservoire reservoire);

    std::string voirDonnesAllTurbines();
    std::string voirDonnesOneTurbine(int idTurbine);
    std::string voirDonnesReservoire();
    // voir pour changée les sortie afin de renvoyée se dont le front a besoin

    //fonction de calcule du débit
    float fonctionT1(float chuteNet,float debitUnitaire);
    float fonctionT2(float chuteNet,float debitUnitaire);
    float fonctionT3(float chuteNet,float debitUnitaire);
    float fonctionT4(float chuteNet,float debitUnitaire);
    float fonctionT5(float chuteNet,float debitUnitaire);
    float fonctionPtot(float chuteNet,float q1,float q2,float q3,float q4,float q5);
    float elevationAval();
    float hauteurDeChuteNet();
};

#endif // CENTRALE_H
