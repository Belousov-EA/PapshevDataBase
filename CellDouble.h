/*
 * CellDouble.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef CELLDOUBLE_H_
#define CELLDOUBLE_H_

#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Cell.h"

class CellDouble: public Cell {
public:
	CellDouble() : value(0){};
	virtual ~CellDouble(){};

	virtual void setValue(string v)
	{
		value = atof(v.c_str());
	}
	virtual string getValue()
	{
		string buf;
		stringstream ss;
		ss << value;
		buf = ss.str();
		return buf;
	}
	virtual bool compare(Cell* other)
	{
		double otherValue = atof(other->getValue().c_str());
		return value > otherValue;
	}

private:
	double value;
};

#endif /* CELLDOUBLE_H_ */
