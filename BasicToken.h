#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum class TokenType {
	AND, AS, CDBL, CINT, CLONG, CONTINUE, CSNG, DIM, DO, DOUBLE, ELSE, END, EXIT, IF, INTEGER, LONG, LOOP, MOD,
	NOT, OR, REM, SINGLE, THEN, UNTIL, WEND, WHILE, XOR, APOSTROPHE, MULTIPLY, ADD, SUBTRACT, DIVIDE, LESSTHAN, 
	LESSTHANOREQUALTO, NOTEQUAL, EQUAL, GREATERTHAN, GREATERTHANOREQUALTO, STRINGLITERAL, NUMBERLITERAL, IDENTIFIER
};
class BasicToken
{
public:
	BasicToken(TokenType type, string lexeme, string data = "");
private:
	TokenType _type;
	string _lexeme;
	string _data;
};

