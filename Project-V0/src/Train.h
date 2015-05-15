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
        float Gety() { return y; }
        float Getz() { return z; }
        float getAy();

        float GetPreviousX() { return previousX; }
        float GetPreviousY() { return previousY; }
        float GetPreviousZ() { return previousZ; }

        void draw();
        void move();
        CPoint3f getPointPrec();
        CPoint3f getPointSuiv();

    private:

        float x;
        float y;
        float z;

        float previousX;
        float previousY;
        float previousZ;

        float vit;
        int cPt;

        CArc arc;
        CGraphe *graphe;
};

#endif // TRAIN_H
