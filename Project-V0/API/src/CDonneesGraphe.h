/*
 * CDonneesGraphe.h
 *
 *  Created on: 24 déc. 2009
 *      Author: girouxi
 */

#ifndef CDONNEESGRAPHE_H_
#define CDONNEESGRAPHE_H_
#include <vector>
#include <string.h>
#include "CData2i.h"
#include "CPoint3f.h"
class CDonneesGraphe {
public:
	std::vector <CPoint3f> SXYZ;
	int NS;
	std::vector<CData2i> SIF;
	int NA;
	std::vector<CData2i> PAXYZ;
	std::vector<CPoint3f> AXYZ;
	int NPA;
	void load_array_2i(std::string &, std::vector<CData2i > &);
	void load_array_3d(std::string &,
			std::vector<CPoint3f > &);
	void load_file(std::string file_name);
	void afficheDonnees();
	void reshapePAXYZ();
	CDonneesGraphe(std::string sxyz_file_name, std::string, std::string, std::string);
	CDonneesGraphe();
	virtual ~CDonneesGraphe();
};

#endif /* CDONNEESGRAPHE_H_ */
