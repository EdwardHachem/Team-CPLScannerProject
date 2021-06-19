#include "LanguageProcessor.h"
#include <string>

void LanguageProcessor::processLine(string code)
{
	// Get an instance of scanner and call get tokens.
}

void LanguageProcessor::processFile(string fileName)
{
	string line;
	ifstream testfile(fileName);
	if (testfile.is_open())
	{
		while (getline(testfile, line))
		{
			cout << line << '\n';
			processLine(line);
		}
		testfile.close();
	}
	else cout << "File not found.";
}