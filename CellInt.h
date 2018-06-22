/*
 * CellInt.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef CELLINT_H_
#define CELLINT_H_

#include <sstream>
#include <iostream>
#include <stdlib.h>


using namespace std;

#include "Cell.h"

class CellInt : public Cell {
public:
	CellInt():value(0){};
	virtual ~CellInt(){};
	virtual string getValue()
	{
		string buf;
		stringstream ss;
		ss << value;
		buf = ss.str();
		return buf;
	}

	virtual void setValue(string v)
	{
		value = atoi(v.c_str());
	}

	virtual bool compare(Cell* other)
	{
		int otherValue = atoi(other->getValue().c_str());
		return value > otherValue;
	}
private:
	int value;
};

#endif /* CELLINT_H_ */
