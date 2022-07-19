#pragma once
#include "defs.h"
#include <map>
using namespace std;

typedef char IM[MaxText]; // текст ИМ
typedef char LEX[MaxLex];
typedef string LEXs;

class TScaner
{
	IM t; // исходный модуль
	int uk; // указатель текущей позиции в ИМ
	int ukLine;
	int ukInLine;
	int line;
	int pos; //строка, позиция
	map <string, int> KW;
	map <int, string> ErrorMessages;

public:
	TScaner();	
	void SetUK(int newUK,int newUKLine, int newUKInLine);
	int GetUK() const;
	int GetUKLine() const;
	int GetUKInLine() const;
	int GetTypeOfNextLex(LEXs *l);
	void PrintError(int er_type, int i, LEX l) const; // выдать сообщение об ошибке;
	void PrintError(int oldukLine, int oldUkInLine, int er_type, LEXs l); // выдать сообщение об ошибке;
	int scan(LEX l); // основная функция сканера
	int scan(LEXs *l); // основная функция сканера
};