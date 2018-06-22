/*
 * DBTableTxt.h
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#ifndef DBTABLETXT_H_
#define DBTABLETXT_H_

#include <iostream>
using namespace std;

#include <string>

#include <vector>

#include <map>

#include "Cell.h"

#include "CellInt.h"
#include "CellDouble.h"
#include "CellString.h"

#include "Column.h"

//types:
enum DBType {
		 NoType,
		 Int32,
		 Double,
		 String,
		// Date
	};


typedef map<string, Cell*> Row;
typedef map<string, Column> Header;

struct Strip{//полоса распечатки таблицы
	int nField;//число полей
	int* fieldWidth;//ширина полей в полосе (массив)
};


class DBTableTxt {
public:
	DBTableTxt();
	~DBTableTxt();

	void read(string tab);
	void write(string tab);
	void print(int screenWidth);
	void addRow(Row row);
	Row getRow(int index);
	vector<int> getIndex(Row row);
	vector<int> getIndexByValue(string columnName, string value);
	void sort(string columnName);
	Header getHead();
	void setHead(Header hdr);
	void setName(string name);
	void setPrimaryKey(string primaryKey);
	string getName()
	{
		return name;
	}
	void addRow(vector<string> input);
	void delRow(int index);
private:
	vector<Row> data;
	string name;
	string primaryKey;
	Header head;

	//private functions:
	vector<string> getVectorWordsOfString(string str, string delSymbol);
	void CreateTableMaket(Strip *&strips,int &nStrips,int screenWidth);
};

#endif /* DBTABLETXT_H_ */
