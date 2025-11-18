#ifndef TURBINE_H
#define TURBINE_H
#include <iostream>
#include "Subject.h"
#include "Capteur.h"
class Turbine : public Subject {
    private:

        //Attributs
        int id;
        Capteur capteur;
        long debits;
        bool etatActivation;
        std::string dateMiseEnPlace;
        int debitsMax;
        int debitsMin;

    public:
        Turbine(int id, long debits, bool etatActivation,std::string date, Capteur capteur);

        int getId();
        long getdebits();
        bool getEtatActivation();
        std::string getDate();
        int getpMax();
        int getpMin();

        void setId(int id);
        void setdebits(long d);
        void setEtatActivation(bool etat);
        void setDate(std::string date);
        void setpMax(int debits);
        void setpMin(int debits);


        // Méthodes

        void activer();
        void desactiver();
        void afficherInfos() const;
};

#endif // TURBINE_H
