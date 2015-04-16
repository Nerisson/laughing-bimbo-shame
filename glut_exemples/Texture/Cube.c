/**************/
/* Texture.c */
/**************/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <tiffio.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265


/*  Variables globales definissant les dimensions
    de la pyramide
    --------------------------------------------- */
 
GLfloat aspect;
GLfloat near = 0.1;
GLfloat far = 16.;
GLfloat fovy = 45. ;
int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
GLfloat MaxObjectsSize;
GLfloat xtrans,ytrans,ztrans,xangle,yangle,zangle;
enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

uint IdTex[2]; /* tableau d'Id pour les 2 textures */
float Decalage=0; /* décalage de la texture procedurale pour l'animation */
GLint TextureId;

/* déclaration des indicateurs booleens */

GLboolean LightFlag = GL_TRUE;
GLboolean SeparateFlag = GL_TRUE;

GLfloat Distance=2.5;
GLfloat  Angley=20.;
GLfloat  Anglex=30.;

/* prototypes des fonctions */

GLvoid Inactif();
GLvoid ChargeTextureTiff(char *fichier,GLint numtex);
GLvoid ChargeTextureProc(int numtex);
GLint FonctionTexture(GLint x,GLint y);

GLvoid Initialise( char * );
GLvoid DrawScene( GLvoid );
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int, int);
GLvoid Resize( GLsizei , GLsizei );
GLvoid PilotView(GLvoid);

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


  /* Chargement des textures
     ----------------------- */

    Initialise(argv[0]);


  
  /* Mise en place des fonctions de rappel glut
     ------------------------------------------ */
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc( MouseMove );
    glutDisplayFunc(DrawScene);
    glutReshapeFunc(Resize);

  /* Boucle principale */
  glutMainLoop();
  return 0;
}
GLvoid
Initialise( char * Titre )
{
  GLvoid DefineWindow( char *) ;
  GLvoid DefineLight(GLvoid);
  

  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

  /* Initialisation
     -------------- */

  MaxObjectsSize=far-near;
  xtrans = 0.;
  ytrans = 0.;
  ztrans = MaxObjectsSize/2. + near;
  xangle = 0.;
  yangle = 0.;
  zangle = 0;
  
  DefineWindow( Titre) ;  
  
 /* Chargement des textures
     ----------------------- */

    glGenTextures(2,IdTex);
    ChargeTextureTiff("cube.tif",IdTex[0]);
    TextureId = IdTex[0] ;
    ChargeTextureProc(IdTex[1]); 
 

/* Definition des source lumineuse 
   ------------------------------- */

//  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);

    DefineLight();
}

GLvoid DefineLight(GLvoid)
{
  GLfloat Light0pos[]={ 0.0,2.0,-1.0};
  GLfloat Light0dif[]={ 1.0,0.6,0.6};
  GLfloat Light1pos[]={ 2.0,2.0,2.0};
  GLfloat Light1dif[]={ 0.0,0.5,1.0};
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,Light0dif);
  glLightfv(GL_LIGHT0,GL_POSITION,Light0pos);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,Light1dif);
  glLightfv(GL_LIGHT1,GL_POSITION,Light1pos); 
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

  glClearColor(0.8,0.8,0.8,1.);
  glShadeModel(GL_SMOOTH);

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


GLvoid DrawScene()
{
   GLvoid DrawGrid(GLvoid);
   GLvoid DrawCube(GLint) ;   
   GLfloat CubeSpecular[]={0.5,0.5,0.5};
   GLfloat CubeShiny=50;

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Positionnement de l'observateur (ou de l'objet) 
     ------------------------------------------------*/

  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,CubeSpecular);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,CubeShiny);

 
  glPushMatrix();
  gluLookAt(0.0,0.0,Distance,0.0,-ztrans,0.0,0.0,1.0,0.0);
 glPopMatrix();
 glPushMatrix();
  PilotView();
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_LIGHTING);

  glPushMatrix();
    glTranslatef(0.,0.,-0.501);
    DrawGrid();
  glPopMatrix();
  glColor3f(1.0,1.0,1.0);
if (LightFlag)
    glEnable(GL_LIGHTING);
 else
    glDisable(GL_LIGHTING);


  /* Affichage du Cube
     ----------------- */

  DrawCube(TextureId) ; 
 
  /*  echange de tampon (double buffering)*/
  glPopMatrix();
  glutSwapBuffers();
}

