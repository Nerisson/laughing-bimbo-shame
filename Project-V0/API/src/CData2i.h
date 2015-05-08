/*
 * CPoint2.h
 *
 *  Created on: 22 déc. 2009
 *      Author: girouxi
 */

#ifndef CDATA2I_H_
#define CDATA2I_H_

class CData2i {
public:
	int colonne_1,colonne_2;
	CData2i(int, int);
	CData2i();
	CData2i(const CData2i&);
};

#endif /* CDATA2D_H_ */
