#ifndef TURBINE_H
#define TURBINE_H
#include "iostream"
class turbine
{
private:

    //Attributs
    int id;
    long puissance;
    bool etatActivation;
    std::string dateMiseEnPlace;
    int puissanceMax;
    int puissanceMin;

public:
    turbine(int id, long puissance, bool etatActivation,std::string date, int pMax, int pMin);

    // Méthodes
    void activer();
    void desactiver();
    void afficherInfos() const;
};

#endif // TURBINE_H
