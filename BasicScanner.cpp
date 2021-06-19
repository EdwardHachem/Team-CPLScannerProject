#include "BasicScanner.h"

BasicScanner::BasicScanner(string sourceCode)
{
	_sourceCode = sourceCode;
}

list<BasicToken> BasicScanner::scanTokens()
{
	while (_current < _sourceCode.length())
	{
		// New Lexeme
		_start = _current;
		scanToken();
	}
	return _tokens;
}

void BasicScanner::scanToken()
{
	char c = advanceChar();
	switch (c)
	{
	case '"':
		addStringToken();
		break;

	case ' ':
	case '\r':
	case'\t':
	case '\n':
		break;

	case '\'':
		addToken(keywords["'"], "'");
		break;

	case '*':
		addToken(keywords["*"], "*");
		break;

	case '+':
		addToken(keywords["+"], "+");
		break;

	case '-':
		addToken(keywords["-"], "-");
		break;

	case '\\':
		addToken(keywords["\\"], "\\");
		break;

	case '=':
		addToken(keywords["="], "=");
		break;

//Now check for 2 character operators before one character.
	case '<':
		switch( _sourceCode[_current])
		{
		case '=':
			addToken(keywords["<="], "<=");
			_current++;
			break;
		case '>':
			addToken(keywords["<>"], "<>");
			_current++;
			break;
		default:
			addToken(keywords["<"], "<");
		}
		break;

	case '>':
		switch( _sourceCode[_current])
		{
		case '=':
			addToken(keywords[">="], ">=");
			_current++;
			break;
		default:
			addToken(keywords[">"], ">");
		}
		break;
	default:
		if (isDigit(c))
		{
			addNumberToken();
		}
		else if (isAlpha(c)) //Adds keywords and identifiers
		{
			addKeywordToken(); 
		}
		else
		{
			cout << "Invalid input." << endl;
		}
	}
}

void BasicScanner::initKeywords()
{
	keywords["'"] = TokenType::APOSTROPHE;
	keywords["*"] = TokenType::MULTIPLY;
	keywords["+"] = TokenType::ADD;
	keywords["-"] = TokenType::SUBTRACT;
	keywords["/"] = TokenType::DIVIDE;
	keywords["<"] = TokenType::LESSTHAN;
	keywords["<="] = TokenType::LESSTHANOREQUALTO;
	keywords["<>"] = TokenType::NOTEQUAL;
	keywords["="] = TokenType::EQUAL;
	keywords[">"] = TokenType::GREATERTHAN;
	keywords[">="] = TokenType::GREATERTHANOREQUALTO;
	keywords["and"] = TokenType::AND;
	keywords["as"] = TokenType::AS;
	keywords["cdbl"] = TokenType::CDBL;
	keywords["cint"] = TokenType::CINT;
	keywords["clong"] = TokenType::CLONG;
	keywords["continue"] = TokenType::CONTINUE;
	keywords["csng"] = TokenType::CSNG;
	keywords["dim"] = TokenType::DIM;
	keywords["do"] = TokenType::DO;
	keywords["double"] = TokenType::DOUBLE;
	keywords["else"] = TokenType::ELSE;
	keywords["end"] = TokenType::END;
	keywords["exit"] = TokenType::EXIT;
	keywords["if"] = TokenType::IF;
	keywords["integer"] = TokenType::INTEGER;
	keywords["long"] = TokenType::LONG;
	keywords["loop"] = TokenType::LOOP;
	keywords["mod"] = TokenType::MOD;
	keywords["not"] = TokenType::NOT;
	keywords["or"] = TokenType::OR;
	keywords["rem"] = TokenType::REM;
	keywords["single"] = TokenType::SINGLE;
	keywords["then"] = TokenType::THEN;
	keywords["until"] = TokenType::UNTIL;
	keywords["wend"] = TokenType::WEND;
	keywords["while"] = TokenType::WHILE;
	keywords["xor"] = TokenType::XOR;
}

char BasicScanner::advanceChar()
{
	return _sourceCode[_current++];
}

void BasicScanner::addToken(TokenType type, string data)
{
	_tokens.push_back(BasicToken(type, data));
}

void BasicScanner::addStringToken()
{
	while (_current < _sourceCode.length() && 
		_sourceCode[_current] != '"')
	{
		_current++;
	}
	if (_sourceCode[_current] == '"')
	{
		string strliteral = _sourceCode.substr(_start + 1, _current - 1);
		addToken(TokenType::STRINGLITERAL, strliteral);
	}
}

bool BasicScanner::isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '_');
}

bool BasicScanner::isAlphaNumeric(char c)
{
	return isAlpha(c) || isDigit(c);
}

bool BasicScanner::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

void BasicScanner::addKeywordToken()
{
	while (_current < _sourceCode.length() && isAlphaNumeric(_sourceCode[_current]))
	{
		_current++;
	}
	string identifier =  _sourceCode.substr(_start, _current);
	for (int i = 0; i < identifier.length(); i++)
	{
		identifier[i] = tolower(identifier[i]);
	}
	map<string, TokenType>::iterator it;
	it = keywords.find(identifier);
	if (it != keywords.end())
	{
		//Found the token in our keywords list so this is a keyword
		addToken(it->second, identifier);
	}
	else
	{
		//Token not in our keywords list, so this is an identifier
		addToken(TokenType::IDENTIFIER, identifier);

	}
}

void BasicScanner::addNumberToken()
{
	while (_current < _sourceCode.length() &&
		isDigit(_sourceCode[_current]))
	{
		_current++;
	}
	if (_sourceCode[_current] == '.' && _current < _sourceCode.length())
	{
		if (isDigit(_sourceCode[_current + 1]))
		{
			_current++;
			while (_current < _sourceCode.length() &&
				isDigit(_sourceCode[_current]))
			{
				_current++;
			}
		}
	}
	string numliteral = _sourceCode.substr(_start, _current);
	addToken(TokenType::NUMBERLITERAL, numliteral);
}

list<BasicToken> BasicScanner::getTokens()
{
	return _tokens; 
}


