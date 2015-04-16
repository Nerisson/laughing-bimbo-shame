#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <tiffio.h>
/*  Fonctions Prototypes  */

GLvoid Initialise( char * );
GLvoid DrawScene( GLvoid );
GLvoid DrawGrid(GLvoid);
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int , int );
GLvoid Resize( GLsizei , GLsizei );
GLvoid PilotView(GLvoid);
GLvoid Inactif();
GLvoid ChargeTextureTiff(char *fichier,GLint numtex);
GLvoid ChargeTextureProc(int numtex);
GLint FonctionTexture(GLint x,GLint y);

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
GLdouble    MaxObjectsSize; 
GLboolean   LightFlag=GL_TRUE;
GLboolean   TextureFlag=GL_FALSE;
int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;
GLfloat xtrans,ytrans,ztrans,xangle,yangle,zangle;

enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

/* Points de Controle de la courbe de Bezier
   ----------------------------------------- */
#define UNumber 3 
#define VNumber 4

uint IdTex[2]; /* tableau d'Id pour les 2 textures */
float Decalage=0; /* décalage de la texture procedurale pour l'animation */
GLint TextureId;

typedef struct BezierSurf {
  GLint Degree[2] ;
  GLint ControlNumber[2] ;
  GLfloat   ControlPoints[UNumber][VNumber][3] ;
} BezierSurface ;

GLvoid InitialiseBezierSurface ( GLint * ,
                                 GLfloat A[UNumber][VNumber][3] ,
				 BezierSurface * ) ;

GLfloat GivenPoints[3][4][3] = {
{ {-2.0,0.0,1.0},{-1.0,1.0,1.0},{1.0,1.0,1.0} ,{2.0,0.0,1.0}},
{  {-2.0,1.0,2.0},{-1.0,1.5,2.0},{1.0,1.5,2.0},{2.0,1.0,2.0}},
{  {-2.0,0.0,3.0},{-1.0,2.0,3.0},{1.0,2.0,3.0},{2.0,0.0,3.0}}  };


GLint PointsNumber = 100 ;


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
  GLvoid DefineLight(GLvoid);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  /* Definir le monde des coordonnees
     ------------------------------- */

 
  DefineWindow( Titre) ;  
  glPointSize(5.);
  glLineWidth(1.);
  MaxObjectsSize=far-near;
  xtrans = 0.;
  ytrans = 0.;
  ztrans = MaxObjectsSize/2. + near;
  xangle = 0.;
  yangle = 0.;
  zangle = 0;  

/* Chargement des textures
  ----------------------- */

    glGenTextures(2,IdTex);
    ChargeTextureTiff("cube.tif",IdTex[0]);
    TextureId = IdTex[0] ;
    ChargeTextureProc(IdTex[1]); 
 


/* Definir une source lumineuse par defaut
     --------------------------------------- */
  DefineLight();
 
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

GLvoid DefineLight(GLvoid)
{
  GLfloat LumiereAmbient[]={1.,0.8,0.9,1.};
  GLfloat LumiereDiffuse[]={1.,0.,0.,1.};
  GLfloat LumiereSpecular[]={1.,1.,1.,1.};

/* Proprietes et Definition de la lumiere
     -------------------------------------- */

  glLightfv(GL_LIGHT0,GL_AMBIENT,LumiereAmbient); 
  glLightfv(GL_LIGHT0,GL_DIFFUSE,LumiereDiffuse); 
  glLightfv(GL_LIGHT0,GL_SPECULAR,LumiereSpecular);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);
}
GLvoid 
LocateLight(GLfloat *Location,GLint LightNumber)
{
  if ( LightFlag == GL_FALSE ) return ; 
  glLightfv(LightNumber,GL_POSITION,Location);
  glPushMatrix();
   glTranslatef(Location[0],Location[1],Location[2]);
    glDisable(GL_LIGHTING);
    glColor3fv(Yellow);
    glutSolidSphere(0.05,30,30);
    glEnable(GL_LIGHTING);
  glPopMatrix();
  
  
}


GLvoid
Resize( int  WindowWidth , int   WindowHeight )
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
 GLvoid DrawBezierSurface ( GLfloat * ,
			    GLfloat [2][2] ,
			    BezierSurface * )   ;
 GLvoid LocateLight(GLfloat *, GLint ) ;
  GLint i;
  BezierSurface Surface ;
  GLfloat DomainBounds[2][2] ;
  GLint GivenDegree[2] ;
  GLfloat   Light0Position[]={-3.,-5.,-2.,1.};
 
  DomainBounds[0][0] = 0. ;
  DomainBounds[0][1] = 1. ;
  DomainBounds[1][0] = 0. ;
  DomainBounds[1][1] = 1. ;

  GivenDegree[0] = UNumber-1;
  GivenDegree[1] = VNumber-1;
 
  InitialiseBezierSurface( GivenDegree ,
			   GivenPoints,
			   &Surface ) ;



  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
 
  glPushMatrix();
  glTranslated( 0.0, 0.0,-ztrans);
    LocateLight(Light0Position,GL_LIGHT0);       
  glPopMatrix(); 
  
  glPushMatrix();
  PilotView();
    DrawBezierSurface( Magenta ,
                       DomainBounds,
		       &Surface);

    glDisable(GL_DEPTH_TEST);

    glPopMatrix();
    glPushMatrix();
     glTranslated( 0.0, 0.0,-ztrans);
     DrawAxes() ;		
    glPopMatrix(); 
   
   
