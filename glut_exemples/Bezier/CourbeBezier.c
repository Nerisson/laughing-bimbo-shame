#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
/*  Fonctions Prototypes 
    -------------------- */

GLvoid Initialise( char * );
GLvoid DrawScene( GLvoid );
GLvoid DrawGrid(GLvoid);
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int, int);
GLvoid Resize( GLsizei , GLsizei );
GLvoid PilotView(GLvoid);

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
GLfloat near = 3.;
GLfloat far = 12.;
GLfloat fovy = 60.;

GLdouble    MaxObjectsSize; 
GLboolean   LightFlag=GL_FALSE;
int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
GLfloat xtrans,ytrans,ztrans,xangle,yangle,zangle;

enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;
/* Points de Controle de la courbe de Bezier
   ----------------------------------------- */

#define  Number  4 

typedef struct BezierCurv {
  GLint Degree ;
  GLint ControlNumber ;
  GLfloat   ControlPoints[Number][3] ;
} BezierCurve ;

GLvoid InitialiseBezierCurve ( GLint ,
			       GLfloat A[][3] ,
			       BezierCurve *    ) ;

GLfloat GivenPoints1[4][3] = { { -1.,-1.,1.} , {-1.,1.,-1.},
                        {1.,1.,1.} , { 1.,-1.,-1.} } ; 

GLfloat GivenPoints2[4][3] = { { -1.,-1.,1.} , {1.,1.,-1.},

                        {-1.,1.,1.} , { 1.,-1.,-1.} } ; 
GLint PointsNumber = 30 ;

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

  
  DefineWindow( Titre) ;  

  MaxObjectsSize=far-near;
  xtrans = 0.;
  ytrans = 0.;
  ztrans = MaxObjectsSize/2. + near;
  xangle = 0.;
  yangle = 0.;
  zangle = 0;
  

/* Definir une source lumineuse par defaut
     --------------------------------------- */

 glEnable(GL_NORMALIZE);
 glEnable(GL_LIGHT0);
 glPointSize(4.);
 glLineWidth(1.);
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

  glClearColor(0.,0.,0.,1.);

}


GLvoid
Resize( int WindowWidth , int WindowHeight )
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
 GLvoid DrawAxes( GLvoid ) ; 

  GLvoid DrawBezierCurve ( GLfloat * ,
			  GLfloat * ,
			  BezierCurve * )   ;
  GLint i;
  BezierCurve Curve1 ;
  BezierCurve Curve2 ;
  GLfloat DomainBounds[2] ;
  
  DomainBounds[0] = 0. ;
  DomainBounds[1] = 1. ;

  InitialiseBezierCurve( 3 ,
                         GivenPoints1,
                         &Curve1 ) ;

  InitialiseBezierCurve( 3 ,
                         GivenPoints2,
                         &Curve2 ) ;


  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
    
 
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  
  glPushMatrix();
  PilotView();
    if ( LightFlag == GL_TRUE )
      glEnable(GL_LIGHTING) ;
    else glDisable(GL_LIGHTING) ;
       
    DrawBezierCurve( Yellow ,
                     DomainBounds,
                     &Curve1);

    DrawBezierCurve( Cyan ,
                     DomainBounds,
                     &Curve2);

    glDisable(GL_DEPTH_TEST);
    DrawAxes() ;		
    glPopMatrix();
  
/* Force a la bonne execution de l'affichage
     ----------------------------------------- */

  glutSwapBuffers(); 

}
GLvoid InitialiseBezierCurve ( GLint GivenDegree ,
			       GLfloat GivenPoints[][3] ,
			       BezierCurve * Curve  ) 
{
  int i,j;
  Curve->Degree = GivenDegree ;
  Curve->ControlNumber = GivenDegree + 1 ;
  for ( i = 0 ; i <  Curve->ControlNumber ; i++ )
    {
      for ( j = 0 ; j < 3 ; j++ ) 
	{
	  Curve->ControlPoints[i][j] = GivenPoints[i][j] ;
	}
    }
}

GLvoid DrawBezierCurve ( GLfloat * Color,
                         GLfloat * DomainBounds,
                         BezierCurve * Curve_ptr )
{
  GLint i;
  GLfloat Parameter;
  
  glBegin(GL_POINTS);
    for ( i=0;i < Curve_ptr->ControlNumber;i++)
      {
	glVertex3fv(Curve_ptr->ControlPoints[i]);
      }
  glEnd();
 
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
PilotView(GLvoid)
{
  glTranslatef(-xtrans,-ytrans,-ztrans);
  glRotatef(zangle,0.,0.,1.);
  glRotatef(xangle,1.,0.,0.);
  glRotatef(yangle,0.,1.,0.);
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

     case 'z':
      ztrans-=0.1;
      if (ztrans < 0.1 ) ztrans = 0.1; 
      glutPostRedisplay();
      break; 

    case 'Z':
      ztrans+=0.1;
      glutPostRedisplay();
      break; 
    case 'x':
      xtrans-=0.1;
      glutPostRedisplay();
      break; 

    case 'X':
      xtrans+=0.1;
      glutPostRedisplay();
      break;
 
    case 'y':
      ytrans-=0.1;
      glutPostRedisplay();
      break; 

    case 'Y':
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
  if ( Flag == ROTATE )
    {
      GLsizei WindowWidth,WindowHeight;
      WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
       WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
      xangle+=(xoffset* 36.) / WindowWidth;
      yangle+=(yoffset*36.) / WindowHeight;
      xoffset=x-xstart;

      yoffset=y-ystart;

      if ( xangle > 360. || xangle < -360. ) xangle=0. ; 
      if ( yangle > 360. || yangle < -360. ) yangle=0.; 
     xstart=x;
     ystart=y; 
    }
  else if ( Flag == ZOOM_IN )
    ztrans-=0.05;
  else
   ztrans+=0.05;
  glutPostRedisplay(); 
 }

GLvoid
Mouse(int button , int state , int x , int y)
{
  xstart=x;
  ystart=y; 
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
  glLineWidth( 1.0 );
  
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














