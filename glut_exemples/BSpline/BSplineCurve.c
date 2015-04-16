#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
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
GLfloat far = 20.;
GLfloat fovy = 70.;
GLsizei WindowWidth,WindowHeight;
GLdouble    Radius,Radius_init;
GLdouble    Twist=0.;
GLdouble    Latitude=0.,Latitude_init;
GLdouble    Longitude=0.,Longitude_init; 
GLdouble    MaxObjectsSize; 
int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

/* Donnees de la Courbe BSPline
   ---------------------------- */
#define CONTROLNUMBER 9
#define KNOTSNUMBER 13
GLfloat TheGivenPoints[9][3] = { {-2.,1.,0.},{-1.,0.,1.},{-3.,0.,0.},{-3.,3.,1.},
                              {0.,5.,0.},{3.,3.,-1.},{3.,0.,0.},{1.,0.-1.},{2.,1.,0.}} ;

GLfloat TheGivenKnots[13]={0.,0.,0.,0.,0.25,0.25,0.5,0.75,0.75,1.,1.,1.,1.};

typedef struct BsplineCurv {
  GLint Dimension ;
  GLint Degree ;
  GLint ControlNumber ;
  GLint Order ;
  GLint KnotsNumber ;
  GLfloat  Knots[KNOTSNUMBER] ;
  GLfloat  ControlPoints[CONTROLNUMBER][3] ;
} BsplineCurve, * BsplineCurvePtr ;

  GLUnurbsObj * TheNurbs ;

GLfloat dx = 0. ;
GLfloat dy = 0. ;
GLfloat dz = 0. ;
GLint Index=0 ;

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
  glPointSize(5.);
  glLineWidth(2.5);
  MaxObjectsSize=far-near;
  Radius=MaxObjectsSize /2. + near;
  Radius_init=Radius;
  Latitude_init=Latitude;
  Longitude_init=Longitude;
/* Rajouter l initialisation du pointeur TheNurbs
     ---------------------------------------------- */
  TheNurbs = gluNewNurbsRenderer() ;
  gluNurbsProperty( TheNurbs,GLU_SAMPLING_TOLERANCE,25.0) ;
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

  /* Trouver les dimensions de la fenetre
     ------------------------------------ */


   WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
   WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
   Resize(WindowWidth,WindowHeight);
}


GLvoid
Resize( int WW, int WH )
{
  WindowWidth  = WW;
  WindowHeight = WH;
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

 GLvoid DrawBsplineCurve ( GLfloat * ,
			   BsplineCurvePtr ) ;
 GLvoid DrawAxes(GLvoid) ;
 GLvoid InitializeBsplineCurve ( GLint  ,
				 GLint  ,
				 GLint  ,
				 GLfloat * ,
				 GLfloat A[][3] ,
				 BsplineCurvePtr  ) ;
  BsplineCurve Curve ;

  InitializeBsplineCurve ( 3 ,
                           3,
                           9 ,
                           TheGivenKnots ,
                           TheGivenPoints ,
                           &Curve    ) ;

  
  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
  

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glPushMatrix();
    PolarView(Radius,Twist,Longitude,Latitude);

    DrawBsplineCurve ( Red ,
		       &Curve  ) ;


    glDisable(GL_DEPTH_TEST);

      DrawAxes() ;		
 glPopMatrix();
	  

  
/* Force a la bonne execution de l'affichage
     ----------------------------------------- */
  glutSwapBuffers(); 

}
GLvoid DrawBsplineCurve ( GLfloat * Color ,
                          BsplineCurvePtr Curve_ptr ) 
{
  GLvoid
    DrawKnotsPoints(  BsplineCurvePtr );
  GLvoid
 EvaluateBsplineCurve ( GLfloat ,
                        BsplineCurvePtr ,
                        GLfloat *  ) ;
  int i ;
  GLfloat t,dt;
  GLint N = 100;
  GLfloat Point[3] ;
  dt = 1./N;
  glColor3fv(Color);

  /* Tracer de la courbe 
     ------------------ */
  
   glColor3fv(Blue);

  glColor3fv(Yellow); 
  glBegin(GL_POINTS);
    for ( i=0;i < Curve_ptr->ControlNumber;i++)
      {
	glVertex3fv(Curve_ptr->ControlPoints[i]);
      }
  glEnd();

  glBegin(GL_LINE_STRIP);
  /*  for (i=0 ; i <= N ; i++ )
    {
      t = i*dt ;
      EvaluateBsplineCurve(t,Curve_ptr,Point);
      glVertex3fv(Point); 
    }
    glEnd();*/
}


GLvoid InitializeBsplineCurve ( GLint Dimension , 
				GLint GivenDegree ,
				GLint GivenNumber ,
				GLfloat * GivenKnots,
				GLfloat GivenPoints[][3] ,
				BsplineCurvePtr Curve_ptr  )

{
  GLint i,j;
  Curve_ptr->Dimension = Dimension ;
  Curve_ptr->Degree = GivenDegree ;
  Curve_ptr->ControlNumber = GivenNumber;
  Curve_ptr->Order = GivenDegree + 1 ;
  Curve_ptr->KnotsNumber = Curve_ptr->Order +  Curve_ptr->ControlNumber;

  for ( i = 0 ; i < Curve_ptr->KnotsNumber ; i++ )
    {
      Curve_ptr->Knots[i] = GivenKnots[i] ;
    }


  for ( i = 0 ; i <  Curve_ptr->ControlNumber ; i++ ) 
    {
      for ( j = 0 ; j < Dimension ; j++ )
	{
          Curve_ptr->ControlPoints[i][j]  = GivenPoints[i][j] ;

	}

    }
  Curve_ptr->ControlPoints[Index][0] += dx ;
  Curve_ptr->ControlPoints[Index][1] += dy ;
  Curve_ptr->ControlPoints[Index][2] += dz ;
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

     case 'i':
      Radius-=0.1;
      glutPostRedisplay();
      break; 

    case 'o':
      Radius+=0.1;
      glutPostRedisplay();
      break; 

    case 'c':
      Radius=Radius_init;
      Longitude=Longitude_init;
      Latitude=Latitude_init;
      glutPostRedisplay();
      break;
    case 's':
      dx = 0. ;
      dy = 0. ;
      dz = 0. ;
      printf ("Selectionner un point : \n") ;
      scanf("%d",&Index) ;
      printf("%d\n",Index);
      glutPostRedisplay();
      break;
    case 'x':
      dx += 0.01 ;
      glutPostRedisplay();
      break; 
    case 'y':
      dy += 0.01 ;
      glutPostRedisplay();
      break; 
    case 'z':
      dz += 0.01 ;
      glutPostRedisplay();
      break; 
    case 'X':
      dx -= 0.01 ;
      glutPostRedisplay();
      break; 
    case 'Y':
      dy -= 0.01 ;
      glutPostRedisplay();
      break; 
    case 'Z':
      dz -= 0.01 ;
      glutPostRedisplay();
      break; 
    case 27:
      exit(-1);
      break;
    }
}
GLvoid
MouseMove(int x , int y)
{
  GLsizei WindowWidth,WindowHeight;
  WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
  WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
  if ( Flag == ROTATE )
    {
      
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














