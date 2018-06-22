/*
 * Column.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef COLUMN_H_
#define COLUMN_H_

#include <string>
using namespace std;




class Column {
public:
	int getType();
	string getTypeStr();
	void setType(int type);
	void setType(string type);
	string getName();
	void setName(string name);
	int getLength();
	void setLength(int length);
	void setLength(string length);
private:
	int type;
	string name;
	int length;
	string columnTypes[4] = {
		"NoType",
		"Int32",
		"Double",
		"String"
	};
};

#endif /* COLUMN_H_ */
