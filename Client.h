/*
 * Client.h
 *
 *  Created on: May 30, 2018
 *      Author: user
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "DataBase.h"

class Client {
public:
	Client();
	void loop();
private:
	int state;
	DataBase currDataBase;
	DBTableTxt currTable;
	vector<string> dataBasesNames;
	int screenWidth;
};

#endif /* CLIENT_H_ */
