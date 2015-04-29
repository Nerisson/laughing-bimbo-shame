//============================================================================
// Name        : Geom2.cpp
// Author      : giroux
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "CDonneesGraphe.h"
#include "CGraphe.h"
#include "CSommet.h"
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;
int test_main() {

	cout<< "Hello \n";
	// construction des tables du graphe
	CDonneesGraphe gdata("SXYZ.TXT", "SIF.TXT", "PAXYZ.TXT", "AXYZ.TXT");
	// affichage des donnees
	gdata.afficheDonnees();
	// Constrcuction de la base de donnees orientee objet du graphe
	CGraphe graphe(gdata);
	// Preparation du generateur de nombre aleatoire
	srand((unsigned)time(NULL));
	// Tirage d'un nombre aleatoire entre 0 et NS (nombre de sommet)
	int iSommet = rand() % gdata.NS;
	cout<<iSommet<<endl;
	// Recuperation du sommet
	CSommet sommet = graphe.list_sommet.at(iSommet);
	cout<<sommet.toString()<<endl;
	// Recuperation arcs entrants
	vector<CArc> list_arc_entrant = graphe.getListArcEntrantDansSommet(sommet);
	int nb_arc_entrant = list_arc_entrant.size();
	for (int i = 0; i < nb_arc_entrant; ++i) {
		CArc arc = list_arc_entrant.at(i);
		cout<<arc.toString()<<endl;
	}
	graphe.afficheSommets();
	graphe.afficheArcs();
	graphe.buildExtent();
	cout<<graphe.getExtentLowerLeft().toString()<<endl;
	cout<<graphe.getExtentUpperRight().toString()<<endl;
    return EXIT_SUCCESS;
}
