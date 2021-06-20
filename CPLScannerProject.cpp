#include <iostream>
#include <fstream>
#include <string>
#include "LanguageProcessor.h"
using namespace std;

int main()
{	
	LanguageProcessor languageProcessor;
	languageProcessor.processFile("basicCode.txt");
	
	return 0;
}


