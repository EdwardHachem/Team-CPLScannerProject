#pragma once
#include <iostream>
#include "BasicToken.h"
#include <map>
#include <list>
using namespace std;

class BasicScanner
{
public:
	BasicScanner(string sourceCode); 
	list<BasicToken> scanTokens();
	list<BasicToken> getTokens();

private:
	void scanToken();
	size_t _start = 0;
	size_t _current = 0;
	list<BasicToken> _tokens;
	string _sourceCode;
	map <string, TokenType> keywords;
	void initKeywords();
	char advanceChar();
	void addToken(TokenType type, string data);
	void addStringToken();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
	bool isDigit(char c);
	void addKeywordToken();
	void addNumberToken();
};

