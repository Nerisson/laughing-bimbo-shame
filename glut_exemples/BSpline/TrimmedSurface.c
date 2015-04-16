#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/*  Fonctions Prototypes  */

GLvoid Initialize( char * );
GLvoid DrawScene( GLvoid );
GLvoid DrawGrid(GLvoid);
GLvoid DrawAxes(GLvoid);
GLvoid Resize( GLsizei , GLsizei );
GLvoid PolarView(GLdouble,GLdouble,GLdouble,GLdouble);
GLvoid Keyboard(unsigned char , int, int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int  , int );
enum Action { ROTATE,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

GLboolean ExchangeFlag = GL_FALSE ;  
GLboolean GridFlag = GL_TRUE ;
GLboolean ReverseFlag = GL_FALSE ;   

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
GLfloat near = 0.5;
GLfloat far = 20;
GLfloat fovy = 60.;
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

#define UCONTROLNUMBER 9
#define VCONTROLNUMBER 5
#define UKNOTSNUMBER 13
#define VKNOTSNUMBER 8
GLfloat TheGivenPoints[VCONTROLNUMBER][UCONTROLNUMBER][3] ;
GLfloat TheUGivenKnots[13]={0.,0.,0.,0.,0.25,0.25,0.5,0.75,0.75,1.,1.,1.,1.};
GLfloat TheVGivenKnots[8]={0.,0.,0.,0.25,0.75,1.,1.,1.};

typedef struct BsplineSurf {
  GLint Dimension ;
  GLint Degree[2] ;
  GLint ControlNumber[2] ;
  GLint Order[2] ;
  GLint KnotsNumber[2] ;
  GLfloat *  UKnots ;
  GLfloat *  VKnots ;
  GLfloat *  ControlPoints ;
} BsplineSurface, * BsplineSurfacePtr ;

typedef struct SurfProperties {
  GLboolean EmissionFlag ;
  GLfloat Emission[4] ;
  GLboolean SpecularFlag ;
  GLfloat Specular[4] ;
  GLboolean AmbientFlag ;
  GLfloat Ambient[4] ;
  GLboolean DiffuseFlag ;
  GLfloat Diffuse[4] ;
  GLboolean ShininessFlag ;
  GLfloat Shininess ;
  GLboolean TransparencyFlag ;
  GLfloat Transparency ;
} MaterialsProperties, *MaterialsPropertiesPtr ; 


typedef struct Surf {
  BsplineSurfacePtr  Surface_ptr ;
  MaterialsPropertiesPtr SurfaceProperties_ptr;
} ParametricSurface, * ParametricSurfacePtr;

 
GLUnurbsObj * TheNurbs ;
GLfloat dx = 0. ;
GLfloat dy = 0. ;
GLfloat dz = 0. ;
GLint Index=0 ;

ParametricSurfacePtr ParamSurface_ptr ;

/* Programme principal
   ------------------- */

int
main(int argc ,  char ** argv)
{
  GLvoid
  FreeParametricSurface(ParametricSurfacePtr * ) ;

  GLvoid
  InitializeParametricSurface(ParametricSurfacePtr * ) ;

  
  glutInit(&argc,argv);
  Initialize(argv[0]);
  InitializeParametricSurface(&ParamSurface_ptr);
  glutKeyboardFunc(Keyboard);
  glutMouseFunc(Mouse);
  glutMotionFunc( MouseMove );
  glutDisplayFunc(DrawScene);
  glutReshapeFunc(Resize);
  glutMainLoop();

  FreeParametricSurface( &ParamSurface_ptr ) ;
  return 0;
}

GLvoid
  FreeParametricSurface(ParametricSurfacePtr * ParamSurface_pptr )
{
  BsplineSurfacePtr Surface_ptr ;
  GLvoid FreeBsplineSurface ( BsplineSurfacePtr * ) ;
  Surface_ptr = (*ParamSurface_pptr)-> Surface_ptr ;
  FreeBsplineSurface( &Surface_ptr ) ;
  free((*ParamSurface_pptr)-> SurfaceProperties_ptr) ;
  free(*ParamSurface_pptr);
  *ParamSurface_pptr = NULL ;
}

GLvoid
FreeBsplineSurface ( BsplineSurfacePtr * Surface_pptr  )
{ 
   
   BsplineSurfacePtr Surface_ptr ;
   Surface_ptr = *Surface_pptr ;

   free( Surface_ptr->UKnots);
   free( Surface_ptr->VKnots);
   free( Surface_ptr->ControlNumber);
   free (Surface_ptr) ;
   *Surface_pptr = NULL ;
}
  
GLvoid
  InitializeParametricSurface(ParametricSurfacePtr * ParamSurface_pptr )
{
  GLvoid InitialiseControlPoints(GLvoid) ;

  GLvoid InitializeBsplineSurface ( GLint  ,
				    GLint Deg[2] ,
				    GLint Num[2] ,
				    GLfloat * ,
				    GLfloat * ,
				    GLfloat A[VCONTROLNUMBER][UCONTROLNUMBER][3] ,
				    BsplineSurfacePtr *  ) ;

  GLint GivenDegree[2] ;
  GLint GivenNumber[2] ;

  MaterialsPropertiesPtr Properties_ptr ;
  BsplineSurfacePtr Surface_ptr ;
    
  *ParamSurface_pptr = (ParametricSurfacePtr) malloc( sizeof(ParametricSurface)) ;

  /* Proprietes de la surface
     ------------------------ */

  Properties_ptr = (MaterialsPropertiesPtr)malloc( sizeof(MaterialsProperties));

  Properties_ptr->EmissionFlag = GL_FALSE ;
  Properties_ptr->TransparencyFlag = GL_TRUE ;
 
  Properties_ptr->AmbientFlag = GL_TRUE ;
  Properties_ptr->Ambient[0] = 0. ;
  Properties_ptr->Ambient[1] = 0.157811 ;
  Properties_ptr->Ambient[2] = 0.187004 ;
  Properties_ptr->Ambient[3] = 0.8 ;


  Properties_ptr->DiffuseFlag = GL_TRUE ;
  Properties_ptr->Diffuse[0] = 0. ;
  Properties_ptr->Diffuse[1] = 0.631244 ;
  Properties_ptr->Diffuse[2] = 0.748016 ;
  Properties_ptr->Diffuse[3] = 0.8 ;

  Properties_ptr->SpecularFlag = GL_TRUE ;
  Properties_ptr->Specular[0] = 0.915152 ;
  Properties_ptr->Specular[1] = 0.915152 ;
  Properties_ptr->Specular[2] = 0.915152 ;
  Properties_ptr->Specular[3] = 0.8 ;

  Properties_ptr->ShininessFlag = GL_TRUE ;
  Properties_ptr->Shininess = 64.2424 ;

  Properties_ptr->TransparencyFlag = GL_TRUE ;
  Properties_ptr->Transparency = 0.8 ;

  (*ParamSurface_pptr)->SurfaceProperties_ptr = Properties_ptr ;
 
  GivenDegree[0] = UKNOTSNUMBER - UCONTROLNUMBER -1 ;
  GivenDegree[1] = VKNOTSNUMBER - VCONTROLNUMBER -1 ;
  GivenNumber[0] = UCONTROLNUMBER ;
  GivenNumber[1] = VCONTROLNUMBER ;

  InitialiseControlPoints() ;


  
  InitializeBsplineSurface ( 3 ,
			     GivenDegree,
			     GivenNumber ,
			     TheUGivenKnots ,
			     TheVGivenKnots ,
			     TheGivenPoints ,
			     &Surface_ptr    ) ;

  
 (*ParamSurface_pptr)->Surface_ptr = Surface_ptr ;
}

GLvoid InitializeBsplineSurface ( GLint Dimension , 
				  GLint GivenDegree[2] ,
				  GLint GivenNumber[2] ,
				  GLfloat * UGivenKnots,
				  GLfloat * VGivenKnots,
				  GLfloat GivenPoints[VCONTROLNUMBER]
                                                     [UCONTROLNUMBER][3] ,
				  BsplineSurfacePtr * Surface_pptr  )

{
  GLint i,j,k;
  GLint ia ;
  BsplineSurfacePtr Surface_ptr ;
  *Surface_pptr = (BsplineSurfacePtr) malloc ( sizeof( BsplineSurface));
   Surface_ptr =  *Surface_pptr ;

  Surface_ptr->Dimension = Dimension ;

  for (i = 0 ; i < 2 ; i++ )
    {
      Surface_ptr->Degree[i] = GivenDegree[i] ;
      Surface_ptr->ControlNumber[i] = GivenNumber[i];
      Surface_ptr->Order[i] = GivenDegree[i] + 1 ;
      Surface_ptr->KnotsNumber[i] = GivenNumber[i] + GivenDegree[i] + 1;
     }

  Surface_ptr->VKnots = ( GLfloat * )  malloc ( Surface_ptr->KnotsNumber[1]* sizeof(GLfloat)) ;

  for ( i = 0 ; i < Surface_ptr->KnotsNumber[1] ; i++ )
    {
      Surface_ptr->VKnots[i] = VGivenKnots[i] ;
    }

  Surface_ptr->UKnots = ( GLfloat * )  malloc ( Surface_ptr->KnotsNumber[0]* sizeof(GLfloat)) ;

  for ( i = 0 ; i < Surface_ptr->KnotsNumber[0] ; i++ )
    {
      Surface_ptr->UKnots[i] = UGivenKnots[i] ;
    }

  Surface_ptr->ControlPoints = ( GLfloat * ) malloc (3*Surface_ptr->ControlNumber[0]*
                                                     Surface_ptr->ControlNumber[1]*sizeof(GLfloat)); 
  ia = 0 ; 
  for ( i = 0 ; i <  Surface_ptr->ControlNumber[1] ; i++ )
    {
      
      for ( j = 0 ; j <  Surface_ptr->ControlNumber[0] ; j++ )
	{
	  for ( k = 0 ; k < 3 ; k++ ) 
	    {
	      Surface_ptr->ControlPoints[ia] = GivenPoints[i][j][k];
              ia +=1;
	    }
	}
    }
}

GLvoid  InitialiseControlPoints(GLvoid) 
{
  GLint i,j;

  dz = -2. ;
  for ( i = 0; i < VCONTROLNUMBER ; i++ )
    {
      dx = -2. ;
      dy = 0. ;
      for (j = 0 ; j < UCONTROLNUMBER ; j++ )
	{
         TheGivenPoints[i][j][0] += dx ;
         TheGivenPoints[i][j][1] += dy ;
	 TheGivenPoints[i][j][2] += dz ;
         dx += 0.5 ;

         if ( dx > 0. ) dy -= 0.25 ;
         else
	   dy += 0.25 ;
	}
      dz +=1. ;  
    }
}


GLvoid
Initialize( char * Titre )
{

  GLvoid DefineWindow( char *) ;
  GLfloat Light0Ambient[]={0.,0.,1.,1.};
  GLfloat Light0Diffuse[]={1.,0.,0.,1.} ;
  GLfloat Light0Specular[]={1.,1.,1.,1.} ;

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

  /* Definir la lumiere
     ------------------ */

  glEnable(GL_LIGHT0);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
  glDepthFunc(GL_LESS);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
  glLightfv(GL_LIGHT0,GL_AMBIENT,Light0Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,Light0Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Light0Specular);


  /* Definition de la surface nurbs
     ------------------------------ */

  TheNurbs = gluNewNurbsRenderer() ;
  gluNurbsProperty( TheNurbs,GLU_SAMPLING_TOLERANCE,50.0) ;
  gluNurbsProperty( TheNurbs,GLU_DISPLAY_MODE,GLU_FILL);
}

GLvoid DefineWindow( char * Titre)
{
  
  GLsizei ScreenWidth,ScreenHeight;

  /* Largeur et hauteur de l'écran
     -------------------------------- */

    
/* Largeur et hauteur de l'écran
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
  GLfloat   Light0Position[]={2.,2.,2.,1.};

  GLvoid LocateLight(GLfloat *, GLint ) ;

  GLvoid DrawBsplineSurface ( GLfloat * ,
			      ParametricSurfacePtr  ) ;
  GLvoid DrawGrid(GLvoid);
  
  /* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
  

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);


  glPushMatrix();
    glTranslated( 0.0, 0.0,-Radius);
    LocateLight(Light0Position,GL_LIGHT0);       
  glPopMatrix();

  glPushMatrix();
    PolarView(Radius,Twist,Longitude,Latitude);
    if (GridFlag == GL_TRUE )
      {
	glPushMatrix();
	glDisable(GL_LIGHTING) ;
	glRotatef(90.,1.,0.,0.);
	DrawGrid();
	if ( ExchangeFlag == GL_TRUE )  glEnable(GL_LIGHTING);
	glPopMatrix();
      }
 
    DrawBsplineSurface ( Red ,
			 ParamSurface_ptr  ) ;

	
    glDisable(GL_DEPTH_TEST);
    DrawAxes() ;		

 glPopMatrix();

  glutSwapBuffers();
  
/* Force a la bonne execution de l'affichage
     ----------------------------------------- */


}

GLvoid 
LocateLight(GLfloat *Location,GLint LightNumber)
{
  GLvoid DrawLight(GLvoid);
  glLightfv(LightNumber,GL_POSITION,Location);
  glPushMatrix();
  glTranslatef(Location[0],Location[1],Location[2]);
  DrawLight();
  glPopMatrix();
  
  
}

GLvoid 
DrawLight(GLvoid)
{
  glDisable(GL_LIGHTING);
  glColor3fv(Yellow);
  glutSolidCone(0.1,0.2,10,10);
  if ( ExchangeFlag == GL_TRUE )  glEnable(GL_LIGHTING);
  
}
GLvoid DrawBsplineSurface ( GLfloat * Color ,
			    ParametricSurfacePtr ParamSurface_ptr ) 
{
  GLvoid
   SetSurfaceProperties( MaterialsPropertiesPtr);

GLvoid
   DrawCurvesContour(GLvoid);

  int i,j, ia, k ;
  GLfloat Point[3] ;
  BsplineSurfacePtr Surface_ptr ;
  
  if ( ExchangeFlag == GL_FALSE ) 
    glColor3fv(Color);
  else
    {
      glPushAttrib( GL_LIGHTING_BIT );
      SetSurfaceProperties( ParamSurface_ptr-> SurfaceProperties_ptr ) ;
    }  

   Surface_ptr = ParamSurface_ptr-> Surface_ptr ; 
  gluBeginSurface(TheNurbs) ;
 
    gluNurbsSurface(TheNurbs,
		    Surface_ptr->KnotsNumber[0],
		    Surface_ptr->UKnots,
		    Surface_ptr->KnotsNumber[1],
		    Surface_ptr->VKnots,
		    3,
		    3 * Surface_ptr->ControlNumber[0],

		    Surface_ptr->ControlPoints,
		    Surface_ptr->Order[0] ,
		    Surface_ptr->Order[1] ,
		    GL_MAP2_VERTEX_3 );
   DrawCurvesContour() ; 
  gluEndSurface(TheNurbs) ; 
  if ( ExchangeFlag == GL_TRUE )
    glPopAttrib();

  glColor3fv(Blue);
  glBegin(GL_POINTS);
  ia = 0 ;
  for ( i=0;i < Surface_ptr->ControlNumber[1];i++)
    {
      for ( j=0;j < Surface_ptr->ControlNumber[0];j++)
	{
          for ( k = 0 ; k < 3 ; k++ )
	    {
                Point[k] =  Surface_ptr->ControlPoints[ia];
		ia++;
	    }
	  glVertex3fv(Point);
	}
    }
  glEnd();
  
}
 
GLvoid
DrawCurvesContour (GLvoid)
{
  GLvoid ReverseArray ( GLint , GLfloat  Array[][2]) ;
  
}
 
GLvoid ReverseArray ( GLint N , GLfloat  Array[][2])
{
  GLfloat P ;
  GLint i,j,k,ia;
  
}


GLvoid
   SetSurfaceProperties( MaterialsPropertiesPtr Properties_ptr)
{
  
    if ( Properties_ptr->EmissionFlag == GL_TRUE ) 
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, Properties_ptr->Emission );

