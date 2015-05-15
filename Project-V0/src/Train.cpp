#include "Train.h"
#include <math.h>

Train::Train(float xx, float yy, float zz, float v, CArc aArc, CGraphe *g){
    x = xx;
    y = yy;
    z = zz;

    previousX = 0;
    previousY = 0;
    previousZ = 0;

    vit = v;
    arc = aArc;
    cPt = 0;
    graphe = g;
}

Train::~Train()
{
    //dtor
}

void Train::draw(){
    glTranslatef(x, z, y);
    float alpha = atan(getAy());
    if(getPointPrec().X>getPointSuiv().X){
        alpha = (alpha + M_PI);
        if(alpha>2*M_PI)
            alpha-=2*M_PI;
    }
    glRotatef(alpha*360/(2*M_PI),0.0f,1.0f,0.0f);
    float scale = 0.5;
        // color for the snowman's nose
    float red = 1.0f, blue=0.5f, green=0.5f;

	glScalef(scale, scale, scale);
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(red, green, blue);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
    //glPopMatrix();

// Draw Little Snowman
    glPushMatrix();
	glScalef(scale, scale, scale);
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Little Snowman's Body
	glTranslatef(-2.f ,-2.8f, -2.f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.f, 1.f, 0.f);
	glutSolidSphere(0.30f,10,10);

// Draw Little Snwoman's Nose
	glColor3f(red, 0, 0);
	glRotatef(0.0f,.5f, 0.0f, 0.0f);
	glTranslatef(0.2f, .0f, 0.2f);
	glutSolidCone(0.04f,0.2f,10,2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

CPoint3f Train::getPointPrec(){
    if(cPt==0)
        return graphe->list_sommet.at(arc.id_sommet_ini);
    else
        return graphe->list_point_annexe.at(arc.list_point_annexe.at(cPt-1));
}

CPoint3f Train::getPointSuiv(){
    if(cPt<arc.list_point_annexe.size())
        return graphe->list_point_annexe.at(arc.list_point_annexe.at(cPt));
    else
        return graphe->list_sommet.at(arc.id_sommet_fin);

}

void Train::move(){





    GLfloat vT = vit;

    bool b = true;

    while(b){

        GLfloat d = sqrt((x-getPointSuiv().X)*(x-getPointSuiv().X)+(y-getPointSuiv().Y)*(y-getPointSuiv().Y)+(z-getPointSuiv().Z)*(z-getPointSuiv().Z));
        GLfloat d2 = sqrt((previousX-getPointSuiv().X)*(previousX-getPointSuiv().X)+(previousY-getPointSuiv().Y)*(previousY-getPointSuiv().Y)+(previousZ-getPointSuiv().Z)*(previousZ-getPointSuiv().Z));
//        cout << d-vT << " " << cPt << " " << d << " " << vT << endl;
//        cout << x << " " << y << " " << z << " " << d << " " << d2 << " " << d2-d << endl;
        if(d<vT || d2<d){
            vT = vT - d;
//            cout << cPt << " " << arc.list_point_annexe.size() << endl;
            if(cPt==arc.list_point_annexe.size()){
                CSommet s = graphe->list_sommet.at(arc.id_sommet_fin);
//                graphe->afficheListArcSortantDuSommet(s);
                CArc ar = graphe->getListArcSortantDuSommet(s).at(rand() % graphe->getListArcSortantDuSommet(s).size());
//                cout << graphe->getListArcSortantDuSommet(s).size() << endl;
                arc = ar;
                cPt = 0;
                x = s.X;
                y = s.Y;
                z = s.Z;
                previousX = x;
                previousY = y;
                previousZ = z;
//                z = 0;
            } else {
                cPt += 1;
                x = getPointPrec().X;
                y = getPointPrec().Y;
                z = getPointPrec().Z;
                previousX = x;
                previousY = y;
                previousZ = z;
//                z = 0.0f;
            }
        } else
            b = false;
    }
    GLfloat ay = getAy();
    GLfloat az = (getPointPrec().Z-getPointSuiv().Z)/(getPointPrec().X-getPointSuiv().X);

    GLfloat dx = fabs(vT/sqrt(1+ay*ay+az*az));
    GLfloat dy = fabs(ay*dx);
    GLfloat dz = fabs(az*dx);

    previousX = x;
    previousY = y;
    previousZ = z;

//    GLfloat nor= sqrt(dx*dx + dy*dy + dz*dz);
//
//    cout<<nor << endl;

    if(getPointPrec().X>getPointSuiv().X)
        x -= dx;
    else
        x += dx;

    if(getPointPrec().Y>getPointSuiv().Y)
        y -= dy;
    else
        y += dy;

    if(getPointPrec().Z>getPointSuiv().Z)
        z -= dz;
    else
        z += dz;


}

float Train::getAy(){
    return (getPointPrec().Y-getPointSuiv().Y)/(getPointPrec().X-getPointSuiv().X);
}
