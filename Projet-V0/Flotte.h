#ifndef FLOTTE_H
#define FLOTTE_H

#include "API/src/CGraphe.h"
#include "API/src/CArc.h"
#include "API/src/CSommet.h"

#include "Train.h"

#include <iostream>
#include <vector>
using namespace std;



class Flotte
{
    public:
        Flotte(int n, CGraphe *g);
        Flotte(CGraphe *g);

        virtual ~Flotte();

        void initFlotte(int n);
        int size();
        void draw();
        void move();

        vector<Train> Getv() { return v; }
        void Setv(vector<Train> val) { v = val; }

    private:
        vector<Train> v;
        CGraphe *graphe;
};

#endif // FLOTTE_H
