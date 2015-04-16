/**************/
/* Texture.c */
/**************/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include "texture.h"
#define PI 3.14159265


/*  Variables globales definissant les dimensions
    de la pyramide
    --------------------------------------------- */
 
GLfloat aspect;
GLfloat near = 0.1;
GLfloat far = 20.;
GLfloat fovy = 50. ;
int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
GLdouble    Radius=5.;
GLfloat Radius_init;
GLdouble    Twist=0.;
GLdouble    Latitude=0.,Latitude_init;
GLdouble    Longitude=0.,Longitude_init; 
enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

/* déclaration des indicateurs booleens */

GLboolean LightFlag = GL_TRUE;
GLint texture = 0;
GLfloat rot = 0;
GLfloat opacity = 1.0;
GLfloat intensity = 1.0;
GLfloat size = 1., delta = 0;
GLfloat scale = 1.;
GLfloat transx, transy, rotx, roty;
/* prototypes des fonctions */

GLvoid Initialise( char * );
GLvoid DrawScene( GLvoid );
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int, int);
GLvoid Resize( GLsizei , GLsizei );
GLvoid DefineTexture(char *);
GLvoid visible(GLint);
GLvoid PolarView(GLdouble,GLdouble,GLdouble,GLdouble);

GLfloat White[]={1.,1.,1.,1.};
GLfloat Black[]={0.,0.,0.,1.};
GLfloat Red[]={1.,0.,0.};
GLfloat Green[]={0.,1.,0.};
GLfloat Blue[]={0.,0.,1.};
GLfloat Magenta[]={1.,0.,1.};
GLfloat Cyan[]={0.,1.,1.,0.6};
GLfloat Yellow[]={1.,1.,0.,0.6};


/**********************************/
/* int main(int argc,char **argv) */
/**********************************/
/* fonction principale            */
/**********************************/

int main(int argc,char **argv)
{
  /* Initialisation de glut 
     ------------------------*/
    glutInit(&argc,argv);


  /* Initialisation
     --------------- */

    Initialise(argv[0]);

/* Chargement des textures
     ----------------------- */

    DefineTexture(argc == 1 ? "explode.rgba" : argv[1]);

  
  /* Mise en place des fonctions de rappel glut
     ------------------------------------------ */

    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc( MouseMove );
    glutDisplayFunc(DrawScene);
    glutReshapeFunc(Resize);
    glutVisibilityFunc(visible);

  /* Boucle principale */

  glutMainLoop();
  return 0;
}

GLvoid
Initialise( char * Titre )
{
  GLvoid DefineWindow( char *) ;
  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

  /* Definir le monde des coordonnees
     ------------------------------- */

  
  DefineWindow( Titre) ;  

  Radius_init=Radius;
  Latitude_init=Latitude;
  Longitude_init=Longitude;

/* Definition des source lumineuse 
   ------------------------------- */

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

}


GLvoid DefineWindow( char * Titre)
{
  
  GLsizei ScreenWidth,ScreenHeight;
  GLsizei WindowWidth,WindowHeight;

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

  glClearColor(.25f, .25f, .75f, .25f);

  /* Trouver les dimensions de la fenetre
     ------------------------------------ */


   WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
   WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
   
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
  glLoadIdentity();
}

GLvoid
DefineTexture(char *filename) {
    static unsigned *image;
    static GLint width, height, components;
    if (filename) {
	image = read_texture(filename, &width, &height, &components);
	if (image == NULL) {
	    fprintf(stderr, "Error: Can't load image file \"%s\".\n",
		    filename);
	    exit(EXIT_FAILURE);
	} else {
	    printf("%d x %d image loaded\n", width, height);
	}
	if (components != 2 && components != 4) {
	    printf("warning: texture should be an RGBA or LA image\n");
	}
    } else {
	GLint i, j;
	unsigned char *img;
	components = 4; width = height = 512;
	image = (unsigned *) malloc(width*height*sizeof(unsigned));
	img = (unsigned char *)image;
	for (j = 0; j < height; j++)
	    for (i = 0; i < width; i++) {
		int w2 = width/2, h2 = height/2;
		if (i & 32)
		    img[4*(i+j*width)+0] = 0xff;
		else
		    img[4*(i+j*width)+1] = 0xff;
		if (j&32)
		    img[4*(i+j*width)+2] = 0xff;
		if ((i-w2)*(i-w2) + (j-h2)*(j-h2) > 64*64 &&
		    (i-w2)*(i-w2) + (j-h2)*(j-h2) < 300*300) img[4*(i+j*width)+3] = 0xff;
	    }

    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, components, width,
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);
    glEnable(GL_TEXTURE_2D);
}

