/*
 * CPointAnnexe.cpp
 *
 *  Created on: 22 déc. 2009
 *      Author: girouxi
 */

#include "CPointAnnexe.h"
#include <stdio.h>
using namespace std;
CPointAnnexe::CPointAnnexe() {
}
CPointAnnexe::CPointAnnexe(int id, const CPoint3f &p):CPoint3f(p), id_point_annex(id) {
}

CPointAnnexe::~CPointAnnexe() {
	// TODO Auto-generated destructor stub
}
string CPointAnnexe::toString(){
	char data[1024] = "";
	sprintf(data,"--point annexe identifiant : %d ---\n"
			"`->coord. : %f %f %f",
			id_point_annex, X, Y, Z);
	string str(data);
	return str;
}
