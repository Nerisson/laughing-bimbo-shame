
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

/*  Function Prototypes  */

GLvoid  Initialize( char * );
GLvoid  DrawScene( GLvoid );
GLvoid  Resize(GLsizei , GLsizei);

/* Variables globales 
   ------------------- */
GLfloat Left,Right,Bottom,Top;

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

int
main(int argc ,  char ** argv)
{
/* code executable :
   ---------------   */
  glutInit(&argc,argv);
  Initialize(argv[0]);
  /* Reshape */

 
  glutDisplayFunc(DrawScene);

  glutMainLoop();

  return 0;
}


GLvoid
Initialize( char * Titre )
{
  GLvoid DefineWindow( char *) ;

  glutInitDisplayMode( GLUT_RGBA );

/* Définition de la fenêtre 
     ------------------------ */

  DefineWindow( Titre) ;  

  /* Définir le monde des coordonnees
     ------------------------------- */

  glPointSize(4);
  glLineWidth(2.5);


}

GLvoid DefineWindow( char * Titre)
{
  GLsizei ScreenWidth,ScreenHeight;

  
/* Largeur et hauteur de l'ecran
   ---------------------------------- */

  ScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
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
DrawScene( GLvoid )
{

   GLvoid DrawGrid( GLvoid ) ;

  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */

  glClear( GL_COLOR_BUFFER_BIT );

  DrawGrid();

/* Force a la bonne execution de l'affichage
     ----------------------------------------- */

  glFlush();
}

GLvoid DrawGrid(GLvoid)
{
  GLboolean useblack=GL_TRUE;

  GLfloat x,y;

  for (y=-1.;y<=1.;y=y+0.1)
    {
      for (x=-1.;x<=1.;x=x+0.1)
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


GLvoid Resize(GLsizei WindowWidth, GLsizei WindowHeight)
{
  
}
















