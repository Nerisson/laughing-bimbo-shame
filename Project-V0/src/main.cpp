#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include "API/src/CDonneesGraphe.h"
#include "API/src/CGraphe.h"
#include "API/src/CSommet.h"

#include "Train.h"
#include "Flotte.h"
#include "MNT.h"

#include <string>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;

// XZ position of the camera
//float x=0.0f, z=0.0f;
float x=14.55f, z=8.5f;

// the key states. These variables will be zero
// when no key is being pressesed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

int camera = 0;
float angleCamera;
int cameraTrain = 0;
int nTrains = 5;

// scale of snowman
float scale = 1.0f;


string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

string path = ExePath();
string::size_type loc = path.find( "\\bin\\Debug", 0 );
string path2 = path.substr(0, loc+1)+"API\\";
CDonneesGraphe gdata(path2+"SXYZ.TXT", path2+"SIF.TXT", path2+"PAXYZ.TXT", path2+"AXYZ.TXT");
CGraphe graphe(gdata);

Flotte flotte(&graphe);

MNT mnt(path2+"ZI.dat");

// width and height of the window
int h,w;

void init();

void changeSize(int ww, int hh) {

	h = hh;
	w = ww;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 200.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {
    // color for the snowman's nose
    float red = 1.0f, blue=0.5f, green=0.5f;

	glScalef(scale, scale, scale);
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(red, green, blue);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);

	glColor3f(1.0f, 1.0f, 1.0f);
}

void drawGraphe(){

    //Draw Sommets
	for(int i=0; i<graphe.nb_sommet; i++){
	    CSommet sommet = graphe.list_sommet.at(i);
        glPushMatrix();
        glTranslatef(sommet.X, sommet.Z, sommet.Y);
        glutSolidSphere(0.01f,20,20);
        glPopMatrix();
	}



    //Draw Arcs
    for(int i=0; i<graphe.nb_arc; i++){
        CArc arc = graphe.list_arc.at(i);
        glLineWidth(2.5);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
            CSommet s = graphe.list_sommet.at(arc.id_sommet_ini);
            glVertex3f(s.X, s.Z, s.Y);
            for(unsigned int j=0; j<arc.list_point_annexe.size(); j++){
                CPointAnnexe pa = graphe.list_point_annexe.at(arc.list_point_annexe.at(j));
                glVertex3f(pa.X, pa.Z, pa.Y);
            }
            s = graphe.list_sommet.at(arc.id_sommet_fin);
            glVertex3f(s.X, s.Z, s.Y);
        glEnd();
    }


}

void drawFlotte(){
    flotte.draw();
}

