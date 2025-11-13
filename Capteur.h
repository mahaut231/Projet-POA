#ifndef CAPTEUR_H
#define CAPTEUR_H
#include "iostream"

class Capteur
{
private:
    //Attributs
    int id;
    long mesure;
    std::string niveauDusure;

public:
    Capteur(int id,long mesure, std::string niveauDusure);

    int getId();
    long getMesure();
    std::string getNiveau();

    void setid(int id);
    void setMesure(long mesure);
    void setNiveau(std::string niveau);


};

#endif // CAPTEUR_H
