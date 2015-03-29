#ifndef CAMERA_H
#define CAMERA_H
#include "vecteur.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Camera
{
    Vecteur posCam,posTar,Up,Left,Fwd;
    float theta,phi;
public:
    Camera(float xCam, float yCam, float zCam,
           float xTar, float yTar, float zTar);
    void updateAngles();
    void updateVect();
    void Look();
    void Mouse(int button, int state, int x, int y);
    void Motion(int x, int y);
    void Keybord(unsigned char key, int, int);
};

#endif // CAMERA_H
