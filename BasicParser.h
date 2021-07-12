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

	char dividers[3] = {',', ';', ':'};
private:
	list<BasicToken> *_ptokens;
	map<string, Identifier> *_pidentifiers;
	list<BasicToken>::iterator _tokenIt;


	enum class CMD {	BZ, BR, BP, 
				ASSIGN, PLUS, MINUS, MULT, DIV, POWER,
				EQ, GT, GTE, LT, LTE, NE, NOT, RND, SQR, EXP, INPUT, INT,
				STRING, CONST, IDENTIFIER, NOP,
				PRINT, LINENUMBER
			};

	struct P_struct{
		CMD cmd;
		union {
			int value;
			Identifier* ptr;
		};
	};

	P_struct P[10000];
	int Px=0;

	map<int, int> ProgramLineNumberToPx;
	void pushP(CMD, Identifier *ptr);
	void pushP(CMD, int value=0);

	bool advanceToken();
	void error(const string& msg, int linenumber) {
		cout << linenumber << " " << msg << endl;
	}
};

