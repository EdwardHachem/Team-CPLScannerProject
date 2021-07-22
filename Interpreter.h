#pragma once
#include "BasicParser.h"
#include <stack>
using namespace std;

class Interpreter
{
public:
	void init(P_struct* pTable, int count, map <string, Identifier> *pidentifiers, map <int, int> *plinenumberToPx);
	void interpret();
	Identifier *createIdentifier(const string& name, int value);
	Identifier *getItem(bool& dblValue1, double& dval1, int& val1);
	string getUniqueName();


private:
	P_struct* _pTable;
	int _count;
	map <string, Identifier>* _pidentifiers;
	map <int, int>* _plinenumberToPx;
	int _currentPx = 0;
	stack<P_struct> S;
	string _unique = "__identifier";
	int _uniqueint = 0;
};