    if ( Properties_ptr->DiffuseFlag == GL_TRUE ) 
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, Properties_ptr->Diffuse );

    if ( Properties_ptr->SpecularFlag == GL_TRUE ) 
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, Properties_ptr->Specular );

    if ( Properties_ptr->EmissionFlag == GL_TRUE ) 
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, Properties_ptr->Ambient );

    if ( Properties_ptr->ShininessFlag == GL_TRUE ) 
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS,Properties_ptr->Shininess);

    if ( Properties_ptr-> TransparencyFlag == GL_TRUE )
      {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
      }

}


GLvoid DrawGrid(GLvoid)
{
  GLboolean useblack=GL_TRUE;

  GLfloat x,y;

  for (y=-2.;y<2.;y=y+0.1)
    {
      for (x=-2.;x<2.;x=x+0.1)
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
    glutSolidCone( 0.05, 0.1 , 10 ,10 );
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
    glutSolidCone( 0.05, 0.1, 10 ,10 );
  glPopMatrix();

  glBegin( GL_LINES );                       
    glVertex3f( 0.0, 0.0, 0.0 );
    glVertex3f( 0.0, 1.0, 0.0 );
  glEnd();

  /* Axes  Z rouge     */
  glColor3fv(Red);

  glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glutSolidCone( 0.05, 0.1,10,10 );
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
  glRotated(-Longitude,0.,0.,1.);


}

GLvoid
Keyboard(unsigned char key , int x, int y)
{
  BsplineSurfacePtr  Surface_ptr ;
  Surface_ptr=ParamSurface_ptr-> Surface_ptr;
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
    case 'g':
      GridFlag =!GridFlag ;
      glutPostRedisplay();
      break;
    case 'f':
      ExchangeFlag =!ExchangeFlag ;
      glutPostRedisplay();
      break;
    case 't':
      ReverseFlag =!ReverseFlag ;
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
    {
      Radius-=0.05;
      if ( Radius < 0.5) Radius =0.5 ;
    }
  else
   Radius+=0.05;
  glutPostRedisplay(); 
 }

GLvoid
Mouse(int button , int state , int x , int y)
{
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
      
    

GLvoid SelectPoint (GLvoid )
{
  GLint ControlNumber;
  BsplineSurfacePtr  Surface_ptr;
  Surface_ptr=ParamSurface_ptr-> Surface_ptr;
  ControlNumber= Surface_ptr->ControlNumber[0]*
                 Surface_ptr->ControlNumber[1] - 1;

SELECT:
  printf ("Selectionner un point : \n") ;
  scanf("%d",&Index) ;
  printf("%d\n",Index);
  if ( Index < 0 || Index > ControlNumber )
    {
      printf("Donner un indice inferieur a %d\n",ControlNumber);
      goto SELECT ;
    }
  Index = 3 * Index ;
}














