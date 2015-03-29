#include "gcoll.h"
#include <cmath>
#include <iostream>
#define D_SEC 4.321

using namespace std;

bool inListe(vector<int> vect, int ID)
{
    for(int i=0;i<vect.size();i++)
    {
        if(vect.at(i)==ID) return true;
    }
    return false;
}

GColl::GColl()
{

}

void GColl::addTrain(Train _train)
{
    listeTrain.push_back(_train);
}

void GColl::drawTrains()
{
    for(int IDT=0; IDT<listeTrain.size(); IDT++)
    {
        listeTrain.at(IDT).drawTrain();
    }
}

void GColl::deplaceTrains()
{
    for(int IDT=0; IDT<listeTrain.size(); IDT++)
    {
        listeTrain.at(IDT).deplace();
    }
    ralentir();
    arreter();


    demarrer();
    accelerer();
}

void GColl::stopTrain(int IDT)
{
    float v_temp = listeTrain.at(IDT).speed;
    liTarr.push_back(IDT);
    liVarr.push_back(v_temp);
    listeTrain.at(IDT).setSpeed(0);
}

void GColl::raleTrain(int IDT, float _speed)
{
    float v_temp = listeTrain.at(IDT).speed;
    liTral.push_back(IDT);
    liVral.push_back(v_temp);
    listeTrain.at(IDT).setSpeed(_speed);
}

void GColl::demarreTrain(int IDT)
{
    listeTrain.at(liTarr.at(IDT)).setSpeed(liVarr.at(IDT));
    liTarr.erase(liTarr.begin()+IDT);
    liVarr.erase(liVarr.begin()+IDT);
}

void GColl::accelerTrain(int IDT)
{
    listeTrain.at(liTral.at(IDT)).setSpeed(liVral.at(IDT));
    liTral.erase(liTral.begin()+IDT);
    liVral.erase(liVral.begin()+IDT);
}

void GColl::ralentir()
{
    // on parcourt la liste des trains
    for(int _IDT=0; _IDT<listeTrain.size(); _IDT++)
    {
        int ID = testTroncon(_IDT);
        if(ID!=-1)
        {
            if(listeTrain.at(_IDT).speed>listeTrain.at(ID).speed) raleTrain(_IDT,listeTrain.at(ID).speed);
        }
    }
}

void GColl::arreter()
{
    for(int _IDT=0;_IDT<listeTrain.size();_IDT++)
    {
        Wagon PW = listeTrain.at(_IDT).wagons.at(0);
        // on fait le test quand le train est à D_SEC d'un sommet
        if(PW.dernierTroncon() && PW.Dmax-PW.D<D_SEC && !inListe(liTarr,_IDT))
        {
            // cas où le sommet est libre
            if(testSommet(_IDT))
            {
                int ID = testArc(_IDT);
                if(ID!=-1 && listeTrain.at(_IDT).speed!=0.0) stopTrain(_IDT);
            }
            else if(!testSommet(_IDT) && listeTrain.at(_IDT).speed!=0.0) stopTrain(_IDT);
        }
    }
}

void GColl::accelerer()
{
    // on parcourt la liste des trains ralentis
    for(int _IDT=liTral.size()-1;_IDT>=0;_IDT--)
    {
        if(testTroncon(liTral.at(_IDT))==-1)  accelerTrain(_IDT);

    }
}

void GColl::demarrer()
{
    //    for(int _IDT=0; _IDT<liTarr.size(); _IDT++)
    for(int _IDT=liTarr.size()-1;_IDT>=0;_IDT--)
    {
        // le sommet est libre et le train est prioritaire (ou il n'y a pas de train sur l'autre arc)
        if(testSommet(liTarr.at(_IDT)) && testArc(liTarr.at(_IDT))==-1) demarreTrain(_IDT);

    }
}

bool GColl::testSommet(int _IDT)
{
    // premier wagon du train
    Wagon PW = listeTrain.at(_IDT).wagons.at(0);
    for(int ID=0; ID<listeTrain.size(); ID++)
    {
        if(ID!=_IDT)
        {
            Wagon PW2 = listeTrain.at(ID).wagons.at(0);
            if(PW.IDSI==PW2.IDSI && !PW2.dernierTroncon())
            {
                for(int IDW=0; IDW<listeTrain.at(ID).NBW-1; IDW++)
                {
                    if(listeTrain.at(ID).wagons.at(IDW).premierTroncon() &&
                            listeTrain.at(ID).wagons.at(IDW+1).dernierTroncon()) return false;
                }
            }
        }
    }
    return true;
}

int GColl::testTroncon(int _IDT)
{
    // premier wagon du train
    Wagon PW = listeTrain.at(_IDT).wagons.at(0);
    for(int ID=0; ID<listeTrain.size(); ID++)
    {
        if(_IDT!=ID)
        {
            Wagon DW = listeTrain.at(ID).wagons.at(listeTrain.at(ID).NBW-1);
            if((PW.IDSI==DW.IDSI && PW.IDSF==DW.IDSF && PW.dernierTroncon() && DW.premierTroncon() && DW.D+PW.Dmax-PW.D<D_SEC) ||   // premier troncon d'un arc
                    (PW.IDSI==DW.IDSI && PW.IDSF==DW.IDSF && PW.IDPA==DW.IDPA-1 && PW.Dmax-PW.D+DW.D<D_SEC) ||  // troncon suivant
                    (PW.IDSI==DW.IDSI && PW.IDSF==DW.IDSF && PW.IDPA==DW.IDPA && fabs(DW.D-PW.D)<D_SEC) ||            // même troncon
                    (DW.dernierTroncon() && PW.IDSF==DW.IDSI && PW.avDerniTroncon()) ||             // dernier troncon
                    (DW.dernierTroncon() && PW.dernierTroncon() && DW.D-PW.D<D_SEC && PW.IDSI==DW.IDSI && PW.posIni==DW.posIni))
            {
                return ID;
            }
        }
    }
    return -1;
}

int GColl::testArc(int _IDT)
{
    Wagon PW = listeTrain.at(_IDT).wagons.at(0);
    for(int ID=0; ID<listeTrain.size(); ID++)
    {
        if(ID!=_IDT)
        {
            Wagon PW2 = listeTrain.at(ID).wagons.at(0);
            if((PW2.dernierTroncon() && PW.IDSI==PW2.IDSI && PW2.speed>PW.speed && PW.D/PW.Dmax<PW2.D/PW2.Dmax) ||
                    (PW2.dernierTroncon() && PW.IDSI==PW2.IDSI && PW2.speed<=PW.speed && PW2.D/PW2.Dmax>PW.D/PW.Dmax)) return ID;
        }
    }
    return -1;
}

