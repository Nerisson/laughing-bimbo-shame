#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
/*  Fonctions Prototypes  */

GLvoid Initialise( char * );
GLvoid DrawScene();
GLvoid DrawGrid();
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int  , int );
GLvoid Resize( GLsizei , GLsizei );
GLvoid PilotView();
/* Variables globales definissant les couleurs
   ------------------------------------------- */

GLfloat White[]={1.,1.,1.,1.};
GLfloat Black[]={0.,0.,0.,1.};
GLfloat Red[]={1.,0.,0.};
GLfloat Green[]={0.,1.,0.};
GLfloat Blue[]={0.,0.,1.};
GLfloat Magenta[]={1.,0.,1.};
GLfloat Cyan[]={0.,1.,1.,0.6};
GLfloat Yellow[]={1.,1.,0.,0.6};

/*  Variables globales definissant les dimensions
    --------------------------------------------- */

GLfloat Ratio;
GLfloat near = 1.;
GLfloat far = 20.;
GLfloat fovy = 45.;
GLfloat xtrans,ytrans,ztrans,xangle,yangle,zangle;
GLdouble    MaxObjectsSize;
GLboolean   filledFlag=GL_TRUE;

int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

/* Programme principal
   ------------------- */

int
main(int argc ,  char ** argv)
{
  int error = 0;
  glutInit(&argc,argv);
  Initialise(argv[0]);
  glutKeyboardFunc(Keyboard);
  glutDisplayFunc(DrawScene);
  glutReshapeFunc(Resize);
  glutMainLoop();
  return error;
}


GLvoid
Initialise( char * Titre )
{
  GLvoid DefineWindow( char *) ;

  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );


/* Définition Sde la fenêtre
     ------------------------ */

  DefineWindow( Titre) ;


  /* Taille des points et lignes
     --------------------------- */

  glPointSize(4);
  glLineWidth(2.5);

  MaxObjectsSize=far-near;
  xtrans = 0.;
  ytrans = 0.;
  ztrans = MaxObjectsSize/2. + near;
  xangle = 0.;
  yangle = 0.;
  zangle = 0;
}

GLvoid
Resize(GLsizei WindowWidth , GLsizei WindowHeight  )
{
  glViewport( 0, 0,WindowWidth , WindowHeight) ;

  Ratio = (GLfloat)  WindowWidth /  WindowHeight;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy,Ratio,near,far);
  glMatrixMode(GL_MODELVIEW);
}

GLvoid DefineWindow( char * Titre)
{

  GLsizei ScreenWidth,ScreenHeight;


  /* Largeur et hauteur de l'écran
     -------------------------------- */


/* Largeur et hauteur de l'écran
   ---------------------------------- */

   ScreenWidth  = glutGet(GLUT_SCREEN_WIDTH) ;
   ScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);


/* Definition de la fenetre
     ------------------------ */

  glutInitWindowPosition(ScreenWidth/4,ScreenHeight/4);
  glutInitWindowSize(ScreenWidth/2,ScreenHeight/2);

  glutCreateWindow(Titre);

/* Definition de la couleur du fond de la fenetre
   ----------------------------------------------- */

  glClearColor(0.,0.,1.,1.);

}






GLvoid
DrawScene()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();
  PilotView();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
      glTranslatef(-1.,0.,0.);
      glRotatef(90.,1.,0.,0.);
      DrawGrid() ;
    glPopMatrix();

   glPushMatrix();
      glColor4fv(Yellow);
      glTranslatef(-1.,0.5,0.5);
      if(filledFlag==GL_FALSE)
	glutSolidSphere(0.5,18,18);
      else glutWireSphere(0.5,15,15);
    glPopMatrix();


    glColor4fv(Cyan);
    if(filledFlag==GL_FALSE)
      glutSolidCube(2.);
    else glutWireCube(2.);


  glPushMatrix();
      glColor4fv(Yellow);
      glTranslatef(-1.,0.5,0.5);
      if(filledFlag==GL_FALSE)
	glutSolidSphere(0.5,18,18);
      else glutWireSphere(0.5,15,15);
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
	      glColor4fv(White);
	    }
	  else
	    {
	      glColor4fv (Black);
	    }
	  glRectf(x,y,x+0.1,y+0.1);
	  useblack=!useblack;
	}
      useblack=!useblack;
    }

}


GLvoid
PilotView()
{
  glTranslated(xtrans,ytrans,-ztrans);
  glRotated(zangle,0.,0.,1.);
  glRotated(xangle,1.,0.,0.);
  glRotated(yangle,0.,0.,1.);
}


GLvoid
Keyboard(unsigned char key , int x, int y)
{
  switch(key)
    {
    case 'f':
      filledFlag=!filledFlag;
      glutPostRedisplay();
      break;

    case 't':
      zangle+=1;
      glutPostRedisplay();
      break;

    case 'T':
      zangle-=1;
      glutPostRedisplay();
      break;

    case 'u':
      xangle+=1;
      glutPostRedisplay();
      break;

     case 'd':
      xangle-=1;
      glutPostRedisplay();
      break;

    case 'r':
      yangle+=1;
      glutPostRedisplay();
      break;

     case 'l':
      yangle-=1;
      glutPostRedisplay();
      break;

     case 'Z':
      ztrans-=1.;

      glutPostRedisplay();
      break;

    case 'z':
      ztrans+=1.;
      glutPostRedisplay();
      break;
    case 'X':
      xtrans-=0.1;
       glutPostRedisplay();
      break;

    case 'x':
      xtrans+=0.1;
      glutPostRedisplay();
      break;

    case 'Y':
      ytrans-=0.1;
      glutPostRedisplay();
      break;

    case 'y':
      ytrans+=0.1;
      glutPostRedisplay();
      break;

    case 27:
      exit(1);
      break;
    }
}

GLvoid
MouseMove(int x , int y)
{
  GLsizei WindowWidth,WindowHeight;
  WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
  WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  glutPostRedisplay();
 }

GLvoid
Mouse(int button , int state , int x , int y)
{
}








