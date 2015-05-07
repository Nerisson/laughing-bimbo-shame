#ifndef TRAIN_H
#define TRAIN_H

#include "API/src/CArc.h"
#include "API/src/CGraphe.h"
#include "API/src/CPointAnnexe.h"

#include <math.h>
#include <windows.h>
#include <GL/glut.h>

#include <iostream>
using namespace std;

class Train
{
    public:
        Train(float x, float y, float z, float vit, CArc arc, CGraphe *g);
        virtual ~Train();

        float Getx() { return x; }
        void Setx(float val) { x = val; }
        float Gety() { return y; }
        void Sety(float val) { y = val; }
        float Getz() { return z; }
        void Setz(float val) { z = val; }

        void draw();
        void move();
        CPoint3f getPointPrec();
        CPoint3f getPointSuiv();

    private:

        float x;
        float y;
        float z;

        float vit;
        int cPt;

        CArc arc;
        CGraphe *graphe;
};

#endif // TRAIN_H
