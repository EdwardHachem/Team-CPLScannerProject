#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum class TokenType {
	AND = 0, AS, CDBL, CINT, CLONG, CONTINUE, CSNG, DIM, DO, DOUBLE, ELSE, END, EXIT, IF, INTEGER, LONG, LOOP, MOD,
	NOT, OR, REM, SINGLE, THEN, UNTIL, WEND, WHILE, XOR, APOSTROPHE, MULTIPLY, ADD, SUBTRACT, DIVIDE, LESSTHAN, 
	LESSTHANOREQUALTO, NOTEQUAL, EQUAL, GREATERTHAN, GREATERTHANOREQUALTO, SQR, EXP,
	GOTO, PRINT, DEF, FOR, LET, NEXT, INT, RND, LEFTPAREN, RIGHTPAREN, RETURN, STOP, GOSUB, INPUT, READ, NEGATE,
	STRINGLITERAL, NUMBERDBLLITERAL, NUMBERINTLITERAL, IDENTIFIER, PROGRAMLINENUMBER, EOL, TO
};
class BasicToken
{
public:
	BasicToken(TokenType type, string lexeme, int linenumber);
	void printToken();

	int linenumber;
	TokenType type;
	string lexeme;
};

