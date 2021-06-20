#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum class TokenType {
	AND = 0, AS, CDBL, CINT, CLONG, CONTINUE, CSNG, DIM, DO, DOUBLE, ELSE, END, EXIT, IF, INTEGER, LONG, LOOP, MOD,
	NOT, OR, REM, SINGLE, THEN, UNTIL, WEND, WHILE, XOR, APOSTROPHE, MULTIPLY, ADD, SUBTRACT, DIVIDE, LESSTHAN, 
	LESSTHANOREQUALTO, NOTEQUAL, EQUAL, GREATERTHAN, GREATERTHANOREQUALTO, STRINGLITERAL, NUMBERLITERAL, IDENTIFIER
};
class BasicToken
{
public:
	BasicToken(TokenType type, string lexeme);
	void printToken();

private:
	TokenType _type;
	string _lexeme;
};

