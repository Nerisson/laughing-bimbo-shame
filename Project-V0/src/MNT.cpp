#include "MNT.h"

MNT::MNT(string path)
{

    cout << path << endl;

    float z;

    ifstream infileASCII;
    infileASCII.open(path.c_str());


    float min_x = 1;
    float max_x = 25.8;
    float min_y = 0;
    float max_y = 23;

    float dx = (max_x-min_x)/49;
    float dy = (max_y-min_y)/49;

    if (infileASCII){
        for(float i=0; i<50; i++){
            for(float j=0; j<50; j++){
                infileASCII >> z;
                CPoint3f p(min_x+j*dx, min_y+i*dy, z);
                grid.push_back(p);
            }
        }
    } else {
        cout << "File not open for reading!" << endl;
    }

    cout << "MNT: " << grid.size() << endl;
	infileASCII.close();

}

CPoint3f MNT::get(int i){
    return grid.at(i);
}

int MNT::getSize(){
    return grid.size();
}

MNT::~MNT()
{
    //dtor
}
