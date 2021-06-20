#pragma once
#include <iostream>
#include "BasicToken.h"
#include <map>
#include <list>
using namespace std;

class BasicScanner
{
public:
	BasicScanner();
	BasicScanner(const string& sourceCode); 
	list<BasicToken> scanTokens();
	void setSourceCode(const string& sourceCode);
	void clear();
private:
	void scanToken();
	size_t _start = 0;
	size_t _current = 0;
	list<BasicToken> _tokens;
	string _sourceCode;
	map <string, TokenType> keywords;
	void initKeywords();
	char advanceChar();
	void addToken(TokenType type, const string& data);
	void addStringToken();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
	bool isDigit(char c);
	void addKeywordToken();
	void addNumberToken();
};

