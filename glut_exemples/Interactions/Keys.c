
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>

/*  Fonctions Prototypes  */

GLvoid Initialise( char * );
GLvoid DrawScene();
GLvoid DrawGrid();
GLvoid Keyboard(unsigned char , int, int);
GLvoid Resize( GLsizei , GLsizei );
GLvoid PolarView(GLdouble,GLdouble,GLdouble,GLdouble);

/* Variables globales definissant les couleurs
   ------------------------------------------- */

GLfloat White[]={1.,1.,1.};
GLfloat Black[]={0.,0.,0.};
GLfloat Red[]={1.,0.,0.};
GLfloat Green[]={0.,1.,0.};
GLfloat Blue[]={0.,0.,1.};
GLfloat Magenta[]={1.,0.,1.};
GLfloat Cyan[]={0.,1.,1.};
GLfloat Yellow[]={1.,1.,0.};

/*  Variables globales definissant les dimensions
    --------------------------------------------- */

GLfloat aspect;
GLfloat near = 3.;
GLfloat far = 7.;
GLfloat fovy = 45.;
GLsizei WindowWidth=500,WindowHeight=500;
GLdouble    Radius,Radius_init;
GLdouble    Twist=0.;
GLdouble    Latitude=0.,Latitude_init;
GLdouble    Longitude=0.,Longitude_init;
GLdouble    MaxObjectsSize;
GLboolean   filledFlag=GL_TRUE;

/* Programme principal
   ------------------- */
int
main(int argc ,  char ** argv)
{
  glutInit(&argc,argv);
  Initialise(argv[0]);
  glutDisplayFunc(DrawScene);
  glutReshapeFunc(Resize);
  glutMainLoop();
  return 0;
}


GLvoid
Initialise( char * Titre )
{
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(500,500);
  glutCreateWindow(Titre);
  glClearColor(0.,0.,1.,1.);

  /* Definir le monde des coordonnees
     ------------------------------- */
  glPointSize(4);
  glLineWidth(2.5);
  glViewport( 0, 0,WindowWidth , WindowHeight) ;
  aspect = (GLfloat) WindowWidth / WindowHeight;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy,aspect,near,far);
  MaxObjectsSize=far-near;
  Radius=MaxObjectsSize /2. + near;
  Radius_init=Radius;
  Latitude_init=Latitude;
  Longitude_init=Longitude;
  glMatrixMode(GL_MODELVIEW);
}



GLvoid
Resize( int WindowWidth, int WindowHeight )
{
  glViewport( 0, 0,WindowWidth , WindowHeight) ;
  aspect = (GLfloat) WindowWidth / WindowHeight;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy,aspect,near,far);
  glMatrixMode(GL_MODELVIEW);
}


GLvoid
DrawScene()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();
    PolarView(Radius,Twist,Longitude,Latitude);
    glColor3fv(Cyan);
    if(filledFlag==GL_FALSE)
      glutSolidCube(2.);
    else glutWireCube(2.);

    glPushMatrix();
      glColor3fv(Yellow);
      glTranslatef(-1.,0.5,0.5);
      if(filledFlag==GL_FALSE)
	glutSolidSphere(0.5,18,18);
      else glutWireSphere(0.5,15,15);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-1.,0.,0.);
      glRotatef(90.,1.,0.,0.);
      DrawGrid() ;
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);

  glPopMatrix();
  glutSwapBuffers();
}

GLvoid
DrawGrid()
{
  GLboolean useblack=GL_TRUE;

  GLfloat x,y;

  for (y=-2.;y<2.;y=y+0.1)
    {
      for (x=-2.;x<2.;x=x+0.1)
	{
	  if (useblack==GL_TRUE)
	    {
	      glColor3fv(White);
	    }
	  else
	    {
	      glColor3fv (Black);
	    }
	  glRectf(x,y,x+0.1,y+0.1);
	  useblack=!useblack;
	}
      useblack=!useblack;
    }

}


GLvoid
PolarView(GLdouble Radius,GLdouble Twist,GLdouble Latitude,GLdouble Longitude)
{
  glTranslated(0.,0., -Radius);
  glRotated(-Twist,0.,0.,1.);
  glRotated(-Latitude,1.,0.,0.);
  glRotated(-Longitude,0.,0.,1.);
}


GLvoid
Keyboard(unsigned char key , int x, int y)
{
  switch(key)
    {

    case 27:
      exit(1);
      break;
    }
}
