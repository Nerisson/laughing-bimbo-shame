#include "train.h"
#include <iostream>
#define LONG_WAG 1.0
using namespace std;

Train::Train(int _IDT, int _NBW, float _speed, int _IDA, CGraphe *_graphe)
{
    float D;
    for(int _IDW=0;_IDW<_NBW;_IDW++)
    {
        Wagon wag(_IDW,_IDA,_graphe,_speed);
        D = (_NBW-_IDW)*LONG_WAG;
        if(D>wag.Dmax)
        {
            float Dmax_temp = wag.Dmax;
            wag.changeTroncon();
            wag.D = D-Dmax_temp;
        }
        else wag.D = (_NBW-_IDW)*LONG_WAG;
        wagons.push_back(wag);
    }
    NBW = _NBW;
    graphe = _graphe;
    IDT = _IDT;
    speed = _speed;
}

Train::Train(Train const& tr)
    : IDT(tr.IDT), NBW(tr.NBW), speed(tr.speed)
{
    wagons = tr.wagons;
    graphe = tr.graphe;
}

void Train::deplace()
{
    wagons[0].deplace();
    // si le premier wagon change d'arc (on suppose qu'il en existe au moins 2)
    if(wagons[0].IDA!=wagons[1].IDA)
    {
        for(int i=1;i<NBW;i++)
        {
            wagons[i].IDAS=wagons[0].IDA;
        }
    }
    for(int i=1;i<NBW;i++)
    {
        wagons[i].deplace();
    }
}

void Train::drawTrain()
{
    for(int i=0; i<wagons.size(); i++)
    {
        wagons[i].drawWagon();
    }
}

void Train::setSpeed(float _speed)
{
    speed = _speed;
    for(int i=0; i<NBW; i++)
    {
        wagons.at(i).speed = _speed;
    }
}
