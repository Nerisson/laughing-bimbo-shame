#include <windows.h>
#include <iostream>
#include <GL/glut.h>


using namespace std;

double rotAngle = 10;

GLfloat trans[] =  {0., 0., 0.,};


void init()
{
    glClearColor(0, 0, 0, 0);		// background color
    glClearDepth(1.0);			// background depth value

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 1000);	// setup a perspective projection

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(				// set up the camera
		0.0, 0.0, 5.0,		// eye position
		0.0, 0.0, 0.0,		// lookat position
		0.0, 1.0, 0.0);		// up direction

    glEnable(GL_DEPTH_TEST);		// enable hidden surface removal

    glEnable(GL_LIGHTING);		// enable lighting
    glEnable(GL_LIGHT0);		// enable

    float lpos[] = { 5, 5, 5, 0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    // glShadeModel(GL_FLAT);		// flat shading
    glShadeModel(GL_SMOOTH);		// smooth shading
}

//-----------------------------------------------------------------------
// display callback function
//	This is called each time application needs to redraw itself.
//	Most of the opengl work is done through this function.
//-----------------------------------------------------------------------

void display()
{
    glClear(
	GL_COLOR_BUFFER_BIT |		// clear the frame buffer (color)
	GL_DEPTH_BUFFER_BIT);		// clear the depth buffer (depths)



    glPushMatrix();			// save the current camera transform

    glRotated(rotAngle, 0, 1, 0);	// rotate by rotAngle about y-axis
    glTranslatef(trans[0], trans[1], trans[2]);

    glEnable(GL_COLOR_MATERIAL);	// specify object color
    glColor3f(1.0, 0.1, 0.1);		// redish

    glutSolidTeapot(1);			// draw the teapot

    glPopMatrix();			// restore the modelview matrix
    glFlush();				// force OpenGL to render now


 //   cout << ""


    glutSwapBuffers();			// make the image visible
}

//-----------------------------------------------------------------------
// keyboard callback function
//	This is called whenever a keyboard key is hit.
//-----------------------------------------------------------------------

void keyboard(unsigned char k, int x, int y)
{
    cout << "keyboard:" << k << endl;
    switch (k)
    {
    case 'q':
        trans[0] -= 0.1;			// increase rotation by 5 degrees
	break;
    case 'd':
        trans[0] += 0.1;			// decrease rotation by 5 degrees
	break;
   case 'z':
        trans[2] += 0.1;			// decrease rotation by 5 degrees
	break;
   case 's':
        trans[2] -= 0.1;			// decrease rotation by 5 degrees
       case 'a':
        rotAngle += 5;			// decrease rotation by 5 degrees
	break;
   case 'e':
        rotAngle -= 5;			// decrease rotation by 5 degrees


	break;
        case 27:
	exit(0);			// exit
    }

    glutPostRedisplay();		// redraw the image now
}

//-----------------------------------------------------------------------
//  usage
//	Explain how to use program.
//-----------------------------------------------------------------------

void usage()
{
    cout << "\n\
-----------------------------------------------------------------------\n\
  CMSC 427 Sample Program.\n\
  Inputs:\n\
    a:              Rotate counterclockwise\n\
    l:              Rotate clockwise\n\
    q:              Quit\n\
  You may need to place the cursor over the graphics window for\n\
  keyboard input to be processed.\n\
-----------------------------------------------------------------------\n";
    cout.flush();
}

//-----------------------------------------------------------------------
// main program
//	Where everything begins.
//-----------------------------------------------------------------------

int main()
{
    usage();				// explain how to use
    glutInitDisplayMode(		// initialize GLUT
		GLUT_DOUBLE |		// use double buffering
		GLUT_DEPTH |		// request memory for z-buffer
		GLUT_RGB );		// set RGB color mode

    glutCreateWindow("GLUT Example");	// create the window

    glutDisplayFunc(display);		// call display() to redraw window
    glutKeyboardFunc(keyboard);		// call keyboard() when key is hit

    init();				// our own initializations

    glutMainLoop();			// let GLUT take care of everything
    return 0;
}
