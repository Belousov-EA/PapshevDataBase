/*
 * DBTableTxt.cpp
 *
 *  Created on: May 28, 2018
 *      Author: user
 */

#include "DBTableTxt.h"

#include <fstream>
#include <iostream>
#include <iomanip>

DBTableTxt::DBTableTxt() {
	// TODO Auto-generated constructor stub

}

DBTableTxt::~DBTableTxt() {
	Row::iterator it;
	int dataSize = (int)data.size();
	for(int i = 0; i < dataSize; i++)
	{
		it = data[i].begin();
		delete it->second;
	}
}


vector<string> DBTableTxt::getVectorWordsOfString(string str, string delSymbol)
{
	//перобразование строки в список arr:
	size_t prev = 0;
	size_t next;
	size_t delta = delSymbol.length();
	vector<string> arr;

	while( ( next = str.find( delSymbol, prev ) ) != string::npos ){
	  //Отладка-start
	  string tmp = str.substr( prev, next-prev );
	  //Отладка-end
	  arr.push_back( str.substr( prev, next-prev ) );
	  prev = next + delta;
	}
	//Отладка-start
	string tmp = str.substr( prev );
	//Отладка-end
	arr.push_back( str.substr( prev ) );
	return arr;
}


void DBTableTxt::read(string tab)
{
	string delSymbol = "|";
	ifstream ifs(tab.c_str(), ios::binary|ios::in);
	if(ifs.is_open())
	{
		// get first line
		string line;
		vector<string> vectorOfLine;
		if(getline(ifs, line))
		{
			vectorOfLine = getVectorWordsOfString(line, delSymbol);
			if(vectorOfLine.size() == 2)
			{
				name = vectorOfLine[0];
				primaryKey = vectorOfLine[1];
				//test:
				//cout << tableName<< endl << primaryKey << endl;
			}else
			{
				cerr << "Wrong first line of the table" << endl;
			}
		}
		head.clear();
		if(getline(ifs, line))
		{
			vectorOfLine.clear();
			vectorOfLine = getVectorWordsOfString(line, delSymbol);
			if(vectorOfLine.size()%3 == 0)
			{
				vector<string>::size_type vectorOfLineSize = vectorOfLine.size();
				for(vector<string>::size_type i = 0; i < vectorOfLineSize; i = i + 3)
				{
					Column column;
					//columnHeader.colName = vectorOfLine[i];
					column.setName(vectorOfLine[i]);
					column.setType(vectorOfLine[i+1]);
					column.setLength(vectorOfLine[i+2]);
					head.insert(pair<string, Column>(vectorOfLine[i], column));
				}
			}else
			{
				cerr << "Wrong second line of table" << endl;
			}

			//other lines:
			data.clear();
			while(getline(ifs, line))
			{
				Row row;
				//Row row=*(new Row());
				vectorOfLine.clear();
				vectorOfLine = getVectorWordsOfString(line, delSymbol);
				vector<string>::size_type vectorOfLineSize = vectorOfLine.size();
				Header::iterator it = head.begin();
				for(vector<string>::size_type i = 0; i < vectorOfLineSize; ++i)
				{
					if(it -> second.getType() == Int32)
					{
						CellInt* cell = new CellInt;
						row[it->second.getName()] = cell;
					}else if(it -> second.getType() == Double)
					{
						CellDouble* cell = new CellDouble;
						row[it -> second.getName()] = cell;
					}else if(it -> second.getType() == String)
					{
						CellString* cell = new CellString;
						row[it -> second.getName()] = cell;
					}
					row[it -> second.getName()]->setValue(vectorOfLine[i]);



					//string value = vectorOfLine[i];
					//row[it->first] = GetValue(value, it->first, columnHeaders);
					//row[it->first] = GetValue(value, it->first, columnHeaders);
					//row[it->second.colName] = GetValue(value, it->second.colName, columnHeaders);
					it++;
				}
				data.push_back(row);
				//cout << "it is true"<<endl;
			}
		}
		}else
	{
		cerr << "File is not found"<< endl;
		return;
	}
}



