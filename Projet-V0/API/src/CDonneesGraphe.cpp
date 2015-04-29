/*
 * CDonneesGraphe.cpp
 *
 *  Created on: 24 d�c. 2009
 *      Author: girouxi
 */
using namespace std;
#include "consts.h"
#include "CDonneesGraphe.h"
#include <fstream>
#include <iostream>
#include <sstream>
CDonneesGraphe::CDonneesGraphe() {
	// TODO Auto-generated constructor stub

}

CDonneesGraphe::~CDonneesGraphe() {
	// TODO Auto-generated destructor stub
}

CPoint3f lowerLeft(const CPoint3f &p1, const CPoint3f &p2){
	float xMin = MIN(p1.X, p2.X);
	float yMin = MIN(p1.Y, p2.Y);
	float zMin = MIN(p1.Z, p2.Z);
	return (CPoint3f(xMin, yMin, zMin));
}

CPoint3f upperRight(const CPoint3f &p1, const CPoint3f &p2){

	float xMax = MAX(p1.X, p2.X);
	float yMax = MAX(p1.Y, p2.Y);
	float zMax = MAX(p1.Z, p2.Z);
	return (CPoint3f(xMax, yMax, zMax));
}

CPoint3f lowerLeft(vector<CPoint3f> pointVector, const CPoint3f &p0){
	CPoint3f ll = p0;
	for (vector<CPoint3f>::iterator iter = pointVector.begin(); iter < pointVector.end(); ++iter) {
		ll = lowerLeft(ll, *iter);
	}
	return (ll);
}

CPoint3f upperRight(vector<CPoint3f> pointVector, const CPoint3f &p0){
	CPoint3f ur = p0;
	for (vector<CPoint3f>::iterator iter = pointVector.begin(); iter < pointVector.end(); ++iter) {
		ur = upperRight(ur, *iter);
	}
	return (ur);
}

CDonneesGraphe::CDonneesGraphe(string sxyz_file_name,
		string sif_file_name,
		string paxyz_file_name,
		string axyz_file_name)
{
	CPoint3f LL(RNOVA);
	CPoint3f UR(-RNOVA);
	load_array_3d(sxyz_file_name, SXYZ);
	NS = SXYZ.size();
	cout<<"NS = "<<NS<<endl;
	load_array_2i(sif_file_name, SIF);
	NA = SIF.size();
	cout<<"NA = "<<NA<<endl;
	load_array_2i(paxyz_file_name, PAXYZ);
	load_array_3d(axyz_file_name, AXYZ);
	NPA = AXYZ.size();
	LL = lowerLeft(SXYZ, LL);
	LL = lowerLeft(AXYZ, LL);
	UR = upperRight(SXYZ, UR);
	UR = upperRight(AXYZ, UR);
	cout<<"LL \n"<< LL.toString() <<endl;
	cout<<"UR \n"<< UR.toString() <<endl;
	reshapePAXYZ();

}

void CDonneesGraphe::load_array_2i(string &file_name,
		vector<CData2i> &tab)
{

	cout << "loading " <<file_name << " : debut..." << endl;
	ifstream fichier( file_name.data() );
	char line[TAILLE_MAX + 1] = "";
	if (!fichier) {
		cout<<"probleme lecture fichier : "<<file_name<<" inexistant ou deja ouvert\n";
		return;
	}
	// ce test �choue si le fichier n'est pas ouvert

	string ligne; // variable contenant chaque ligne lue

	// cette boucle s'arr�te d�s qu'une erreur de lecture survient
	while ( getline( fichier, ligne ) )
	{
		// afficher la ligne � l'�cran
		strcpy(line, ligne.data());
		CData2i p(NOVAL,NOVAL);
		int col_count = sscanf(line, "%d %d", &p.colonne_1, &p.colonne_2);
		if (col_count == 2)
		{
			tab.push_back(p);
		}


		cout << line << endl;
	}
	cout << "loading " <<file_name << " : fin" << endl;
}

void CDonneesGraphe::load_array_3d(string &file_name,
		vector<CPoint3f> &tab)
{
	cout << "loading " <<file_name << " : debut..." << endl;
	ifstream fichier( file_name.data() );
	char line[TAILLE_MAX + 1] = "";
	if (! fichier) {
		cout<<"probleme lecture fichier : "<<file_name<<" inexistant ou deja ouvert\n";
		return;
	}
	// ce test �choue si le fichier n'est pas ouvert
	string ligne; // variable contenant chaque ligne lue

	// cette boucle s'arr�te d�s qu'une erreur de lecture survient
	while ( getline( fichier, ligne ) )
	{
		// afficher la ligne � l'�cran
		strcpy(line, ligne.data());
		CPoint3f p(RNOVA, RNOVA, RNOVA);
		int col_count = sscanf(line, "%f %f %f", &p.X, &p.Y, &p.Z);
		if (col_count == 3)
		{
			tab.push_back(p);
		}
		//cout << line << endl;
	}
	cout << "loading " <<file_name << " : fin" << endl;
}
/**
 *
 */
void CDonneesGraphe::afficheDonnees(){
	cout<< "\n SIF\n";
	    for (int i=0; i<NA; i++)
	    {
	        CData2i data = SIF.at(i);
	        cout<< data.colonne_1<< " "<< data.colonne_2<<endl;
	    }

	    cout<< "\n SXYZ\n";
	    for (int i=0; i<NS; i++)
	    {
	        CPoint3f p = SXYZ.at(i);
	        cout<< p.X<< " "<< p.Y<<" "<<p.Z<<endl;
	    }

	    cout<< "\n PAXYZ\n";
	    for (int i=0; i<NA; i++)
	    {
	        CData2i p = PAXYZ.at(i);
	        cout<< p.colonne_1<< " "<< p.colonne_2<<endl;
	    }
	    cout<< "\n AXYZ\n";
	    for (int i=0; i<NPA; i++)
	    {
	        CPoint3f p = AXYZ.at(i);
	        cout<< p.X<< " "<< p.Y<<" "<<p.Z<<endl;
	    }
}
void CDonneesGraphe::reshapePAXYZ(){
	for (int i = 0; i < NA; ++i) {
		CData2i data_pannex = PAXYZ.at(i);
		int nb_pa = data_pannex.colonne_2 - data_pannex.colonne_1 + 1;
		PAXYZ.at(i).colonne_2 = nb_pa;
	}
}
