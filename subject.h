#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "Observer.h"

class Subject {
protected:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() {}

    void ajouterObserver(Observer* obs) {
        observers.push_back(obs);
    }

    void retirerObserver(Observer* obs) {
        for(int i = 0; i < observers.size(); i++) {
            if(observers[i] == obs) {
                observers.erase(observers.begin() + i);
                break;
            }
        }
    }

    void notifierObservers() {
        for(int i = 0; i < observers.size(); i++) {
            observers[i]->update();
        }
    }
};

#endif