/* Force a la bonne execution de l'affichage
     ----------------------------------------- */

  glutSwapBuffers(); 

}
GLvoid InitialiseBezierSurface ( GLint GivenDegree[2] ,
				 GLfloat GivenPoints[UNumber][VNumber][3] ,
				 BezierSurface * Surface_ptr  ) 
{
  int i,j,k;

  for ( i = 0 ; i < 2 ; i++ )
    {
      Surface_ptr->Degree[i] = GivenDegree[i] ;
      Surface_ptr->ControlNumber[i] = GivenDegree[i] + 1 ;
    }

  for ( i = 0 ; i <  Surface_ptr->ControlNumber[0] ; i++ )
    {
      for ( j = 0 ; j <  Surface_ptr->ControlNumber[1] ; j++ )
	{
	  for ( k = 0 ; k < 3 ; k++ ) 
	    {
	      Surface_ptr->ControlPoints[i][j][k] = GivenPoints[i][j][k];
	    }
	}
    }
}

GLvoid DrawBezierSurface ( GLfloat * Color,
                           GLfloat  DomainBounds[2][2],
			   BezierSurface * Surface_ptr )
{

  GLfloat   surfaceAmbient[] = {0.253968, 0.253968, 0., 1. };

  GLfloat   surfaceDiffuse[] = { 0.45, 1., 0.3, 1.};

  GLfloat   surfaceSpecular[] = {0.899871, 0.893105, 0.896821, 1.};

  
  GLint i,j;
  GLfloat Parameter[2];
  GLfloat TexturePoints[2][2][2] ={{{0.,0.},{0.,1.}},{{1.,0.},{1.,1.}}};

  glPushAttrib( GL_LIGHTING_BIT );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, surfaceAmbient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, surfaceDiffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, surfaceSpecular );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 100. );

    /* Appel de la fonction de mapping
       ------------------------------- */

 

/* if ( TextureFlag == GL_TRUE ) */
/*     { */
/*       glEnable(GL_TEXTURE_2D); */
/*       glBindTexture(GL_TEXTURE_2D,TextureId); */

/*       glMap2f(GL_MAP2_TEXTURE_COORD_2, */
/* 	      DomainBounds[1][0], */
/* 	      DomainBounds[1][1], */
/* 	      2, */
/* 	      2, */
/* 	      DomainBounds[0][0], */
/* 	      DomainBounds[0][1], */
/* 	      4, */
/* 	      2, */
/* 	      &TexturePoints[0][0][0] ); */

/*       glEnable(GL_MAP2_TEXTURE_COORD_2); */

/*     } */

  glColor3fv(Color);

  if ( LightFlag == GL_TRUE )
    {
      glEnable(GL_LIGHTING );
      glEnable(GL_AUTO_NORMAL);  
    }
  else
   glDisable(GL_LIGHTING );

  glPopAttrib();
  glDisable(GL_LIGHTING);
  glColor3fv(Blue);

 /*  if ( TextureFlag == GL_TRUE) */
/*     { */
/*       glDisable(GL_MAP2_TEXTURE_COORD_2); */
/*       glDisable(GL_TEXTURE_2D); */
/*     } */
    for ( i=0;i < Surface_ptr->ControlNumber[0];i++)
      {
	glBegin(GL_LINE_STRIP);
	for ( j=0;j < Surface_ptr->ControlNumber[1];j++)
	  {
	    glVertex3fv(Surface_ptr->ControlPoints[i][j]);
	  }
	glEnd();
      }
for ( i=0;i < Surface_ptr->ControlNumber[1];i++)
      {
	glBegin(GL_LINE_STRIP);
	for ( j=0;j < Surface_ptr->ControlNumber[0];j++)
	  {
	    glVertex3fv(Surface_ptr->ControlPoints[j][i]);
	  }
	glEnd();
      }

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
    /*      paramétrage de la texture */
    glBindTexture(GL_TEXTURE_2D,numtex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,256,256,0,
		 GL_RGB,GL_UNSIGNED_BYTE,image);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
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
  
  /* Paramètrage de la texture */
  glBindTexture(GL_TEXTURE_2D,numtex);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,256,256,0,
	       GL_RGB,GL_UNSIGNED_BYTE,image);
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
      TextureFlag=!TextureFlag;
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














