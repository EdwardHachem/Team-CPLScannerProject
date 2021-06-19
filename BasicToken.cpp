#include "BasicToken.h"

BasicToken::BasicToken(TokenType type, string lexeme, string data)
{
	_type = type;
	_lexeme = lexeme;
	_data = data;
}
