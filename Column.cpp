/*
 * Column.cpp
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#include "Column.h"

#include <stdlib.h>

#include <iostream>
using namespace std;

int Column::getLength()
{
	return length;
}

void Column::setLength(int l)
{
	length = l;
}

string Column::getName()
{
	return name;
}

void Column::setName(string n)
{
	name = n;
}

int Column::getType()
{
	return type;
}

void Column::setType(int t)
{
	type = t;
}


void Column::setType(string t)
{
	if(t == "NoType")
	{
		type = 0;
	}else if(t == "Int32")
	{
		type = 1;
	}else if(t == "Double")
	{
		type = 2;
	}else if(t == "String")
	{
		type = 3;
	}else
	{
		cerr << "Unknown type" << endl;
		system("pause");
	}
}



string Column::getTypeStr()
{
	return columnTypes[type];
}

void Column::setLength(string len)
{
	length = atoi(len.c_str());
}



