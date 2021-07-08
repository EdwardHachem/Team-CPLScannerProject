#pragma once
#include <string>
using namespace std;

class Lines {

};
class Line 
{
};

class Statements 
{
};

class Statement 
{
public:
	void IF() {
		//expression();
		//getNextToken(); // must be "THEN"
		//thenClause();
	}


	//thenClause() {
	//	getNextToken(); //int 
	//}

};

class BNF
{

public:
	void parseLines(Lines lines, Line line)
	{};
	void parseLines(Line line)
	{
	}
	void parseLine (int linenumber, Statements statements)
	{}
	void parseStatements(Statements statements, string colon, Statement statement)
	{}
	void parseStatements(Statement statement)
	{}
	void parseStatement(int i)
	{}
	void parseStatement(string str)
	{
		//get next token
		// if "IF" call IF()
	}
};
