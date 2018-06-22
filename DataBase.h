/*
 * DataBase.h
 *
 *  Created on: May 29, 2018
 *      Author: user
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "DBTableTxt.h"
#include <map>
#include <string>


class DataBase {
public:
	void createTable(string name, Header head, string primaryKey);
	void deleteTable(string name);
	vector<string> getTablesNames();
	void createBase(string name);
	void deleteBase();
	string getName()
	{
		return name;
	}
	string getTablePathTxt(string n);
	void setName(string n);
	string getTablePath(string name);
private:
	//map<string, DBTableTxt> tables;
	string name;
	string path;
	void renewDataFile();

	vector<string> tableNames;
};

#endif /* DATABASE_H_ */
