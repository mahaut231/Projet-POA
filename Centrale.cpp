#include "Centrale.h"
#include <vector>
#include <iostream>

Centrale::Centrale(int id, long volume,std::vector<Turbine> listeTurbine,Reservoire reservoire):
    id(id),
    volumeDeProduction(volume),
    listeDesTurbines(listeTurbine),
    reservoire(reservoire){}


int Centrale::getId(){return id;}
long Centrale::getVolume(){return volumeDeProduction;}
std::vector<Turbine>Centrale:: getListeTurbine(){return listeDesTurbines;}
Reservoire Centrale::getreservoire(){return reservoire;}


void Centrale::setid(int id){
    this->id=id;
};
void Centrale::setVolume(long volume){
    this->volumeDeProduction=volume;
};
void Centrale::addTurbine(Turbine turbine){
    this->listeDesTurbines.push_back(turbine);
};//voir pour changée cela avec les valeur d'ID
//void Centrale::supprTurbine(Turbine turbine){
  //  this->listeDesTurbines
//};
void Centrale::setReservoire(Reservoire reservoire){
    this->reservoire=reservoire;
};
