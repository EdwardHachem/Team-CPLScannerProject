#include "BasicToken.h"

BasicToken::BasicToken(TokenType type, string lexeme, int linenumber)
{
	this->linenumber = linenumber;
	this->type = type;
	this->lexeme = lexeme;
}

void BasicToken::printToken() //Prints the token type and Lexeme.
{
	cout << "Line: " << linenumber << " Token Type:" << (int)type << " Lexeme: " << lexeme << endl;
}

