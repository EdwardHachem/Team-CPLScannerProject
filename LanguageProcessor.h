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
private:
	map <string, Identifier> identifiers;
	BasicScanner scanner;
};