GLvoid DrawCube(GLint TextureId)
{
  
  glBegin(GL_POLYGON);
  glNormal3f(0.0,0.0,1.0);
   glVertex3f(-0.5, 0.5, 0.5);
   glVertex3f(-0.5,-0.5, 0.5);
   glVertex3f( 0.5,-0.5, 0.5);
   glVertex3f( 0.5, 0.5, 0.5);
  glEnd();

  
  glBegin(GL_POLYGON); 
  glNormal3f(1.0,0.0,0.0);
   glVertex3f( 0.5, 0.5, 0.5);
   glVertex3f( 0.5,-0.5, 0.5);
   glVertex3f( 0.5,-0.5,-0.5);
   glVertex3f( 0.5, 0.5,-0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0.0,0.0,-1.0);
   glVertex3f( 0.5, 0.5,-0.5);
   glVertex3f( 0.5,-0.5,-0.5);
   glVertex3f(-0.5,-0.5,-0.5);
   glVertex3f(-0.5, 0.5,-0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(-1.0,0.0,0.0);
   glVertex3f(-0.5, 0.5,-0.5);
   glVertex3f(-0.5,-0.5,-0.5);
   glVertex3f(-0.5,-0.5, 0.5);
   glVertex3f(-0.5, 0.5, 0.5);
  glEnd();

  glBegin(GL_POLYGON);
  glNormal3f(0.0,1.0,0.0);
   glVertex3f(-0.5, 0.5,-0.5);
   glVertex3f(-0.5, 0.5, 0.5);
   glVertex3f( 0.5, 0.5, 0.5);
   glVertex3f( 0.5, 0.5,-0.5);
  glEnd();


  glBegin(GL_POLYGON);
  glNormal3f(0.0,-1.0,0.0);
   glVertex3f(-0.5,-0.5,-0.5);
   glVertex3f(-0.5,-0.5, 0.5);
   glVertex3f( 0.5,-0.5, 0.5);
   glVertex3f( 0.5,-0.5,-0.5);
  glEnd();

  
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


/**************************************************/
/* void clavier(unsigned char touche,int x,int y) */
/**************************************************/
/* fonction de rappel clavier                     */
/**************************************************/

GLvoid
Keyboard(unsigned char key , int x, int y)
{
  switch(key)
    {
    case 'f':
      LightFlag=!LightFlag;
      glutPostRedisplay();
      break;
    case 's':
      SeparateFlag=!SeparateFlag;
      glutPostRedisplay();
      break;

    case 'i':
      if ( TextureId == IdTex[0] ) TextureId = IdTex[1] ;
      else TextureId = IdTex[0] ;
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


/****************************************************/
/* void ChargeTextureTiff(char *fichier,int numtex) */
/****************************************************/
/* Chargement de l'image tif 'fichier' et placement */
/* dans la texture de numero 'numtex'               */
/****************************************************/

void ChargeTextureTiff(char *fichier,int numtex)
{
  unsigned char image[256][256][3];
  uint32 l, h;
  int i,j;
  size_t npixels;
  uint32* raster;
  
  /* chargement de l'image TIF */
  TIFF* tif = TIFFOpen(fichier, "r");
  if (tif) {
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &l);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h); 
    npixels = l * h;
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32)); 
    if (raster != NULL) {
      /* lecture de l'image */
      if (TIFFReadRGBAImage(tif, l, h, raster, 1)) {
	/* transfert de l'image vers le tableau 'image' */
	for (i=0;i<256;i++)
	  for (j=0;j<256;j++) {
	    image[i][j][0]=((unsigned char *)raster)[i*256*4+j*4+0];
	    image[i][j][1]=((unsigned char *)raster)[i*256*4+j*4+1];
	    image[i][j][2]=((unsigned char *)raster)[i*256*4+j*4+2];
	  }
      }
      else {
	printf("erreur de chargement du fichier %s\n",fichier);
	exit(0);
      }
      _TIFFfree(raster); 
    }
    TIFFClose(tif);
      
    /*      paramétrage et spécification de la texture
            ------------------------------------------ */

    
  }
  
}

/**************************************/
/* void ChargeTextureProc(int numtex) */
/**************************************/
/* Création de la texture procedurale */
/* de numero 'numtex'                 */
/**************************************/

void ChargeTextureProc(int numtex)
{
  unsigned char image[256][256][3];
  int i,j;
  int a;
  
  /* calcule de l'image */
  for (i=0;i<256;i++)
    for (j=0;j<256;j++) {
      a=FonctionTexture(i,j);
      image[i][j][0]=a;
      image[i][j][1]=128;
      image[i][j][2]=128;
    }
  
  /* Paramètrage de la texture
     ------------------------- */
  
}



/*****************************************************/
/* int fonctionTexture(int x,int y)                  */
/*****************************************************/
/* Calcule et renvoie la valeur de la fonction       */
/* utilisee pour la texture procedurale au point x,y */
/*****************************************************/

int FonctionTexture(int x,int y)
{
  float dx=(128.0-(float)x)/255.0*40.0;
  float dy=(128.0-(float)y)/255.0*40.0;
  float a=cos(sqrt(dx*dx+dy*dy)+0.01);
  return (int)((a+1.0)/2.0*255);
    
}
GLvoid
MouseMove(int x , int y)
{
  if ( Flag == ROTATE )
    {
      GLsizei WindowWidth,WindowHeight;
      WindowWidth  = glutGet(GLUT_WINDOW_WIDTH) ;
       WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
      xangle+=(xoffset* 360.) / WindowWidth;
      yangle+=(yoffset*360.) / WindowHeight;
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
GLvoid  
PilotView(GLvoid)
{
  glTranslatef(-xtrans,-ytrans,-ztrans);
  glRotatef(zangle,0.,0.,1.);
  glRotatef(xangle,1.,0.,0.);
  glRotatef(yangle,0.,1.,0.);
}
