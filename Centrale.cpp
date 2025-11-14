#include "Centrale.h"
#include <vector>
#include <iostream>
#include <cmath>

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

float Centrale::fonctionT1(float hc, float du){
    float p00 = 1.102;
    float p10 = -0.03187;
    float p01 = -0.04866;
    float p11 = 0.003308;
    float p02 = 0.002182;
    float p12 = 3.638*pow(10,-5);
    float p03 = -1.277*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    return res;

}

float Centrale::fonctionT2(float hc, float du){
    float p00 = -1.382;
    float p10 = 0.09969;
    float p01 = -1.945;
    float p11 = 0.09224;
    float p20 = -0.001724;
    float p02 = 0.007721;
    float p12 = -6.622*pow(10,-5);
    float p21 = -0.001096;
    float p03 = -1.933*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc,2) + p02*hc*pow(du,2) + p21*du*pow(hc,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    return res;

}

float Centrale::fonctionT3(float hc, float du){
    float p00 = 0.7799;
    float p10 = -0.02261;
    float p01 = 0.1995;
    float p11 = -0.001695;
    float p02 = -3.519* pow(10,-5);
    float p12 = 7.235 * pow(10,-5);
    float p03 = -9.338 * pow(10,-6);

    float res = p00 + p10*hc + p01*du + p11*hc* du + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    return res;
}

float Centrale::fonctionT4(float hc, float du){
    float p00 = 20.22;
    float p10 = -0.5777;
    float p01 = -0.4586;
    float p11 = 0.01151;
    float p02 = 0.004886;
    float p12 = 1.379 * pow(10,-5);
    float p03 = -1.882 * pow(10,-5);

    float res = p00 + p10*hc + p11*du*hc + p02*pow(du,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    return res;
}

float Centrale::fonctionT5(float hc, float du){
    float p00 = -212.1;
    float p10 = 12.17;
    float p01 = 0.004397;
    float p11 = -0.006808;
    float p20 = -0.1746;
    float p02 = 0.004529;
    float p12 = -4.211*pow(10,-5);
    float p21 = 0.0002936;
    float p03 = -1.176*pow(10,-5);

    float res = p00 + p10*hc + p01*du + p11*hc*du + p20*pow(hc,2) + p02*pow(du,2) + p21*du*pow(hc,2) + p12*hc*pow(du,2) + p03*pow(du,3);
    return res;
}



float Centrale::elevationAval(){
    float p1 = -1.453*exp(-6);
    float p2 = 0.007022;
    float p3 = 99.98;

    float res = p1*pow(volumeDeProduction,2) + p2*volumeDeProduction + p3;
    return res;
}
