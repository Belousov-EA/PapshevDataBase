/*
 * DataBase.cpp
 *
 *  Created on: May 29, 2018
 *      Author: user
 */

#include "DataBase.h"
#include <fstream>

void DataBase::createTable(string n, Header h, string key)
{
	bool isExist = false;
	name = n;
	tableNames = getTablesNames();
	for(int i = 0; i < tableNames.size(); i++)
	{
		if(n == tableNames[i])
		{
			isExist = true;
			cout << "Таблица с таким именем уже существует" << endl;
		}
	}
	if(!isExist)
	{
		DBTableTxt table;
		table.setHead(h);
		table.setName(n);
		table.setPrimaryKey(key);
		string namefile = path+"/"+name+".txt";
		std::FILE* file = fopen(namefile.c_str(), "w");
		table.write(namefile);
		renewDataFile();
	}
}


void DataBase::renewDataFile()
{
	string fileName = path+"/DataFile.txt";
	ofstream ofs;
	ofs.open(fileName.c_str());
	ofs.close();
	string comand = "ls "+ path +" >> " + path + "/DataFile.txt";
	system(comand.c_str());
}

void DataBase::deleteTable(string n)
{
	string comand = "rm "+path+"/"+n+".txt";
	system(comand.c_str());
	renewDataFile();
}

string DataBase::getTablePath(string n)
{
	string tab = path+ "/"+ n+ ".txt";
	return tab;
}

string DataBase::getTablePathTxt(string n)
{
	string tab = path+"/" + n;
	return tab;
}

vector<string> DataBase::getTablesNames()
{
	string tab = name + "/DataFile.txt";
	ifstream ifs(tab.c_str(), ios::binary|ios::in);
	tableNames.clear();
	string line;
	string end = ".txt";
	while(getline(ifs, line))
	{
		if(line.find(end) != string::npos && line != "DataFile.txt")
		{
			tableNames.push_back(line);
		}
	}
	return tableNames;
}

void DataBase::createBase(string _name)
{
	name = _name;
	path = name;
	string comand = "mkdir "+ name;
	system(comand.c_str());
	comand = "touch "+getTablePath("DataFile");
	system(comand.c_str());
}

void DataBase::setName(string _name)
{
	name = _name;
	path = name;
	renewDataFile();

}

void DataBase::deleteBase()
{
	string comand = "rm -r "+ name;
	system(comand.c_str());
}



