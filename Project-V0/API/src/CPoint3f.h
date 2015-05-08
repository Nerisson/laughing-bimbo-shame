/*
 * CPoint3.h
 *
 *  Created on: 22 d�c. 2009
 *      Author: girouxi
 */

#ifndef CPOINT3_H_
#define CPOINT3_H_
#include <string>
class CPoint3f {
public:
	float X;
	float Y;
	float Z;
	CPoint3f(const float&, const float&, const float&);
	CPoint3f(const float&);
	CPoint3f();
	CPoint3f(const CPoint3f&);
	std::string toString();
};

#endif /* CPOINT3_H_ */
