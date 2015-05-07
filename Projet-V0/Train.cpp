#include "Train.h"

Train::Train(float xx, float yy, float zz, float v, CArc aArc, CGraphe *g){
    x = xx;
    y = yy;
    z = zz;

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

	glColor3f(1.0f, 1.0f, 1.0f);
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

    float vT = vit;

    bool b = true;

    while(b){

        float d = sqrt((x-getPointSuiv().X)*(x-getPointSuiv().X)+(y-getPointSuiv().Y)*(y-getPointSuiv().Y));
        if(d<vT){
            vT = vT - d;
//            cout << cPt << arc.list_point_annexe.size() << endl;
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
                z = 0;
            } else {
                cPt += 1;
                x = getPointPrec().X;
                y = getPointPrec().Y;
                z = getPointPrec().Z;
                z = 0.0f;
            }
        } else
            b = false;
    }
    float a = (getPointPrec().Y-getPointSuiv().Y)/(getPointPrec().X-getPointSuiv().X);

    float dx = fabs(vT/sqrt(1+a*a));
    float dy = fabs(a*dx);

    if(getPointPrec().X>getPointSuiv().X)
        x -= dx;
    else
        x += dx;

    if(getPointPrec().Y>getPointSuiv().Y)
        y -= dy;
    else
        y += dy;
}
