/*
 * CArc.cpp
 *
 *  Created on: 22 déc. 2009
 *      Author: girouxi
 */

#include "CArc.h"
#include <string>
#include <stdio.h>
using namespace std;
CArc::CArc() {
}
CArc::CArc(int idarc, int sini, int sfin):id_arc(idarc), id_sommet_ini(sini), id_sommet_fin(sfin) {
}
string CArc::toString(){
	char data[1024] = "";
	sprintf(data,"--arc identifiant : %d ---\n"
			"`-> sommet ini %d \n"
			"`-> sommet fin %d\n" ,id_arc, id_sommet_ini, id_sommet_fin);
	string str(data);
	return str;
}
