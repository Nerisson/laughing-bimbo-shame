#ifndef GCOLL_H
#define GCOLL_H
#include <vector>
#include <train.h>

using namespace std;

class GColl
{
public:
    vector<Train> listeTrain;       // liste des trains
    vector<int>   liTarr;           // liste des trains arrêtés
    vector<float> liVarr;           // liste des vitessses des trains arrêtés
    vector<int>   liTral;           // liste des trains ralentis
    vector<float> liVral;           // liste des vitesses initiales des trains ralentis

public:
    GColl();
    void addTrain(Train _train);

    void stopTrain(int IDT);
    void raleTrain(int IDT, float _speed);
    void demarreTrain(int IDT);
    void accelerTrain(int IDT);

    void ralentir();
    void arreter();

    void accelerer();
    void demarrer();

    void drawTrains();
    void deplaceTrains();

    bool testSommet(int _IDT);      // vrai si le sommet est libre et faux sinon
    int testTroncon(int _IDT);      // renvoie l'id du train devant et -1 si pas de train
    int testArc(int _IDT);          // renvoie l'id du train sur l'autre arc et -1 si pas de train
};

#endif // GCOLL_H
