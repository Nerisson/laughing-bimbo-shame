#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "API/CDonneesGraphe.h"
#include "API/CGraphe.h"
#include "API/CSommet.h"
#include "Camera.h"
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "wagon.h"
#include "train.h"
#include "gcoll.h"
#include "sdlglutils.h"
GLuint herbe;
GLuint ciel;
GLuint bois;
GLuint feuille;
#define mSPF 30

using namespace std;

// création de la caméra
Camera *cam;
GColl *flotte;

// construction des tables du graphe
CDonneesGraphe gdata(   "dataG/SXYZ.TXT",
                        "dataG/SIF.TXT",
                        "dataG/PAXYZ.TXT",
                        "dataG/AXYZ.TXT");
// Constrcuction de la base de donnees orientee objet du graphe
CGraphe graphe(gdata);

// fonctions
void Dessiner();
void Initialiser();
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Keybord(unsigned char key, int x, int y);
void drawGraphe();
void Resize(int width, int height);
void drawTree(float x, float y, float z, float hauteur);
void drawPont();
void rotate(int value)
{
    flotte->deplaceTrains();
    glutPostRedisplay();
    glutTimerFunc(mSPF,rotate,value);
}

int main(int argc, char *argv[])
{
    /*---------------------------------------------*/
    glutInit(&argc, argv);
    Initialiser();
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    herbe = loadTexture("textures/grass.jpg");
    ciel  = loadTexture("textures/ciel.jpg");
    bois  = loadTexture("textures/bois.jpg");
    feuille=loadTexture("textures/feuille.jpg");
    cam = new Camera(0,0,10,20,4,0);

    flotte = new GColl;
    flotte->addTrain(Train(0,5,0.3,0,&graphe));
    flotte->addTrain(Train(1,4,0.1,2,&graphe));
    flotte->addTrain(Train(2,3,0.15,4,&graphe));


    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(Keybord);
    glutReshapeFunc(Resize);

    glutTimerFunc(mSPF,rotate,0);
    glutDisplayFunc(Dessiner);
    glutMainLoop();
    /*---------------------------------------------*/
    return EXIT_SUCCESS;
}

void Dessiner()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam->Look();

    glColor3f(0,0,0);
    drawGraphe();

    // herbe
    glColor3f(0.1,1,0.1);
    drawTree(-2,14,0,2.7);    drawTree(1,15,0,1.5);    drawTree(0,13,0,1.4);
    drawTree(13,5,0,2.5);    drawTree(11,5.5,0,1.7);    drawTree(9,4.5,0,1.6);
    drawTree(-4,14,0,1.5);    drawTree(11,7,0,2.0);    drawTree(-2.5,16.0,0,1.5);
    drawTree(13,10,0,1.5);    drawTree(20,7,0,1.5);    drawTree(23,7.5,0,1.5);

    glColor3d(1,1,1);
    glEnable(GL_TEXTURE_2D);
    // ciel
    glPushMatrix();
    glTranslatef(12.205,10.065,0.0);
    // glutSolidSphere(16,100,100);
    GLUquadricObj *sphere;
    sphere = gluNewQuadric();
    gluQuadricTexture(sphere,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D,ciel);
    gluSphere(sphere,20,100,100);
    gluDeleteQuadric(sphere);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // trains
    glColor3f(1,0.2,0.1);
    glPushMatrix();
    glTranslatef(0,0,0.25);
    flotte->drawTrains();
    glPopMatrix();


    glColor3d(1,1,1);
    // herbe
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, herbe);
    glBegin(GL_QUADS);
    glTexCoord2d(-10,-10);  glVertex3d(-10,-10,0); glColor3ub(255,255,255);
    glTexCoord2d(40,-10);  glVertex3d(40,-10,0); glColor3ub(255,255,255);
    glTexCoord2d(40,40);  glVertex3d(40,40,0); glColor3ub(255,255,255);
    glTexCoord2d(-10,40);  glVertex3d(-10,40,0); glColor3ub(255,255,255);
    glEnd();
    glDisable(GL_TEXTURE_2D);



    glColor3f(1,1,1);

    drawPont();

    glFlush();
    glutSwapBuffers();
}

