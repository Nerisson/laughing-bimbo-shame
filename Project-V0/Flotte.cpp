#include "Flotte.h"

Flotte::Flotte(int n, CGraphe *g){
    graphe = g;
    vector<Train> v;
    initFlotte(n);
}

Flotte::Flotte(CGraphe *g){
    graphe = g;
}

Flotte::~Flotte()
{
    //dtor
}

void Flotte::initFlotte(int n){
    for(int i=0; i<n; i++){
        int nArc = rand() % graphe->nb_arc;
//        nArc=5;
        CArc arc = graphe->list_arc.at(nArc);
        CSommet si = graphe->list_sommet.at(arc.id_sommet_ini);
        Train t(si.X, si.Y, si.Z, 0.0005, arc, graphe);
        v.push_back(t);
    }
    cout << "Trains Charges: " << v.size() << endl;
}

int Flotte::size(){
    return v.size();
}

void Flotte::draw(){
    for(unsigned int i=0; i<v.size(); i++){
        glPushMatrix();
        v.at(i).draw();
        glPopMatrix();
    }
}

void Flotte::move(){
    for(unsigned int i=0; i<v.size(); i++){
        v.at(i).move();
    }
}

Train Flotte::getTrain(int id){
    return v.at(id);
}
