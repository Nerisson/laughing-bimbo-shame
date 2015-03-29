#include "Camera.h"
#include <cmath>
#include <iostream>

using namespace std;
int px=0,py=0,mvt;

Camera::Camera(float xCam, float yCam, float zCam,
               float xTar, float yTar, float zTar)
{
    // positions initiales
    posCam = Vecteur(xCam,yCam,zCam);
    posTar = Vecteur(xTar,yTar,zTar);
    Up = Vecteur(0,0,1);
    // calcul de l'orientation
    updateAngles();
    updateVect();
}

void Camera::updateAngles()
{
    // différence de position
    Vecteur delta = posTar - posCam;
    float r = delta.norm();

    // calcul des angles
    phi = asin(delta.getZ()/r);
    theta = 2*atan(delta.getX()/(delta.getY()+r*cos(phi)));
    phi*=180/M_PI;
    theta*=180/M_PI;
}

void Camera::updateVect()
{
    float r = cos(phi*M_PI/180);
    // passage aux coordonnées sphériques
    Fwd.setX(r*cos(theta*M_PI/180));
    Fwd.setY(r*sin(theta*M_PI/180));
    Fwd.setZ(sin(phi*M_PI/180));
    // produit vectoriel des vecteurs
    Left = Up.prodVect(Fwd);
    // mise à jour
    posTar = posCam + Fwd;
}

void Camera::Look()
{
    gluLookAt(posCam.getX(), posCam.getY(), posCam.getZ(),
              posTar.getX(), posTar.getY(), posTar.getZ(),
              Up.getX()    , Up.getY()    , Up.getZ());
}

void Camera::Mouse(int button, int state, int x, int y)
{
    mvt = button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;

    if(!mvt) return;

    px = x;
    py = y;
}

void Camera::Motion(int x, int y)
{
    if(!mvt) return;
    // calcul des nouveaux angles
    theta-= (x-px)*0.1;
    phi-= (y-py)*0.1;
    // on évite un phi égal à 90 pour ne pas avoir de changement brusque de la caméra
    if(fabs(phi)>=90)
    {
        phi = 89.9*fabs(phi)/phi;
    }
    px = x;
    py = y;
    // mise à jour des vecteurs
    updateVect();
}

void Camera::Keybord(unsigned char key, int , int )
{
    switch (key)
    {
    case '+':
        posCam+=Up.prodScal(0.1);
        break;
    case '-':
        posCam-=Up.prodScal(0.1);
        break;
    case 'q':
        posCam+=Left.prodScal(0.1);
        break;
    case 'd':
        posCam-=Left.prodScal(0.1);
        break;
    case 'z':
        posCam+=Fwd.prodScal(0.1);
        break;
    case 's':
        posCam-=Fwd.prodScal(0.1);
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }

    // mise à jour de la position
    posTar = posCam + Fwd;
    updateVect();
}
