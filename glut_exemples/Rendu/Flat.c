
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*  Fonctions Prototypes  */

GLvoid  Initialize( char * );
GLvoid  DrawScene( GLvoid );
GLvoid DefineWindow( char * Titre);

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
  return 0 ;
}

GLvoid Initialize( char * Titre ){

	GLvoid DefineWindow( char*);
	glutInitDisplayMode(GLUT_RGB);

/* Definition de la fenetre
____________________________*/

	DefineWindow(Titre);

/* Définir l ’espace visible 
    ------------------------------- */

	glPointSize(4);
	glLineWidth(2.5);
	gluOrtho2D(-10.,10.,-10.,10.);

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

	glClearColor(0.,0.,0.,1.);
}

GLvoid DrawScene( GLvoid ){

	int i,j;
	/*GLvoid DrawGrid(GLvoid);*/

/* Effacement de la fenetre pour un nouvel affichage 
    ---------------------------------------------------------------- */

	glClear( GL_COLOR_BUFFER_BIT);	/* à garder toujours*/
	/*DrawGrid();*/

/* 2 Points
	glColor3fv(Yellow);
	glBegin(GL_POINTS);
	glVertex2f(0.3,0.85);
	glVertex2f(0.3,0.90);
	glEnd();
*/
/* 3 Lignes qui se rejoignent
	glColor3fv(Red);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.1,0.1);
	glVertex2f(0.2,0.9);
	glVertex2f(0.5,0.5);
	glEnd();
*/
/* 1 Polygone
	glColor3fv(Magenta);
	glBegin(GL_POLYGON);
	glVertex2f(0.5,0.1);
	glVertex2f(1.,0.4);
	glVertex2f(0.9,1.);
	glVertex2f(0.3,0.8);
	glVertex2f(0.1,0.5);
	glEnd();
*/
/* Plusieurs Triangles collés
	glColor3fv(Yellow);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.4,0.4);
	glVertex2f(0.3,0.4);
	glVertex2f(0.4,0.5);
	glVertex2f(0.5,0.5);
	glVertex2f(0.5,0.4);
	glVertex2f(0.4,0.3);
	glEnd();
*/

	glFlush();	/* à garder toujours */
}

/*GLvoid DrawGrid( GLvoid ){

	GLboolean useblack=GL_TRUE;
	GLfloat i,j;

	for(j=-10.;j<10.;j+=0.5)
		{
		for (i=-10.;i<10.;i+=0.5)
			{
			if(useblack==GL_TRUE)  glColor3fv(Black);
			else glColor3fv(White);
			glRectf(i,j,i+0.5,j+0.5);
			useblack=!useblack;
			}
		useblack=!useblack;
		}
}
*/



