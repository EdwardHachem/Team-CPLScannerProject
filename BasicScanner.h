#pragma once
#include <iostream>
#include "BasicToken.h"
#include <map>
#include <list>
#include "Identifier.h"
using namespace std;

class BasicScanner
{
public:
	BasicScanner();
	BasicScanner(const string& sourceCode);
	list<BasicToken> scanTokens();
	void setSourceCode(map <string, Identifier> &identifiers, const string& sourceCode, int linenumber);
	void clear();
private:
	int _linenumber;
	void scanToken();
	size_t _start = 0;
	size_t _current = 0;
	list<BasicToken> _allTokens;
	string _sourceCode;
	map <string, TokenType> keywords;
	map <string, Identifier> * _identifiers;
	void initKeywords();
	char advanceChar();
	void addToken(TokenType type, const string& data);
	void addStringToken();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
	bool isDigit(char c);
	void addKeywordToken();
	void addNumberToken();
	void addLineToken();
};

