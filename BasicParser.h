#pragma once
#include <list>
#include <stack>
#include <map>
#include "BasicToken.h"
#include "Identifier.h"

class BasicParser
{
public:
	BasicParser() {};
	void init(list<BasicToken>* ptokens, map<string, Identifier>* pidentifiers);
	virtual void parse();

	void parseLine();
	void parseStatements();
	void parseStatement();

	char dividers[3] = { ',', ';', ':' };
private:
	list<BasicToken> *_ptokens;
	map<string, Identifier > *_pidentifiers;
	list<BasicToken>::iterator _tokenIt;

	int P[1000];

	void error(const string& msg, int linenumber) {
		cout << linenumber << " " << msg << endl;
	}
};