void DBTableTxt::write(string tab)
{
	string tabName = getVectorWordsOfString(tab, ".")[0];
	while(tabName.find("/")!=string::npos)
	{
		tabName = getVectorWordsOfString(tabName, "/")[1];
	}
	if(tabName != name)
	{
		cout<<"Имя таблицы в fileName "<<name<<
				" не соответствует имени таблицы "<<tabName<< ",указанному в DBTableTxt\n";
		system("pause");
		return;
	}
	ofstream fout;//открытие файлового потока на вывод
	fout.open(tab.c_str());//,ios::binary|ios::out);
	if (!fout.is_open()){
		cout<<"Ошибка открытия файла "<<tab<<endl;
		system("pause");
		return;
	}

	Header::iterator iter,iterWhile;
	Row ::iterator dataIter,dataIterWhile;
	//first line:
	fout<<name<<'|'<<primaryKey<<endl;

	//second line:
	iter=head.begin();
	iterWhile=iter;//чтобы не печатать в цикле последнюю строку; т.к. нет
	//операций iter+1 и iter<, используем ++iterWhile (префиксный инкремент)

	while(++iterWhile!=head.end())
	{
		fout<< iter->second.getName()<<"|"<< iter->second.getTypeStr()<<
			"|"<<iter->second.getLength()<<"|";
		iter++;
	}
	fout<<iter->second.getName()<<"|"<< iter->second.getTypeStr()
			<<"|"<<iter->second.getLength() <<endl;

	//other lines:
	int nRows=(int)data.size();
	for (int i = 0; i < nRows; i++)
	{


		iter = head.begin();
		iterWhile = iter;
		while(++iterWhile!=head.end())
		{
			fout<<data[i][iter->second.getName()] -> getValue()<<"|";
			iter++;
		}
		fout<<data[i][iter->second.getName()]->getValue()<<endl;
	}
	fout.close();
}



void DBTableTxt::CreateTableMaket(Strip *&strips,int &nStrips,int screenWidth)
{
	Header::iterator headerIter,contHeaderIter;
	int nColumn = (int)head.size();
	//Заполнение массива ширины полей для печати таблицы
	int* fieldW=new int[nColumn];
	headerIter=head.begin();
	for(int k=0;k<nColumn; k++){
		//что шире: заголовок или данные?
		fieldW[k]=headerIter->second.getLength() > headerIter->first.size() ?
			headerIter->second.getLength()+2 : headerIter->first.size()+2;
		headerIter++;
	}
	int currCol=0;//порядковый номер столбца в таблице
	nStrips=1;//число полос в распечатке таблицы
	int sumWidth=0;//суммарная ширина столбцов в полосе
	int n=0;//число столбцов в полосе
	int buff[40]={0};//объявление и обнуление буфера для временного
	//хранения числа столбцов в полосе (n<40)
	for(currCol=0; currCol<nColumn; currCol++){
		if(fieldW[currCol]>=screenWidth-1){
			cout<<"Ширина столбца "<<currCol<<" больше ширины экрана\n";
			cout<<"Таблицу нельзя распечатать"<<endl;
			nStrips=0;
			return;
		}
		sumWidth+=fieldW[currCol];
		if((sumWidth<screenWidth-1)&&(currCol<nColumn-1)){
			n++;
			continue;
		}
		if((sumWidth>=screenWidth-1)){//выход за границу экрана
			currCol--;
			buff[nStrips-1]=n;
			nStrips++;
			n=0;
			sumWidth=0;
			continue;
		}
		if(currCol==nColumn-1){//последняя полоса
			n++;
			buff[nStrips-1]=n;
		}
	}
	//выделение памяти и заполнение макета таблицы strips из буфера buff.
	//strips - "ступенчатый" массив размеров столбцов strips[i].fieldWidth
	//в полосах таблицы, т.е. числа столбцов в полосе могут  отличатся.
	//Для его создания используется структура Strip (сущность более высокого,
	//по сравнению со строкой, уровня, которая скрывает различия в числе столбцов
	//в строках разных полос в макете таблицы. В результате таблица печатается в виде полос
	//одинаковой ширины (за исключением последней полосы). В C# для этого есть
	//специальный тип данных - "ступенчатый массив"(массив из массивов различной длины).
	//Это может рассматриваться как пример "обобщенного программирования" для печати
	//таблиц произвольного размера.
	strips=new Strip[nStrips];
	int col = 0;
	for(int i=0;i<nStrips;i++){
		strips[i].nField=buff[i];
		strips[i].fieldWidth=new int[strips[i].nField];
		for(int j=0;j<strips[i].nField;j++)
			strips[i].fieldWidth[j]=fieldW[col++];
	}
}


