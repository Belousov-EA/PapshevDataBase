/*
 * CellString.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef CELLSTRING_H_
#define CELLSTRING_H_

#include "Cell.h"

class CellString : public Cell{
public:
	CellString():value(){};
	virtual ~CellString(){};
	virtual string getValue()
	{
		return value;
	}
	virtual void setValue(string v)
	{
		value = v;
	}
	virtual bool compare(Cell* other)
	{
		return value > other->getValue();
	}
private:
	string value;
};

#endif /* CELLSTRING_H_ */
