#include "BasicToken.h"

BasicToken::BasicToken(TokenType type, string lexeme)
{
	_type = type;
	_lexeme = lexeme;
}

void BasicToken::printToken() //Prints the token type and Lexeme.
{
	cout << "Token Type:" << (int)_type << " Lexeme: " << _lexeme << endl;
}

