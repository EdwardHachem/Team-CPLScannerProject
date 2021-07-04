#include "LanguageProcessor.h"
#include <string>
#include <ostream>
using namespace std;

void LanguageProcessor::processLine(string code, int linenumber)
{
	// Get an instance of scanner and call get tokens.
	scanner.clear();
	scanner.setSourceCode(identifiers, code, linenumber);
	list<BasicToken> tokens = scanner.scanTokens();
	list<BasicToken>::iterator tokenIt;
	for (tokenIt = tokens.begin(); tokenIt != tokens.end(); ++tokenIt)
	{
		tokenIt->printToken();	
	}
	cout << "Identifier Table" << endl;
	map <string, Identifier>::iterator identifierIt;
	for (identifierIt = identifiers.begin(); identifierIt != identifiers.end(); ++identifierIt)
	{
		cout << identifierIt->second << endl;
	}
}

void LanguageProcessor::processFile(string fileName)
{
	int linenumber = 0;
	string line;
	ifstream testfile(fileName);
	if (testfile.is_open())
	{
		while (getline(testfile, line))
		{
			cout << line << '\n';
			processLine(line, ++linenumber);

		}
		testfile.close();
	}
	else cout << "File not found.";
}