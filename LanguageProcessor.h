#pragma once
#include <iostream>
#include <fstream>
#include "BasicScanner.h"
#include "Identifier.h"
using namespace  std;


class LanguageProcessor
{
public:
	LanguageProcessor() {};
	void processLine(string code, int linenumber);
	void processFile(string fileName);
	void printTokens();
private:
	map <string, Identifier> identifiers;
	list<BasicToken> _tokens;
	BasicScanner scanner;
};

