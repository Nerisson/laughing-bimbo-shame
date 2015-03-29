/*
 * CArc.h
 *
 *  Created on: 22 déc. 2009
 *      Author: girouxi
 */

#ifndef CARC_H_
#define CARC_H_
#include <vector>
#include <string>
class CArc {
public:
	int id_arc;
	int id_sommet_ini;
	int id_sommet_fin;
	CArc();
	CArc(int, int, int);
	std::string toString();
	std::vector<int> list_point_annexe;
};

#endif /* CARC_H_ */