void Initialiser()
{
    glutInitWindowSize(900,600);
    glutCreateWindow("Projet OpenGL - Hamza Biadi - Camille Briend - Elias Couppe");
    glClearColor(0.6,0.6,0.6,0.0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH) ;
    glPointSize(4);
    glLineWidth(2);
    glutFullScreen();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1.0,0.1,1000);
}

void Mouse(int button, int state, int x, int y)
{
    cam->Mouse(button, state, x, y);
}

void Motion(int x, int y)
{
    cam->Motion(x,y);
}

void Keybord(unsigned char key, int x, int y)
{
    cam->Keybord(key, x , y);
}

void drawGraphe()
{
    // début du dessin
    glPushMatrix();
    // on parcourt tous les arcs et on les dessine un par un
    for(int i=0; i<graphe.nb_arc; i++)
    {
        CArc arc = graphe.list_arc[i];
        int i_SI = arc.id_sommet_ini;
        int i_SF = arc.id_sommet_fin;
        int NPA = arc.list_point_annexe.size();
        // dessin de la sphère
        glPushMatrix();
        glTranslatef(graphe.list_sommet[i_SI].X,graphe.list_sommet[i_SI].Y,graphe.list_sommet[i_SI].Z);
        glColor3f(0.8,0.8,0.8);
        glutSolidSphere(0.1,50,50);
        glColor3d(0,0,0);
        glPopMatrix();
        glBegin(GL_LINE_STRIP);
        // sommet initial
        glVertex3f(graphe.list_sommet[i_SI].X,graphe.list_sommet[i_SI].Y,graphe.list_sommet[i_SI].Z);
        // points annexes
        for(int j=0; j<NPA; j++)
        {
            int iPA = arc.list_point_annexe.at(j);
            float x = graphe.list_point_annexe[iPA].X;
            float y = graphe.list_point_annexe[iPA].Y;
            float z = graphe.list_point_annexe[iPA].Z;
            glVertex3f(x,y,z);
        }
        // sommet final
        glVertex3f(graphe.list_sommet[i_SF].X,graphe.list_sommet[i_SF].Y,graphe.list_sommet[i_SF].Z);
        glEnd();
    }
    glPopMatrix();
    // fin du dessin
}

void Resize(int width, int height)
{
    float ratio = (float)width/(float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,ratio,0.1,1000);
    glMatrixMode(GL_MODELVIEW);
}

void drawTree(float x, float y, float z, float hauteur)
{

    glPushMatrix();
    glTranslatef(x,y,z);

    glColor3d(1,1,1);
    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *tronc;
    tronc = gluNewQuadric();
    gluQuadricTexture(tronc,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D,bois);
    gluCylinder(tronc,0.15,0.15,hauteur,100,100);
    gluDeleteQuadric(tronc);

    glTranslatef(0,0,hauteur);

    GLUquadricObj *sphere;
    sphere = gluNewQuadric();
    gluQuadricTexture(sphere,GL_TRUE);
    glBindTexture(GL_TEXTURE_2D,feuille);
    gluSphere(sphere,0.7,10,10);
    gluDeleteQuadric(sphere);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawPont()
{
    glColor3d(0,0,0);
    glBegin(GL_QUADS);
    glVertex3d(11,2.5,2);
    glVertex3d(10,2.5,0);
    glVertex3d(10,3.5,0);
    glVertex3d(11,3.5,2);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3d(13,2.5,2);
    glVertex3d(11,2.5,2);
    glVertex3d(11,3.5,2);
    glVertex3d(13,3.5,2);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3d(14,2.5,0);
    glVertex3d(13,2.5,2);
    glVertex3d(13,3.5,2);
    glVertex3d(14,3.5,0);
    glEnd();

}