void DBTableTxt::print(int screenWidth)
{
	Strip * strips;//выходной параметр функции CreateTableMaket
	int nStrips;//выходной параметр функции CreateTableMaket
	CreateTableMaket(strips,nStrips,screenWidth);
	int nColumn = (int)head.size();
	Header::iterator headerIter,contHeaderIter;
	Row::iterator rowIter, contRowIter;
	cout<<"\nТаблица "<<name<<endl;
	cout<<setfill('=')<<setw(screenWidth-1)<<'='<<setfill(' ')<<endl;
	contHeaderIter=head.begin();
	for ( int r = 0; r < nStrips; r++)
	{//печать заголовка
		//вывод наименований столбцов в заголовке полосы
		headerIter=contHeaderIter;
		for (int j = 0; j < strips[r].nField; j++)
			cout<<setw(strips[r].fieldWidth[j])<< headerIter++->second.getName();
		cout<<endl;
		//вывод типа данных в столбцах в заголовке полосы
		headerIter=contHeaderIter;
		for (int j = 0; j < strips[r].nField; j++)
			cout<<setw(strips[r].fieldWidth[j])<< headerIter++->second.getTypeStr();
		cout<<endl<<setfill('-')<<setw(screenWidth-1)<<'-'<<setfill(' ')<<endl;
		//печать строк таблицы
		int nRows = (int)data.size();
		for (int i = 0; i < nRows; i++)
		{
			//cout.write((*(string*)data[i]["Group"]).c_str(),8); cout<<endl;

			if(r==0){//установка итераторов на начало строки в нулевой полосе
				rowIter=data[i].begin();
				contRowIter=rowIter;

			}
			rowIter=contRowIter;//продолжение строк таблицы в новой полосе
			for (int j=0; j<strips[r].nField; j++)
			{
				//				if(dbName=="LibraryBin"||dbName=="CompanyBin")
				//			 		cout<<(*(string*)(data[i][rowIter->first])).c_str();//бинарный файл
				////				cout<<(char*)row[pr.first]<<endl;
				//				else
				//				//текстовый файл
				cout<<setw(strips[r].fieldWidth[j])<< rowIter->second->getValue();
				rowIter++;
			}
			cout<<endl;
		}
		cout<<setfill('=')<<setw(screenWidth-1)<<'='<<setfill(' ')<<endl;
		if(r<nStrips-1){//установка итераторов продолжения печати таблицы в новой полосе
			contHeaderIter=headerIter;
			contRowIter=rowIter;
			cout<<"\nПродолжение таблицы "<< name<<endl;
			cout<<setfill('=')<<setw(screenWidth-1)<<'='<<setfill(' ')<<endl;
		}
	}
	//Освобождение памяти
	for(int i=0;i<nStrips;i++)
		//for(int j=0;i<strips[i].nField;i++)//можно было бы использовать для удаления Strip
		//здесь возможна утечка
		delete []strips[i].fieldWidth;//виртуальный деструктор
	delete []strips;//вызывающая функция Print владеет объектом strips. Память
	//для strips выделяется в функции CreateTableMaket(strips,nStrips,screenWidth)
}


void DBTableTxt::addRow(Row row)
{
	data.push_back(row);
}


Row DBTableTxt::getRow(int index)
{
	if(index >= (int)data.size())
	{
		cerr << "Out of range" << endl;
		system("pause");
	}
	return data[index];
}


vector<int>  DBTableTxt::getIndex(Row row)
{
	vector<int> out;
	int dataSize = (int)data.size();
	for(int i = 0; i < dataSize; i++)
	{
		bool isEqual = true;
		for(Row::iterator it = data[i].begin();
				it!=data[i].end();
				it++)
		{
			if(it->second->getValue() != row[it->first]->getValue())
			{
				isEqual = false;
			}
		}
		if (isEqual)
		{
			out.push_back(i);
		}
	}
	return out;
}


vector<int> DBTableTxt::getIndexByValue(string columnName, string value)
{
	vector<int> out;
	int dataSize = (int)data.size();
	for(int i = 0; i < dataSize; i++)
	{
		if(data[i][columnName] -> getValue() == value)
		{
			out.push_back(i);
		}
	}
	return out;
}


Header DBTableTxt::getHead()
{
	return head;
}


void DBTableTxt::sort(string columnName)
{
	for( int i = 0; i < data.size(); i++)
	{
		int assignmentOnTheIter = 0;
		for (int j = 0; j < data.size() - 1 - i; j++)
		{
			if(data[j][columnName]->compare(data[j+1][columnName]))
			{
				Row t = data[j];
				data[j] = data[j+1];
				data[j+1] = t;
				assignmentOnTheIter++;
			}
		}
		if(!assignmentOnTheIter)
		{
			return;
		}
	}
}


void DBTableTxt::setHead(Header hdr)
{
	head = hdr;
}

void DBTableTxt::setName(string n)
{
	name = n;
}

void DBTableTxt::setPrimaryKey(string key)
{
	primaryKey = key;
}

void DBTableTxt::addRow(vector<string> input)
{
	int i = 0;
	Row row;
	for(Header::iterator it = head.begin();
			it != head.end();
			it++)
	{
		if(it -> second.getType() == Int32)
		{
			CellInt* cell = new CellInt;
			row[it->second.getName()] = cell;
		}else if(it -> second.getType() == Double)
		{
			CellDouble* cell = new CellDouble;
			row[it -> second.getName()] = cell;
		}else if(it -> second.getType() == String)
		{
			CellString* cell = new CellString;
			row[it -> second.getName()] = cell;
		}
		row[it -> second.getName()]->setValue(input[i]);
		i++;
	}
	addRow(row);
}

void DBTableTxt::delRow(int index)
{
	vector<Row> newData;
	for(int i = 0; i < (int)data.size(); i ++)
	{
		if(i != index)
		{
			newData.push_back(data[i]);
		}
	}
	data = newData;
}


