#ifndef TRAIN_H
#define TRAIN_H
#include <vector>
#include "API/CDonneesGraphe.h"
#include "API/CGraphe.h"
#include "API/CSommet.h"
#include "wagon.h"

class Train
{
public:
    int IDT;                    // identifiant du train
    int NBW;                    // nombre de wagons
    float speed;                // vitesse du train
    std::vector<Wagon> wagons;  // wagons
    CGraphe *graphe;            // graphe

public:
    Train(int _IDT, int NBW, float _speed, int _IDA, CGraphe *_graphe);
    Train(Train const& tr);
    void deplace();
    void drawTrain();
    void setSpeed(float _speed);
};

#endif // TRAIN_H
