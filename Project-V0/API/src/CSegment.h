/*
 * CSegment.h
 *
 *  Created on: 22 déc. 2009
 *      Author: girouxi
 */

#ifndef CSEGMENT_H_
#define CSEGMENT_H_

class CSegment {
public:
	int id_segment;
	int id_point_annex_ini;
	int id_point_annex_fin;
	int id_arc;
	CSegment();
	virtual ~CSegment();
};

#endif /* CSEGMENT_H_ */
