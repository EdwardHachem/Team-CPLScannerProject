#pragma once
#include <iostream>
#include <fstream>
#include "BasicScanner.h"
using namespace  std;

class LanguageProcessor
{
public:
	LanguageProcessor() {};
	void processLine(string code);
	void processFile(string fileName);
private:
	BasicScanner scanner;
};

