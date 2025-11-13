#ifndef RESERVOIRE_H
#define RESERVOIRE_H
#include <iostream>
#include "Capteur.h"
class Reservoire
{
private:
    int id;
    Capteur capteurAmont;
    Capteur capteurAvale;

public:
    Reservoire(int id,Capteur capteurAM, Capteur capteurAV);

    Capteur getCapteurAmont();
    Capteur getCapteurAval();

    void setCapteurAmont(Capteur capteurAM);
    void setCapteurAval(Capteur capteurAV);
};

#endif // RESERVOIRE_H
