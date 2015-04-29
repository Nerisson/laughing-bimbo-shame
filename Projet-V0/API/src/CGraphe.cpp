/*
 * CGraphe.cpp
 *
 *  Created on: 22 d�c. 2009
 *      Author: girouxi
 */
using namespace std;
#include "CGraphe.h"
#include <iostream>
#include <string>
CGraphe::CGraphe(CDonneesGraphe gdata)
{
	buildGrapheObjectsFromData(gdata);
	extent[0] = CPoint3f(RNOVA);
	extent[1] = CPoint3f(-RNOVA);
	buildExtent();
}
void CGraphe::buildGrapheObjectsFromData(CDonneesGraphe gdata){
	nb_sommet = gdata.NS;
	nb_arc = gdata.NA;
	nb_point_annex = gdata.NPA;
	buildSommetsFromData(gdata);
	buildArcsFromData(gdata);
	buildPointsAnnexesFromData(gdata);
}
void CGraphe::buildSommetsFromData(CDonneesGraphe gdata)
{
	list_sommet.resize(gdata.NS);
	for (int isommet = 0; isommet < gdata.NS; ++isommet) {
		// Creation Creation de l'objet sommet initial,
		CPoint3f coord_sommet = gdata.SXYZ.at(isommet); // coordonnees sommet ini
		CSommet sommet_ini(isommet,coord_sommet);
		list_sommet[isommet] = *(new CSommet(isommet, coord_sommet));
	}
	cout << "buildSommetsFromData...fin\n";
}//buildSommetsFromData


CPoint3f CGraphe::getExtentUpperRight(){
	return extent[1];
}

CPoint3f CGraphe::getExtentLowerLeft(){
	return extent[0];
}

void CGraphe::buildArcsFromData(CDonneesGraphe gdata){
	// association sommet-arc
	// definition des sommets initial et final d'un arc
	// definition d'arc entrants ou sortant d'un sommet
	bool sommet_arc[2][gdata.NS][gdata.NA];
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < gdata.NS; ++j) {
			for (int k = 0; k < gdata.NA; ++k) {
				sommet_arc[i][j][k] = false;
			}
		}
	}
	for (int iarc = 0; iarc < gdata.NA; ++iarc) {
		CData2i ligne_sif = gdata.SIF.at(iarc);
		// pour nous la numerotation commence a partir de 0
		// et non a partir de 1
		int sini = ligne_sif.colonne_1-1; // num sommet ini
		int sfin = ligne_sif.colonne_2-1; // num sommet fin
		// Creation de l'objet arc,
		CArc arc_new(iarc, sini, sfin);
		if (!sommet_arc[0][sini][iarc]) {
			sommet_arc[0][sini][iarc] = true;
			list_sommet[sini].id_arc_sortant.push_back(iarc);
		}
		if (!sommet_arc[1][sini][iarc]) {
			sommet_arc[1][sini][iarc] = true;
			list_sommet[sfin].id_arc_entrant.push_back(iarc);
		}
		list_arc.push_back(arc_new);
	}
	cout<<"buildArcsFromData...fin\n";
}//buildArcsFromData

void CGraphe::buildPointsAnnexesFromData(CDonneesGraphe gdata){
	list_point_annexe.resize(gdata.NPA);
	int index_pa = -1;
	for (int iarc = 0; iarc < gdata.NA; ++iarc) {
		CData2i data_pannex = gdata.PAXYZ.at(iarc);
		int nb_pannex = data_pannex.colonne_2;
		for (int ipa = 0; ipa < nb_pannex; ++ipa) {
			index_pa++;
			CPoint3f coord_point_annex = gdata.AXYZ.at(index_pa);
			CPointAnnexe pannex(index_pa, coord_point_annex);
			list_arc.at(iarc).list_point_annexe.push_back(index_pa);
			list_point_annexe[index_pa] = pannex;
		}
	}
	cout<<"buildPointsAnnexesFromData...fin\n";
}//buildPointsAnnexesFromData