void
Animate(void) {
 
    glutPostRedisplay();
}

GLvoid
visible(int state)
{
    if (state == GLUT_VISIBLE)
	glutIdleFunc(Animate);
    else
	glutIdleFunc(NULL);
}

GLvoid
DrawCube(GLvoid) {
    glBegin(GL_QUADS);
	glNormal3f(0.f, 0.f, -1.f);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f(-1.0,  1.0, -1.0);

	glNormal3f(0.f, 0.f, 1.f);
	glVertex3f(-1.0, -1.0,  1.0);
	glVertex3f( 1.0, -1.0,  1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f(-1.0,  1.0,  1.0);

	glNormal3f(0.f, 1.f, 0.f);
	glVertex3f(-1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f(-1.0,  1.0,  1.0);

	glNormal3f(0.f, -1.f, 0.f);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0, -1.0,  1.0);
	glVertex3f(-1.0, -1.0,  1.0);

	glNormal3f( 1.f, 0.f, 0.f);
	glVertex3f( 1.0, -1.0, -1.0);
	glVertex3f( 1.0,  1.0, -1.0);
	glVertex3f( 1.0,  1.0,  1.0);
	glVertex3f( 1.0, -1.0,  1.0);

	glNormal3f(-1.f, 0.f, 0.f);
	glVertex3f(-1.0, -1.0, -1.0);
	glVertex3f(-1.0,  1.0, -1.0);
	glVertex3f(-1.0,  1.0,  1.0);
	glVertex3f(-1.0, -1.0,  1.0);
    glEnd();
}

GLvoid DrawScene()
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glPushMatrix();
     PolarView(Radius,Twist,Longitude,Latitude);

    /* Sol 
       --- */
  
    glColor4f(0.f,.2f,0.f,1.f);
    glBegin(GL_POLYGON);
	glVertex3f(-4.0, -1.0, -4.0);
	glVertex3f( 4.0, -1.0, -4.0);
	glVertex3f( 4.0, -1.0,  4.0);
	glVertex3f(-4.0, -1.0,  4.0);
    glEnd();

    /* Dessin du Cube
       -------------- */

    glEnable(GL_LIGHTING);


     glColor3f(.3f,.3f,.3f);
     glPushMatrix();
      glTranslatef(-1.f, -1.+.2f, -1.5f);
      glScalef(.2f,.2f, .2f);
       DrawCube();
     glPopMatrix();

    glDisable(GL_LIGHTING);

    glTranslatef(-1.f, -1.f, -1.5f);

    /* Explosion
       --------- */

    glScalef(size,size,1.);
    if (texture) glEnable(GL_TEXTURE_2D);
    glColor4f(intensity, intensity, intensity, opacity);
    //    glDepthMask(0);
    glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex2f(-1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex2f(1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex2f(1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, 1.0);
    glEnd();
    //    glDepthMask(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
  glutSwapBuffers();
}


GLvoid
DrawGrid( GLvoid )
{
  GLboolean useblack=GL_TRUE;
  GLfloat i=-1;
  GLfloat j=-1;
    for(j=-1.;j<1.;j+=0.1)
    {
      for (i=-1.;i<1.;i+=0.1)
	{
	  if(useblack==GL_TRUE) glColor4fv(Black);
	  else glColor3fv(White);
	  glRectf(i,j,i+0.1,j+0.1);
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
    case 't':
      texture=!texture;;
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
      if (Radius < 0.1 ) Radius = 0.1; 
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
