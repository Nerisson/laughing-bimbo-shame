#ifndef CSOMMET_H_INCLUDED
#define CSOMMET_H_INCLUDED
#include <vector>
#include <string>
#include "CPoint3f.h"
class CSommet:public CPoint3f
{
	public:
		int id_sommet;
		std::vector<int> id_arc_entrant;
		std::vector<int> id_arc_sortant;
		CSommet();
		CSommet(int);
		CSommet(int, CPoint3f);
		CSommet(float, float, float);
		std::string toString();
	protected:

	private:

};
#endif // CSOMMET_H_INCLUDED
