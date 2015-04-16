
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*  Fonctions Prototypes  */

GLvoid  Initialize( char * );
GLvoid  DrawScene();


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

/* Programme principal
   ------------------- */

int main(int argc ,  char ** argv){

 /* code executable :
   ---------------   */
  glutInit(&argc,argv);
  Initialize(argv[0]);
  glutDisplayFunc(DrawScene);
  glutMainLoop();
  return 0;
}

GLvoid Initialize( char * Titre ){

GLvoid DefineWindow( char *) ;

  glutInitDisplayMode( GLUT_RGBA );

/* Definition de la fenetre
     ------------------------ */

  DefineWindow( Titre) ;

  /* Definir le monde des coordonnees
     ------------------------------- */

  glPointSize(4);
  glLineWidth(2.5);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-4.,4.,-4.,4.,-1.,1.);
  glMatrixMode(GL_MODELVIEW);

}

GLvoid DefineWindow( char * Titre){

  GLsizei width,height;

/* Largeur et hauteur de l'ecran
   ---------------------------------- */

  width = glutGet(GLUT_SCREEN_WIDTH);
  height = glutGet(GLUT_SCREEN_HEIGHT);


/* Definition de la fenetre
     ------------------------ */

  glutInitWindowPosition(width/4,height/4);
  glutInitWindowSize(width/2,height/2);

  glutCreateWindow(Titre);

/* Definition de la couleur du fond de la fenetre
   ----------------------------------------------- */

  glClearColor(0.,0.,1.,1.);
}

GLvoid DrawScene(){

	GLvoid DrawGrid() ;
	GLvoid DrawAxes();

/* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */

	glClear( GL_COLOR_BUFFER_BIT );
	DrawGrid();
	DrawAxes();

/* Premiere Transformation: Translation
     ------------------------------------------------- */

	glPushMatrix();
	glTranslatef(3.,0.,0.);
	DrawGrid();
	DrawAxes();


/* Deuxieme Transformation: Translation + Rotation
     ------------------------------------------------- */

	glTranslatef(3.,0.,0.);
	glRotatef(-6.,0.,0.,1.);
	DrawGrid();
	DrawAxes();


/* Troisieme Transformation: Translation + Homothetie
     ------------------------------------------------- */

	glTranslatef(2.,-2.,0.);
	glScalef(0.5,0.5,1.);
	DrawGrid();
	DrawAxes();

/* Force a la bonne execution de l'affichage
     ----------------------------------------- */

	glPopMatrix();
	glFlush();
}

GLvoid DrawGrid(){
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

GLvoid DrawAxes(){

	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_LINES );
	glVertex2f( 0.0, 0.0 );
	glVertex2f( 1., 0.0 );
	glVertex2f( 0.0, 0.0 );
	glVertex2f( 0.0, 1. );
	glEnd();

	glBegin( GL_POLYGON );
	glVertex2f( 1., 0.0 );
	glVertex2f( 0.9, -0.1 );
	glVertex2f( 0.9, 0.1 );
	glEnd();

	glBegin( GL_POLYGON );
	glVertex2f( 0.0, 1. );
	glVertex2f( -0.1, 0.9 );
	glVertex2f( 0.1, 0.9 );
	glEnd();
}








