#ifndef MNT_H
#define MNT_H

#include <iostream>
#include <fstream>
#include <vector>
#include "API/src/CPoint3f.h"

using namespace std;

class MNT
{
    public:

        MNT(string path);
        virtual ~MNT();
        CPoint3f get(int);
        int getSize();

    private:
        vector<CPoint3f> grid;
};

#endif // MNT_H
