#include "DecorateurCapteur.h"

DecorateurCapteur::DecorateurCapteur(Capteur* capteur)
    : Capteur(capteur->getId(), capteur->getMesure(), capteur->getNiveau()),
    capteurDecore(capteur) {}
