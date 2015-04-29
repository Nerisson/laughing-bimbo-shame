/*
 * CGraphe.h
 *
 *  Created on: 22 d�c. 2009
 *      Author: girouxi
 */

#ifndef CGRAPHE_H_
#define CGRAPHE_H_
#include <vector>
#include <string>
#include "CSegment.h"
#include "CArc.h"
#include "CSommet.h"
#include "CPointAnnexe.h"
#include "CDonneesGraphe.h"
#include "consts.h"
class CGraphe {
public:


	CGraphe(CDonneesGraphe gdata);
	void buildGrapheObjectsFromData(CDonneesGraphe gdata);
	void buildArcsFromData(CDonneesGraphe gdata);
	void buildPointsAnnexesFromData(CDonneesGraphe gdata);
	void buildSommetsFromData(CDonneesGraphe gdata);
	void buildExtent();


	std::vector<CSommet> list_sommet;
	int nb_sommet;
	std::vector<CArc> list_arc;
	int nb_arc;
	std::vector<CPointAnnexe> list_point_annexe;
	int nb_point_annex;
	std::vector<CArc> getListArcEntrantDansSommet(const CSommet &sommet);
	std::vector<CArc> getListArcSortantDuSommet(const CSommet &sommet);
	std::vector<CPointAnnexe> getListPointAnnexeArc(const CArc &arc);
	void afficheSommets();
	void afficheArcs();
	void afficheListArcEntrantDansSommet(CSommet &sommet);
	void afficheListArcSortantDuSommet(CSommet &sommet);
	CPoint3f extent[2];
	void updateExtent(const CPoint3f&);
	CPoint3f getExtentUpperRight();
	CPoint3f getExtentLowerLeft();

};

#endif /* CGRAPHE_H_ */
