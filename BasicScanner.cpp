#include "BasicScanner.h"
#include "Identifier.h"
#include "LanguageProcessor.h"

BasicScanner::BasicScanner()
{
	
	initKeywords();
}

BasicScanner::BasicScanner(const string& sourceCode)
{
	initKeywords();
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
	addToken(TokenType::EOL, "");
	return _allTokens;
}

void BasicScanner::setSourceCode(map <string, Identifier>& identifiers, const string& sourceCode, int linenumber)
{
	_newLine = true;
	_linenumber = linenumber;
	_sourceCode = sourceCode;
	_identifiers = &identifiers;
}

void BasicScanner::clear()
{
	_allTokens.clear();
	_sourceCode.clear();
	_current = 0;
	_start = 0;
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

	case ',':
		addToken(keywords[","], ",");
		break;

	case '\'':
		addToken(keywords["'"], "'");
		break;

	case '/':
		addToken(keywords["/"], "/");
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

	case '^':
		addToken(keywords["^"], "^");
		break;

	case '\\':
		addToken(keywords["\\"], "\\");
		break;

	case '=':
		addToken(keywords["="], "=");
		break;
	case '(':
		addToken(keywords["("], "(");
		break;
	case ')':
		addToken(keywords[")"], ")");
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
		if (isDigit(c) && _newLine)
		{
			addLineToken();
		}
		else if (isDigit(c))
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

//Keyword Table
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
	keywords["^"] = TokenType::POWER;
	keywords[","] = TokenType::COMMA;
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
	keywords["goto"] = TokenType::GOTO;
	keywords["print"] = TokenType::PRINT;
	keywords["def"] = TokenType::DEF;
	keywords["for"] = TokenType::FOR;
	keywords["let"] = TokenType::LET;
	keywords["next"] = TokenType::NEXT;
	keywords["int"] = TokenType::INT;
	keywords["rnd"] = TokenType::RND;
	keywords["return"] = TokenType::RETURN;
	keywords["stop"] = TokenType::STOP;
	keywords["("] = TokenType::LEFTPAREN;
	keywords[")"] = TokenType::RIGHTPAREN;
	keywords["gosub"] = TokenType::GOSUB;
	keywords["input"] = TokenType::INPUT;
	keywords["read"] = TokenType::READ;
	keywords["negate"] = TokenType::NEGATE;
	keywords["sqr"] = TokenType::SQR;
	keywords["exp"] = TokenType::EXP;
	keywords["to"] = TokenType::TO;
}

char BasicScanner::advanceChar()
{
	return _sourceCode[_current++];
}

void BasicScanner::addToken(TokenType type, const string& data)
{
	_newLine = false;
	if (type == TokenType::NUMBERINTLITERAL || type == TokenType::NUMBERDBLLITERAL ||
		type == TokenType::STRINGLITERAL || 
		type == TokenType::IDENTIFIER)
	{
		Identifier identifierInstance;
		identifierInstance.name = data;
		identifierInstance.identifierType = type;
		switch (type) {
		case TokenType::IDENTIFIER:
			break;
		case TokenType::NUMBERDBLLITERAL:
			identifierInstance.valueDouble = stod(data);
		case TokenType::NUMBERINTLITERAL:
			identifierInstance.valueInt = stoi(data);
			break;
		default:
			identifierInstance.valueString = data;
			break;
		}
		(*_identifiers)[data] = identifierInstance;
	}

	_allTokens.push_back(BasicToken(type, data, _linenumber));
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
		string strliteral = _sourceCode.substr(_start + 1, _current - 1 - _start);
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
	string identifier =  _sourceCode.substr(_start, _current - _start);
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
	TokenType type = TokenType::NUMBERINTLITERAL;
	while (_current < _sourceCode.length() &&
		isDigit(_sourceCode[_current]))
	{
		_current++;
	}
	if (_sourceCode[_current] == '.' && _current < _sourceCode.length())
	{
		type = TokenType::NUMBERDBLLITERAL;
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
	string numliteral = _sourceCode.substr(_start, _current - _start);
	addToken(type, numliteral);
}

void BasicScanner::addLineToken()
{
	while (_current < _sourceCode.length() &&
		isDigit(_sourceCode[_current]))
	{
		_current++;
	}
	string linenumber = _sourceCode.substr(_start, _current - _start);
	addToken(TokenType::PROGRAMLINENUMBER, linenumber);
}
