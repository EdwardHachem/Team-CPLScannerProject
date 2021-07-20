#pragma once
#include <list>
#include <stack>
#include <map>
#include "BasicToken.h"
#include "Identifier.h"

enum class CMD {
	BZ, BR, BP,
	ASSIGN, PLUS, MINUS, MULT, DIV, POWER,
	EQ, GT, GTE, LT, LTE, NE, NOT, RND, SQR, EXP, INPUT, INT,
	STRING, CONST, IDENTIFIER, NOP,
	PRINT, LINENUMBER, STACKINT, STACKDOUBLE
};

struct P_struct {
	CMD cmd;
	union {
		int value;
		double dblValue;
		Identifier* ptr;
	};
};

class BasicParser
{
public:
	BasicParser() {};
	void init(list<BasicToken>* ptokens, map<string, Identifier>* pidentifiers);
	virtual void parse();

	void parseLine();
	void parseStatements();
	void parseStatement();
	int parseExpression();
	int parseAndExpression();
	int parseNotExpression();
	int parseCompareExpression();
	int parseAddExpression();
	int parseMultExpression();
	int parseNegateExpression();
	int parseFunctionExpression();
	int parseFOR();
	int parsePowerExpression();
	BasicToken parseValue();
	int parseIdentifierList();
	int parsePrintList();
	P_struct* getPtable();
	int getPtableCount();
	map <int, int>* getLinenumberMap();

	char dividers[3] = {',', ';', ':'};
private:
	list<BasicToken> *_ptokens;
	map<string, Identifier> *_pidentifiers;
	list<BasicToken>::iterator _tokenIt;

	P_struct P[1000];
	int Px=0;

	map<int, int> ProgramLineNumberToPx;
	void pushP(CMD, Identifier *ptr);
	void pushP(CMD, int value=0);

	bool advanceToken();
	void error(const string& msg, int linenumber) {
		cout << linenumber << " " << msg << endl;
	}
};