/**
 * donne la liste des arcs entrants
 */
vector<CArc> CGraphe::getListArcEntrantDansSommet(const CSommet &sommet)
{
	vector<CArc> list_arc_entrant;
	int nb_arc_entrant = sommet.id_arc_entrant.size();
	for (int i = 0; i < nb_arc_entrant; ++i) {
		int id_arc = sommet.id_arc_entrant.at(i);
		list_arc_entrant.push_back(list_arc.at(id_arc));
	}
	return list_arc_entrant;
}

void CGraphe::updateExtent(const CPoint3f &p){
//	CPoint3f p = list_point_annexe.at(n);
	CPoint3f* lowerL = &extent[0];
	CPoint3f* upperR = &extent[1];
	cout<<"Before : "<<lowerL->toString()<<endl;
	cout<<"Before : "<<upperR->toString()<<endl;
	lowerL->X = MIN(lowerL->X, p.X);
	upperR->X = MAX(upperR->X, p.X);
	lowerL->Y = MIN(lowerL->Y, p.Y);
	upperR->Y = MAX(upperR->Y, p.Y);
	lowerL->Z = MIN(lowerL->Z, p.Z);
	upperR->Z = MAX(upperR->Z, p.Z);
	cout<<"After : "<<lowerL->toString()<<endl;
	cout<<"After : "<<upperR->toString()<<endl;
}

void CGraphe::buildExtent(){
	for (vector<CPointAnnexe>::iterator it = list_point_annexe.begin(); it < list_point_annexe.end(); ++it) {
		updateExtent(*it);
	}
	for (vector<CSommet>::iterator it = list_sommet.begin(); it < list_sommet.end(); ++it) {
		updateExtent(*it);
	}
}

/**
 * donne la liste des arcs qui sortent de s
 */
vector<CArc> CGraphe::getListArcSortantDuSommet(const CSommet &sommet)
{
	vector<CArc> list_arc_sortant;
	int nb_arc_entrant = sommet.id_arc_sortant.size();
	for (int i = 0; i < nb_arc_entrant; ++i) {
		int id_arc = sommet.id_arc_sortant.at(i);
		list_arc_sortant.push_back(list_arc.at(id_arc));
	}
	return list_arc_sortant;
}

/**
 * Donne la liste des points annexes de l'arc pass� en param�tre
 */
vector<CPointAnnexe> CGraphe::getListPointAnnexeArc(const CArc &arc){
	vector<CPointAnnexe> list_pannexe;
	int nb_pannexe = arc.list_point_annexe.size();
	for (int i = 0; i < nb_pannexe; ++i) {
		int id_arc = arc.list_point_annexe.at(i);
		list_pannexe.push_back(list_point_annexe.at(id_arc));
	}
	return list_pannexe;
}


void CGraphe::afficheSommets(){
	for (int i = 0; i < nb_sommet; ++i) {
		cout<<list_sommet.at(i).toString()<<endl;
	}
}

void CGraphe::afficheArcs(){
	for (int i = 0; i < nb_arc; ++i) {
		cout<<list_arc.at(i).toString()<<endl;
	}
}

void CGraphe::afficheListArcEntrantDansSommet(CSommet &sommet){
	// R�cup�ration arcs entrants
	vector<CArc> list_arc_entrant = getListArcEntrantDansSommet(sommet);
	int nb_arc_entrant = list_arc_entrant.size();
	for (int i = 0; i < nb_arc_entrant; ++i) {
		CArc arc = list_arc_entrant.at(i);
		cout<<arc.toString()<<endl;
	}
}
void CGraphe::afficheListArcSortantDuSommet(CSommet &sommet){
	// R�cup�ration arcs entrants
	vector<CArc> list_arc_sortant = getListArcSortantDuSommet(sommet);
	int nb_arc_entrant = list_arc_sortant.size();
	for (int i = 0; i < nb_arc_entrant; ++i) {
		CArc arc = list_arc_sortant.at(i);
		cout<<arc.toString()<<endl;
	}
}
