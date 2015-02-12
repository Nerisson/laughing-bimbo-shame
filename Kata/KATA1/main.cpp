#include <windows.h>
#include <GL/glut.h>

#include <iostream>

using namespace std;

void reshape (int w, int h);

GLfloat RED[] =  {1., 0., 0., 1.};
GLfloat GREEN[] = {0., 1., 0., 1.};
GLfloat BLUE[] = {0., 0., 1., 1.};
GLfloat BLACK[] = {0., 0., 0., 1.};
GLfloat WHITE[] = {1., 1., 1., 1.};

int main(int argc, char **argv){
    void initialize(char *);
    void display(void);
    glutInit(&argc, argv) ; /*Initiase GLUT et traite les arguments de la ligne de commande*/
    initialize(argv[0]);
    glutDisplayFunc(display) ;
    glutMainLoop() ;
    return 0 ;
}

void initialize(char *title){
    void defineWindow(char *);
    glutInitDisplayMode(GLUT_RGB) ;
    glutInitDisplayMode(GLUT_RGB) ;
    defineWindow(title);
}

void defineWindow(char *title){
    GLsizei screenWidth, screenHeight  ;
    screenWidth= glutGet(GLUT_SCREEN_WIDTH);
    screenHeight  = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowPosition(screenWidth/4, screenHeight  /4);  glutInitWindowSize(screenWidth/2, screenHeight  /2);
    glutCreateWindow(title) ;
    glutReshapeFunc(reshape);
}


void display(void){
    glClear(GL_COLOR_BUFFER_BIT) ;
    glColor4fv(RED);
    glutSolidTeapot(.5);
    glFlush() ;
}

void reshape (int w, int h){
   cout << "Hello: width:" << w << " height:" << h << endl;
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w/ (GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
