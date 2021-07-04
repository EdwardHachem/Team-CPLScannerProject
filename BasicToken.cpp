#include "BasicToken.h"

BasicToken::BasicToken(TokenType type, string lexeme, int linenumber)
{
	_linenumber = linenumber;
	_type = type;
	_lexeme = lexeme;
}

void BasicToken::printToken() //Prints the token type and Lexeme.
{
	cout << "Line: " << _linenumber << " Token Type:" << (int)_type << " Lexeme: " << _lexeme << endl;
}