void drawFeuillu(float x, float z, float y, float hauteur){


    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(-90,  1,  0,  0);
    glColor3d(0.5,0.3,0);
    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *tronc;
    tronc = gluNewQuadric();
    gluQuadricTexture(tronc,GL_TRUE);
    gluCylinder(tronc,0.20,0.18,hauteur,100,100);
    gluDeleteQuadric(tronc);

    glTranslatef(0,0,hauteur);

    glColor3d(0,0.8,0);
    GLUquadricObj *feuilles;
    feuilles = gluNewQuadric();
    gluQuadricTexture(feuilles,GL_TRUE);
    gluSphere(feuilles,0.7,12,12);
    gluDeleteQuadric(feuilles);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawConnifere (float x, float z, float y, float hauteur){


    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(-90,  1,  0,  0);
    glColor3d(0.5,0.3,0);
    glEnable(GL_TEXTURE_2D);
    GLUquadricObj *tronc;
    tronc = gluNewQuadric();
    gluQuadricTexture(tronc,GL_TRUE);
    gluCylinder(tronc,0.20,0.18,hauteur+0.3,100,100);
    gluDeleteQuadric(tronc);

    glTranslatef(0,0,hauteur);

    glColor3d(0,0.8,0);
    GLUquadricObj *feuilles;
    feuilles = gluNewQuadric();
    gluQuadricTexture(feuilles,GL_TRUE);
    glutSolidCone(1,2,20,4);
    gluDeleteQuadric(feuilles);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void drawNuages (float x, float z, float y){


    glPushMatrix();
    glTranslatef(x,y,z);
    glRotatef(-90,  x,  0,  1);
    glEnable(GL_TEXTURE_2D);

    glColor3d(1.,1.,1.0);
    GLUquadricObj *nuage;
    nuage = gluNewQuadric();
    //On construit les nuages comme une succession de sph�res l�g�rement d�cal�es
    for (int i = 1; i<12; i++){
        gluQuadricTexture(nuage,GL_TRUE);
        glutSolidSphere(0.7,12,12);
        if (i & 1){
            glTranslatef(.1,.1,.3);
            if (i/2 & 1){
                glTranslatef(.1,-0.1,-0.2);
            }
        }
        else {
            glTranslatef(0.1,0.2,-0.3);
        }
    }
    gluDeleteQuadric(nuage);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawHouse(float x, float y, float z){
    glTranslatef(x, y, z);

    //pav�
    glColor3f(0.8f,0.8f,0.8f );
    glutSolidCube(1);

     //Porte
     glColor3f(0.8f, 0.4f, 0.0f);
     glBegin(GL_QUADS);
        glVertex3f( 0.51f, .0f, 0.2f);
        glVertex3f( 0.51f, -0.5f, 0.2f);
        glVertex3f( 0.51f, -0.5f, -0.2f);
        glVertex3f( 0.51f, .0f, -0.2f);
    glEnd();

    //toit
    glBegin(GL_TRIANGLES);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f( 0.0f, 1.0f, 0.0f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(-0.5f, 0.5f, 0.5f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.5f, 0.5f, 0.5f);

         glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.0f, 1.0f, 0.0f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.5f, 0.5f, 0.5f);
         glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.5f, 0.5f, -0.5f);

          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.0f, 1.0f, 0.0f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(0.5f, 0.5f, -0.5f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(-0.5f, 0.5f, -0.5f);

         glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f( 0.0f, 1.0f, 0.0f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(-0.5f,0.5f,-0.5f);
          glColor3f(0.8f, 0.4f, 0.0f);
          glVertex3f(-0.5f,0.5f, 0.5f);
   glEnd();


   glTranslatef(-x, -y, -z);
}

void drawDecor(){
    drawFeuillu(8.7,9.5,0,1.7); // au sol
    drawFeuillu(13,5,1.3,2.5);
    drawFeuillu(11,5.5,0.8,1.7);
    drawFeuillu(11,7,0.1,2.0); // presque au sol
    drawFeuillu(13,10,0,2.3); // au sol
    drawFeuillu(20,7,0,1.9);
    drawFeuillu(1.2,3,0,1.8); // au sol
    drawFeuillu(2,2,0,1.9); // au sol

    drawConnifere(21,5,0,0.4);
    drawConnifere(18,6,0,0.4);
    drawConnifere(19,7,0.,0.4);
    drawConnifere(23,7.5,0.5,.5); // au sol
    drawConnifere(24,6.9,0,.5); // au sol

    drawNuages(15,15,4.5);
    drawNuages(13,5,5.2);
    drawNuages(10,2,5.2);
    drawNuages(8,1,5);

    drawHouse(8.0f, 0.5f, 2.0f);
    drawHouse(10.0f, 0.5f, 10.0f);
    drawHouse(5.5f, 0.5f, -.5f);
    drawHouse(3.f, 0.5f, -.2f);
}


void drawMNT(){

    glColor3f(115.0f/255.0f,84.0f/255.0f,36.0f/255.0f);

    glBegin(GL_QUADS);
    for(int i=0; i<49; i++ ){
        for(int j=0; j<49; j++){
//            cout << min_x << " " << mnt.get(i*50+j).X << " " << mnt.get(i*50+j).X*dx << endl;
            glVertex3f((mnt.get(i*50+j).X),       mnt.get(i*50+j).Z,      (mnt.get(i*50+j).Y));
            glVertex3f((mnt.get(i*50+j+1).X),     mnt.get(i*50+j+1).Z,    (mnt.get(i*50+j+1).Y));
            glVertex3f((mnt.get((i+1)*50+j+1).X), mnt.get((i+1)*50+j+1).Z,(mnt.get((i+1)*50+j+1).Y));
            glVertex3f((mnt.get((i+1)*50+j).X),   mnt.get((i+1)*50+j).Z,  (mnt.get((i+1)*50+j).Y));
        }
    }
    glEnd();
}

void drawScene(){
    drawDecor();
    drawMNT();
}


void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;

}

void look(){
    switch(camera){
        case(0):
            gluLookAt(	x, 0.5,  z,
			    13.9491, 0.5,  9.29935,
			    0.0f, 1.0f, 0.0f);
        break;
        case(1):
            gluLookAt(	x, 0.5,  z,
			    x+lx, 0.5,  z+lz,
			    0.0f, 1.0f, 0.0f);
        break;
        case(2):
            gluLookAt(	14.55, 40.0,  8.5,
			    14.55, 39.0,  8.5,
			    0.0f, 0.0f, 1.0f);
        break;
        case(3):
            float alpha = atan(flotte.getTrain(cameraTrain).getAy());
//            cout << alpha << " " << flotte.getTrain(0).getAy() << endl;

            if(flotte.getTrain(cameraTrain).getPointPrec().X>flotte.getTrain(cameraTrain).getPointSuiv().X){
                alpha = (alpha + M_PI);
                if(alpha>2*M_PI)
                    alpha-=2*M_PI;
            }

            if(angleCamera>alpha)
                angleCamera -= min(0.0005f, angleCamera-alpha);
            else if(angleCamera<alpha)
                angleCamera += min(0.0005f, alpha-angleCamera);
//            float px = flotte.getTrain(0).GetPreviousX();
//            float py = flotte.getTrain(0).GetPreviousY();
//            float pz = flotte.getTrain(0).GetPreviousZ();
            float x = flotte.getTrain(cameraTrain).Getx();
            float y = flotte.getTrain(cameraTrain).Gety();
            float z = flotte.getTrain(cameraTrain).Getz();
            gluLookAt(	  x, z+0.2,   y,
                           x+cos(angleCamera),  z+0.2,    y+sin(angleCamera),
                        0.0f,   1.0f, 0.0f);
        break;
    }



}

void renderScene(void) {


//    cout << x+lx << " " << z+lz << endl;

	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();


	// Set the camera

	look();


    // Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-1000.0f, 0.0f, -1000.0f);
		glVertex3f(-1000.0f, 0.0f,  1000.0f);
		glVertex3f( 1000.0f, 0.0f,  1000.0f);
		glVertex3f( 1000.0f, 0.0f, -1000.0f);
	glEnd();


    drawScene();

    drawGraphe();
    flotte.move();
    drawFlotte();

	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
	    case 'c':
            camera = camera + 1;
            camera = camera % 4;
	    break;
	    case 'n':
            cameraTrain = cameraTrain + 1;
            cameraTrain = cameraTrain % nTrains;
	    break;
		case 27:
			exit(0);
			break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_UP    : deltaMove  =  0.5f;  break;
		case GLUT_KEY_DOWN  : deltaMove  = -0.5f;  break;
		case GLUT_KEY_LEFT  : deltaAngle =  0.01; break;
		case GLUT_KEY_RIGHT : deltaAngle = -0.01; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP    :
		case GLUT_KEY_DOWN  : deltaMove = 0; break;
		case GLUT_KEY_LEFT  :
		case GLUT_KEY_RIGHT : deltaAngle = 0; break;
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}


void init() {

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glClearColor(0.66f, 0.91f, 1.0f, 0.0f);
}

void initFlotte(int n){
    flotte.initFlotte(5);
}

int main(int argc, char **argv) {

    initFlotte(nTrains);

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600,100);
	glutInitWindowSize(800,600);
	glutCreateWindow("Projet-V0");

	// register callbacks
	init();


	// enter GLUT event processing cycle
	glutMainLoop();



	return 0;
}
