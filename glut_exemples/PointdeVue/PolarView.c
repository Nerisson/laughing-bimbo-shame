
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*  Fonctions Prototypes  */

GLvoid Initialize( char * );
GLvoid DrawScene( GLvoid );
GLvoid DrawGrid(GLvoid);
GLvoid DrawAxes(GLvoid);
GLvoid Resize( GLsizei , GLsizei );
GLvoid  PolarView(GLdouble,GLdouble,GLdouble,GLdouble);

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
 
GLfloat Ratio;
GLfloat near = 1.;
GLfloat far = 30.;
GLfloat fovy = 45.;

GLdouble    Radius;
GLdouble    Twist=0.;
GLdouble    Latitude=30.;
GLdouble    Longitude=30.; 


/* Programme principal
   ------------------- */
int
main(int argc ,  char ** argv)
{
 
 /* code executable :
   ---------------   */
  glutInit(&argc,argv);
  Initialize(argv[0]);
  glutDisplayFunc(DrawScene);
  glutReshapeFunc( Resize );
  glutMainLoop(); 
  return 0;
}


GLvoid
Initialize( char * Titre )
{
  GLvoid DefineWindow( char *) ;

  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH  );

/* Définition de la fenêtre 
     ------------------------ */

  DefineWindow( Titre) ;  


  /* Definir le monde des coordonnees
     ------------------------------- */

  glPointSize(4);
  glLineWidth(2.5);
 
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

  glClearColor(0.,0.,1.,1.);

  /* Trouver les dimensions de la fenetre
     ------------------------------------ */


   WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
   WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

}


GLvoid
DrawScene( GLvoid )
{
  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
   glPushMatrix();
   
    glColor3fv(Cyan);  
    glPushMatrix();
     glRotatef(30.,0.,1.,0.);
     glutSolidTorus(0.2,0.5,50,50); 
    glPopMatrix(); 

    glPushMatrix();
       glTranslatef(0.5,0.5,0.5);
       glRotatef(30.,1.,0.,0.);
      glColor3fv(Magenta);
      glutSolidCube(1.);
    glPopMatrix();    

    glPushMatrix();
      glColor3fv(Yellow);
      glTranslatef(-0.5,0.5,0.);
      glutSolidSphere(0.5,50,50);
     glPopMatrix();

    glPushMatrix();

      glScalef(3.,3.,1.);    
      DrawGrid() ;
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    DrawAxes() ;
 glPopMatrix();   
 

/* Force a la bonne execution de l'affichage
     ----------------------------------------- */
 
  glFinish();
}

GLvoid DrawGrid(GLvoid)
{
  GLboolean useblack=GL_TRUE;

  GLfloat x,y;

  for (y=-1.;y<1.;y=y+0.1)
    {
      for (x=-1.;x<1.;x=x+0.1)
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

GLvoid  
PolarView(GLdouble Radius,GLdouble Twist,GLdouble Latitude,GLdouble Longitude)
{
  glTranslated(0.,0., -Radius);
  glRotated(-Twist,0.,0.,1.);
  glRotated(-Latitude,1.,0.,0.);
  glRotated(Longitude,0.,0.,1.);
}



