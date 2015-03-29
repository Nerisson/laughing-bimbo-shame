#include "CSommet.h"
#include "consts.h"
#include <stdio.h>
#include <algorithm>
using namespace std;
CSommet::CSommet():id_sommet(NOVAL){
	CPoint3f();
}
CSommet::CSommet(int id):id_sommet(id){
	CPoint3f();
}
CSommet::CSommet(int id, CPoint3f p):CPoint3f(p), id_sommet(id){
}
string CSommet::toString(){
	char data[80] = "";
	sprintf(data, "--sommet identifiant : %d ---\n"
			"`->coord. : %f %f %f",
			id_sommet, X, Y, Z);
	string str(data);
	//
	int NA = id_arc_entrant.size();
	sprintf(data,"\n`-> %d Arc(s) entrant(s) : ---> ",NA);
	str +=data;
	for (int i = 0; i < NA; ++i) {
		sprintf(data, "%d ", id_arc_entrant.at(i));
		str+=data;
	}
	//
	NA = id_arc_sortant.size();
	sprintf(data,"\n`-> %d Arc(s) sortant(s) : ---> ",NA);
	str +=data;
	for (int i = 0; i < NA; ++i) {
		sprintf(data, "%d ", id_arc_sortant.at(i));
		str+=data;
	}
	str+="\n";
	//
//	std::remove(str.begin(), str.end(), ' ');
	return str;
}
