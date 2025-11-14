#ifndef TURBINE_H
#define TURBINE_H
#include "iostream"
class Turbine
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
        Turbine(int id, long puissance, bool etatActivation,std::string date);

        int getId();
        long getPuissance();
        bool getEtatActivation();
        std::string getDate();
        int getpMax();
        int getpMin();

        void setId(int id);
        void setPuissance(long p);
        void setEtatActivation(bool etat);
        void setDate(std::string date);
        void setpMax(int puissance);
        void setpMin(int puissance);


        // Méthodes

        void activer();
        void desactiver();
        void afficherInfos() const;
};

#endif // TURBINE_H
