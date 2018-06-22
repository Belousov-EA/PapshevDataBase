/*
 * Cell.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef CELL_H_
#define CELL_H_

#include <string>

using std::string;

class Cell {
public:
	Cell(){};
	virtual ~Cell(){};
	virtual bool compare(Cell* other) = 0;
	virtual string getValue() = 0;
	virtual void setValue(string value) = 0;
};

#endif /* CELL_H_ */
