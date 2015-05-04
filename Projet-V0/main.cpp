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

#include <string>

#include <windows.h>
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
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

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
        glutSolidSphere(0.2f,20,20);
        glPopMatrix();
	}



    //Draw Arcs
    for(int i=0; i<graphe.nb_arc; i++){
        CArc arc = graphe.list_arc.at(i);
        glLineWidth(2.5);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINE_STRIP);
            CSommet s = graphe.list_sommet.at(arc.id_sommet_ini);
            glVertex3f(s.X, 0.1, s.Y);
            for(unsigned int j=0; j<arc.list_point_annexe.size(); j++){
                CPointAnnexe pa = graphe.list_point_annexe.at(arc.list_point_annexe.at(j));
                glVertex3f(pa.X, 0.1, pa.Y);
            }
            s = graphe.list_sommet.at(arc.id_sommet_fin);
            glVertex3f(s.X, 0.1, s.Y);
        glEnd();
    }


}

void drawFlotte(){
    flotte.draw();
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

void renderScene(void) {



	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();


	// Set the camera
	gluLookAt(	x, 40.0f,  z,
			    x, 39.0f,  z,
			    0.0f, 0.0f, 1.0f);

    // Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-1000.0f, 0.0f, -1000.0f);
		glVertex3f(-1000.0f, 0.0f,  1000.0f);
		glVertex3f( 1000.0f, 0.0f,  1000.0f);
		glVertex3f( 1000.0f, 0.0f, -1000.0f);
	glEnd();

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
    flotte.initFlotte(n);
}

int main(int argc, char **argv) {

    initFlotte(5);

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
