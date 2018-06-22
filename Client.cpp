/*
 * Client.cpp
 *
 *  Created on: May 30, 2018
 *      Author: user
 */

#include "Client.h"


#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

#define MAIN_MENU 0
#define EXITE 3
#define CREATE_BASE 1
#define OPEN_BASE 2
#define CREATE_FILE 4
#define WATCHING_BASE 5
#define OPEN_TABLE 6
#define WATCHING_TABLE 7
#define ADD_ROW 8
#define SORT_TABLE 9
#define GET_ROW 10
#define GET_VALUE 11
#define DEL_ROW 12
#define DEL_BASE 14

Client::Client()
{
	state = 0;
	screenWidth = 80;
}


void Client::loop()
{
	cout << "Добро пожаловать"<<endl;
	while (true)
	{
		if(state == MAIN_MENU)
		{
			ofstream ofs("DataBasesList.txt");
			string comand = "ls >> DataBasesList.txt";
			system(comand.c_str());
			ifstream ifs("DataBasesList.txt", ios::binary|ios::in);
			string line;
			dataBasesNames.clear();
			while(getline(ifs, line))
			{
				if((line.find(".")==string::npos)&&(line!= "Debug"))
				{
					dataBasesNames.push_back(line);
				}
			}
			cout << "Main menu" << endl << endl;
			cout << "Доступные базы данных:" << endl;
			for(int i = 0; i < (int)dataBasesNames.size(); i++)
			{
				cout << dataBasesNames[i]<< endl;
			}
			int userIn;
			do{
				cout << endl << endl;
				cout << "Для продоолжения работы выбирете одно из действий"<<endl
						<<"[1] - create new Base" << endl
						<<"[2] - open exist Base" << endl
						<<"[3] - exite"<< endl;


				cin >> userIn;
				if(userIn == 1) state = CREATE_BASE;
				if(userIn == 2) state = OPEN_BASE;
				if(userIn == 3) state = EXITE;
			}while(state == MAIN_MENU);
		}

		if(state == EXITE)
		{
			cout<< endl << endl << "Goodbay" << endl;
			return;
		}

		if(state == CREATE_BASE)
		{
			DataBase base;
			cout << "Creating new data base" << endl << endl;
			cout << "Input data base name"<<endl;
			string name;
			cin >> name;
			base.createBase(name);
			cout << "Data Base " << name << " is creating" << endl << endl;
			state = MAIN_MENU;
		}

		if (state == OPEN_BASE)
		{
			int input;
			if(dataBasesNames.size() == 0)
			{
				do
				{
					cout << "There are not any bases to open" <<endl
							<<"[1] - create base" << endl
							<<"[2] - go to main menu" << endl;
					cin >> input;
					if(input == 1) state = CREATE_BASE;
					if(input == 2) state = MAIN_MENU;
				}while(state == OPEN_BASE);
			}else
			{

				cout << "Выберете базу для работы:" << endl;
				for(int i = 0; i < (int)dataBasesNames.size(); i++)
				{
					cout << "[" << i+1 << "] - " << dataBasesNames[i] << endl;
				}
				cin >> input;
				cout << endl;
				currDataBase.setName(dataBasesNames[input - 1]);
				state = WATCHING_BASE;
			}
		}




		if(state == CREATE_FILE)
		{
			string name;
			cout << "Введите имя таблицы" << endl;
			cin >> name;
			string path = name + ".txt";
			if(std::find(currDataBase.getTablesNames().begin(),
					currDataBase.getTablesNames().end(),
					path) != currDataBase.getTablesNames().end())
			{
				cout << "Таблица с таким именем уже существует" << endl;
			}else
			{
				//заполнение заголовка:
				Header head;
				string continueWord;
				do{
					Column column;
					string colName;
					cout << "Ввeдите имя столбца"<< endl;
					cin >> colName;
					column.setName(colName);
					int input = 0;
					do{
						cout << "Выберете тип, хранимый в этом столбце"<< endl <<
								"[1] - Int32" << endl <<
								"[2] - Double" << endl <<
								"[3] - String" << endl;

						cin >> input;
						if(input == 1) column.setType(Int32);
						if(input == 2) column.setType(Double);
						if(input == 3) column.setType(String);
						if((input != 1) && (input !=2) && (input !=3))
						{
							cout << "Неправильный ввод, попробуйте еще раз" << endl;
						}
					}while(input != 1 && input !=2 && input !=3);
					cout << "Введите максимальную длину," << endl <<
							"хранимую в этом столбце" << endl;
					do{
						cin >> input;
						if(input < 1)
						{
							cout << "Недостаточно места" << endl;
						}else
						{
							column.setLength(input);
						}
					}while(input < 1);
					head[colName] = column;

					cout << "Хотите ли вы добавить еще один столбец" <<
							endl<<"y/n"<< endl;
					cin >> continueWord;
				}while(continueWord == "y" || continueWord == "Y"
						|| continueWord == "yes"|| continueWord == "Yes");
				cout << "Введите primaryKey"<< endl;
				string key;
				cin >> key;
				currDataBase.createTable(name, head, key);

			}
			state = WATCHING_BASE;
		}

		if (state == WATCHING_BASE)
		{
			int input;
			cout << "Data base "<< currDataBase.getName()<< " is open" << endl << endl;
			cout << currDataBase.getName()<<endl;
			cout<<setfill('-')<<setw(screenWidth-1)<<'-'<<setfill(' ')<<endl;
			for(int i = 0; i < (int)currDataBase.getTablesNames().size(); i++)
			{
				cout << currDataBase.getTablesNames()[i] << endl;
			}
			do{
				cout <<endl << endl;
				cout << "Для продоолжения работы выбирете одно из действий"<<endl
								<<"[1] - go to main menu" << endl
								<<"[2] - create Table" << endl
								<<"[3] - open Table"<< endl
								<<"[4] - удалить базу" << endl;
				cin >> input;
				if(input == 1) state = MAIN_MENU;
				if(input == 2) state = CREATE_FILE;
				if(input == 3) state = OPEN_TABLE;
				if(input == 4) state = DEL_BASE;
			}while(state == WATCHING_BASE);
		}

		if(state == OPEN_TABLE)
		{
			cout << "Выбирете таблицу из списка"<< endl;
			if((int)currDataBase.getTablesNames().size() == 0)
			{
				cout << "В данной базе не существует ни одной таблицы"<<endl;
				state = WATCHING_BASE;
			}

			for(int i = 0; i < (int)currDataBase.getTablesNames().size(); i++)
			{
				cout << "[" << i+1 <<"] - " << currDataBase.getTablesNames()[i] << endl;
			}
			cout << "[0] - Назад" << endl;
			int input;
			do{
				cin >> input;
				if(input > 0 && input < (int)currDataBase.getTablesNames().size()+1)
				{
					currTable.read(currDataBase.getTablePathTxt(currDataBase.getTablesNames()[input - 1]));
					state = WATCHING_TABLE;
				}else
					if(input == 0)
					{
						state = WATCHING_BASE;
					}
					else
					{
						cout << "Неправильный ввод"<< endl;
					}
			}while(state == OPEN_TABLE);

		}
		if(state == WATCHING_TABLE)
		{
			do{
			currTable.print(screenWidth);
			cout << endl<<endl;
			cout << "Выберете одно из действий с таблицей "<< currTable.getName()<< ":"<<endl;
			cout << "[1] - добавить строку"<<endl;
			cout << "[2] - сортировать" << endl;
			cout << "[3] - получить строку" << endl;
			cout << "[4] - получить значение" << endl;
			cout << "[5] - удалить строку"<< endl;
			cout << "[6] - выйти в меню базы данных" << endl;
			cout << "[7] - выйти в главное меню"<< endl;

			int input;
			cin >> input;
			if(input == 1) state = ADD_ROW;
			if(input == 2) state = SORT_TABLE;
			if(input == 3) state = GET_ROW;
			if(input == 4) state = GET_VALUE;
			if(input == 5) state = DEL_ROW;
			if(input == 6) state = WATCHING_BASE;
			if(input == 7) state = MAIN_MENU;
			if(state == WATCHING_TABLE)
			{
				cout << "Повторите ввод"<< endl;
			}
			}while(state == WATCHING_TABLE);
		}

		if(state == ADD_ROW)
		{
			Header head = currTable.getHead();
			vector<string> cellsVector;
			for(Header::iterator it = head.begin();
					it != head.end();
					it++)
			{
				cout << "Введите значение поля " << it->second.getName()<<endl
						<<"типа "<< it->second.getTypeStr() << endl
						<< "длины не более "<< it->second.getLength()<<endl;
				string input;
				cin >> input;
				cellsVector.push_back(input);
			}
			currTable.addRow(cellsVector);
			currTable.write(currDataBase.getTablePath(currTable.getName()));
			int input;
			do{
				cout << "Выберете одно из следующих действий:"<< endl<<endl;
				cout << "[1] - Выйти в меню таблицы"<< endl;
				cout << "[2] - Выйти в меню базы данных" << endl;
				cout << "[3] - Добавить еще один столбец" << endl;

				cin >> input;
				if(input == 1) state = WATCHING_TABLE;
				if(input == 2) state = WATCHING_BASE;
				if(input == 3) state = ADD_ROW;
				if(!(input == 1||input == 2 || input == 3))
				{
					cout << "Неправильный ввод"<< endl << "Попробуйте еще раз" << endl;
				}
			}while(!(input == 1 || input == 2 || input == 3));
		}

		if(state == SORT_TABLE)
		{
			do{
				int i = 1;
				vector<string> names;
				cout << "Выберете поле, по которому"<<endl<< "хотите произвести сортировку" << endl<< endl;
				Header head = currTable.getHead();
				for(Header::iterator it = head.begin();
						it!= head.end();
						it++)
				{
					cout << "["<< i<<"] - "<<it->second.getName()<<endl;
					i++;
					names.push_back(it -> second.getName());
				}
				cout << "[0] - Выйти в меню таблицы" << endl<<endl;
				int input;
				cin >> input;
				if(input > i || input < 1)
				{
					cout << "Повторите ввод" << endl;
				}else if(input == 0)
				{
					state = WATCHING_TABLE;
				}else
				{
					currTable.sort(names[input - 1]);
					currTable.write(currDataBase.getTablePathTxt(currTable.getName()));
					state = WATCHING_TABLE;
				}
			}while(state == SORT_TABLE);
		}

		if(state == GET_VALUE)
		{
			bool flag;
			int inputForSearch;
			int inputToSearch;
			string value;
			vector<string> headNames;
			Header head = currTable.getHead();
			do{
				cout << "Выберете столбец, по которому должен проводиться поиск:" << endl;
				int i = 0;
				for(Header::iterator it = head.begin();
						it!= head.end();
						it++)
				{
					cout <<"[" << i+1 << "]" << it->second.getName()<<endl;
					i++;
					headNames.push_back(it->second.getName());
				}
				cin >> inputForSearch;
				flag = false;
				if(inputForSearch > i+1 || inputForSearch < 1)
				{
					cout << "Повторите ввод" << endl;
					flag = true;
				}
			}while(flag);
			do{
				cout << "Выберете столбец, значение которого нужно вывести:" << endl;
				vector<string> headNames;
				int i = 0;
				for(Header::iterator it = head.begin();
						it!= head.end();
						it++)
				{
					cout <<"[" << i+1 << "]" << it->second.getName()<<endl;
					i++;
				}
				cin >> inputToSearch;
				flag = false;
				if(inputToSearch > i+1 || inputToSearch < 1)
				{
					cout << "Повторите ввод" << endl;
					flag = true;
				}
			}while(flag);
			inputForSearch--;
			inputToSearch --;
			cout << "Введите значение "<< head[headNames[inputForSearch]].getName()<<endl
					<<"типа " << head[headNames[inputForSearch]].getTypeStr() << endl;
			cin >> value;
			vector<int> rowNums = currTable.getIndexByValue(headNames[inputForSearch], value);
			for(int i = 0; i < (int)rowNums.size(); i++)
			{
				Row row = currTable.getRow(rowNums[i]);
				cout << row[headNames[inputToSearch]]->getValue()<<endl;
			}
			state = WATCHING_TABLE;

		}

		if(state == GET_ROW)
		{
			vector<string> headNames;
			Header head = currTable.getHead();
			int input;
			string value;
			bool flag;
			do{
				cout << "Выберете столбец, по которому должен проводиться поиск:" << endl;
				int i = 0;
				for(Header::iterator it = head.begin();
						it!= head.end();
						it++)
				{
					cout <<"[" << i+1 << "]" << it->second.getName()<<endl;
					i++;
					headNames.push_back(it->second.getName());
				}
			cin >> input;
				flag = false;
				if(input > i+1 || input < 1)
				{
					cout << "Повторите ввод" << endl;
					flag = true;
				}
			}while(flag);
			input --;
			cout << "Введите значение " << head[headNames[input]].getName() << endl <<
					"типа "<< head[headNames[input]].getTypeStr() << endl;
			cin >> value;
			vector<int> rowsNums = currTable.getIndexByValue(headNames[input], value);
			DBTableTxt table;
			table.setHead(head);
			table.setName(currTable.getName());
			table.setPrimaryKey("NoPrimaryKey");
			for(int i = 0; i < (int)rowsNums.size(); i++)
			{
				Row row = currTable.getRow(rowsNums[i]);
				table.addRow(row);
			}
			table.print(screenWidth);
			state = WATCHING_TABLE;
		}
		if(state == DEL_ROW)
		{
			vector<string> headNames;
			Header head = currTable.getHead();
			int input;
			string value;
			bool flag;
			do{
				cout << "Выберете столбец, по которому должно проводиться удаление:" << endl;
				int i = 0;
				for(Header::iterator it = head.begin();
						it!= head.end();
						it++)
				{
					cout <<"[" << i+1 << "]" << it->second.getName()<<endl;
					i++;
					headNames.push_back(it->second.getName());
				}
				cin >> input;
				flag = false;
				if(input > i+1 || input < 1)
				{
					cout << "Повторите ввод" << endl;
					flag = true;
				}
			}while(flag);
			input --;
			cout << "Введите значение " << head[headNames[input]].getName() << endl <<
					"типа "<< head[headNames[input]].getTypeStr() << endl;
			cin >> value;
			vector<int> rowsNums = currTable.getIndexByValue(headNames[input], value);
			for(int i = 0; i < (int)rowsNums.size(); i++)
			{
				currTable.delRow(rowsNums[i] - i);
				currTable.write(currDataBase.getTablePath(currTable.getName()));
			}
			state = WATCHING_TABLE;
		}

		if(state == DEL_BASE)
		{
			currDataBase.deleteBase();
			state = OPEN_BASE;
			vector<string> newDataBasesNames;
			for(int i = 0; i < (int)dataBasesNames.size(); i ++)
			{
				if(dataBasesNames[i] != currDataBase.getName())
				{
					newDataBasesNames.push_back(dataBasesNames[i]);
				}
			}
			dataBasesNames = newDataBasesNames;
		}
	}


}


