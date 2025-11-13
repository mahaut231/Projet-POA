#include "Reservoire.h"

Reservoire::Reservoire(int id,Capteur capteurAM, Capteur capteurAV):
    id(id),
    capteurAmont(capteurAM),
    capteurAvale(capteurAV){}


Capteur Reservoire::getCapteurAmont(){return capteurAmont;}
Capteur Reservoire::getCapteurAval(){return capteurAvale;}

void Reservoire::setCapteurAval(Capteur capteurAV){
    this->capteurAvale=capteurAV;
}

void Reservoire::setCapteurAmont(Capteur capteurAM){
    this->capteurAmont=capteurAM;
}

