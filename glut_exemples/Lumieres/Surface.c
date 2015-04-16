#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

/*  Fonctions Prototypes  */

GLvoid Initialise( char * );
GLvoid DrawScene( GLvoid );
GLvoid DrawGrid(GLvoid);
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int  , int );
GLvoid Resize( GLsizei , GLsizei );
GLvoid PolarView(GLdouble,GLdouble,GLdouble,GLdouble);

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
 
GLfloat aspect;
GLfloat near = 1.;
GLfloat far = 20.;
GLfloat fovy = 60.;
GLsizei WindowWidth=500,WindowHeight=500;
GLdouble    Radius,Radius_init;
GLdouble    Twist=0.;
GLdouble    Latitude=0.,Latitude_init;
GLdouble    Longitude=0.,Longitude_init; 
GLdouble    MaxObjectsSize; 
GLboolean   LightFlag=GL_TRUE;
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
  glutInit(&argc,argv);
  Initialise(argv[0]);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
  glutMotionFunc( MouseMove );
  glutDisplayFunc(DrawScene);
  glutReshapeFunc(Resize);
  glutMainLoop();
  return 0;
}


GLvoid
Initialise( char * Titre )
{
  GLvoid DefineWindow( char *) ;
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  /* Definir le monde des coordonnees
     ------------------------------- */

  glPointSize(4);
  glLineWidth(2.5);
  DefineWindow( Titre) ;  

  MaxObjectsSize=far-near;
  Radius=MaxObjectsSize /2. + near;
  Radius_init=Radius;
  Latitude_init=Latitude;
  Longitude_init=Longitude;
/* Definir une source lumineuse par defaut
     --------------------------------------- */
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);

}


GLvoid DefineWindow( char * Titre)
{
  
  GLsizei ScreenWidth,ScreenHeight;
  GLsizei WindowWidth, WindowHeight;

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

  glClearColor(0.,0.,0.,1.);

  /* Trouver les dimensions de la fenetre
     ------------------------------------ */


   WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
   WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

   Resize(WindowWidth,WindowHeight);
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
DrawScene( GLvoid )
{
  GLfloat   sphereAmbient[] = {0.0877551,0.0639456,0.183051,1.  };
  GLfloat   sphereDiffuse[] = { 0.35102, 0.255782 ,0.732203,1.};
  GLfloat   sphereSpecular[] = {0.374738 ,0.273065,0.781677,1.};

  GLfloat   torusAmbient[] = { 0.00746438, 0.00673081, 0.00690282  ,1.};
  GLfloat   torusDiffuse[] = {0.0291577, 0.0262922, 0.0269642,1.}; 
  GLfloat   torusSpecular[] = { 0.641609, 0.976208, 0.979592,1.};


  GLfloat   cubeAmbient[] = {0.187004, 0., 0.0100218,0.359073};
  GLfloat   cubeDiffuse[] = { 0.748016, 0., 0.0400871,0.359073};
  GLfloat   cubeSpecular[] = {0.89697, 0.89697, 0.89697,0.359073};

  GLvoid DrawAxes(GLvoid);

  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
  
 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();
    PolarView(Radius,Twist,Longitude,Latitude);

    if ( LightFlag == GL_TRUE )
      glEnable(GL_LIGHTING) ;
    else glDisable(GL_LIGHTING) ;


       
	
	   
	   /* sphere
              ------ */

    glColor3fv(Cyan); 	   
    
    glPushMatrix();
      glTranslatef( -2.0, 1.5, 0.0 );
      glutSolidSphere( 2.0,30,30 );
    glPopMatrix();
    	   
	   /* tore
              ---- */

    glColor3fv(Magenta); 
    
    glPushMatrix();
      glTranslatef( 2.5, 0.0, 0.0 );
      glutSolidTorus( 0.5, 1.25 ,20,20);
    glPopMatrix();
    	   
	   /* cube 
             ---- */

    glColor4f(0.5,0.5,1.,0.6);
    
    glPushMatrix();
     glTranslatef( 0.0, 2.0, 2.0 );
     glutSolidCube( 2. );
    glPopMatrix();
       
    if ( LightFlag == GL_TRUE ) glDisable(GL_LIGHTING) ;

    glDisable(GL_DEPTH_TEST);
    DrawAxes() ;		
    glPopMatrix();
	  
/* Force a la bonne execution de l'affichage
     ----------------------------------------- */
  glutSwapBuffers(); 

}

GLvoid
DrawGrid(GLvoid)
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
    case 'f':
      LightFlag=!LightFlag;
      glutPostRedisplay();
      break;

    case 'r':
      Latitude+=10;
      glutPostRedisplay();
      break;

    case 'l':
      Latitude-=10;
      glutPostRedisplay();
      break;  

    case 'u':
      Longitude+=10;
      glutPostRedisplay();
      break;   

     case 'd':
      Longitude-=10;
      glutPostRedisplay();
      break;   

     case 'z':
      Radius-=0.1;
      glutPostRedisplay();
      break; 

    case 'Z':
      Radius+=0.1;
      glutPostRedisplay();
      break; 

    case 's':
      Radius=Radius_init;
      Longitude=Longitude_init;
      Latitude=Latitude_init;
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
  if ( Flag == ROTATE )
    {
      WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
      WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
      Latitude+=(xoffset* 360.) / WindowWidth;
      Longitude+=(yoffset*360.) / WindowHeight;
      xoffset=x-xstart;
      yoffset=y-ystart;

      if ( Latitude > 360. || Latitude < -360. ) Latitude=0. ; 
      if ( Longitude > 360. || Longitude < -360. ) Longitude=0.; 
      xstart=x;
      ystart=y; 
    }
  else if ( Flag == ZOOM_IN )
    Radius-=0.05;
  else
   Radius+=0.05;
  glutPostRedisplay(); 
 }

GLvoid
Mouse(int button , int state , int x , int y)
{
  xstart = x;
  ystart = y;
  if ( state == GLUT_DOWN )
    {
      if (  button == GLUT_LEFT_BUTTON  )
          Flag = ROTATE ;
      else if (button == GLUT_MIDDLE_BUTTON ) 
	  Flag = ZOOM_IN ;

      else 
          Flag = ZOOM_OUT ;      
    }
}
      

GLvoid
DrawAxes( GLvoid )
{
  glLineWidth( 3.0 );
  
  /* Axes  X bleu     */
  glColor3fv(Blue);
    
  glPushMatrix();
    glTranslatef(1.0,0.0,0.0);
    glRotatef(90.,0.,1.,0.) ; 
    glutSolidCone( 0.05, 0.1 , 5, 5 );
  glPopMatrix();

  glBegin( GL_LINES );                       
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( 1.0, 0.0, 0.0 );
  glEnd();

  /* Axes  Y vert     */
  glColor3fv(Green);

  glPushMatrix();
    glTranslatef(0.0,1.0,0.0);
    glRotatef(-90.,1.,0.,0.) ;
    glutSolidCone( 0.05, 0.1 , 5, 5);
  glPopMatrix();

  glBegin( GL_LINES );                       
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( 0.0, 1.0, 0.0 );
  glEnd();

  /* Axes  Z rouge     */
  glColor3fv(Red);
  glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glutSolidCone( 0.05, 0.1 ,5 ,5);
  glPopMatrix();

  glBegin( GL_LINES );                       
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( 0.0, 0.0, 1.0 );
  glEnd();
}














