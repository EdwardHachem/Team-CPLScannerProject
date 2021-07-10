#include "LanguageProcessor.h"
#include <string>
#include <ostream>
#include "BasicParser.h"
using namespace std;

void LanguageProcessor::processLine(string code, int linenumber)
{
	// Get an instance of scanner and call get tokens.
	scanner.clear();
	scanner.setSourceCode(identifiers, code, linenumber);
	//  scan this line into tokens
	list<BasicToken> tokens = scanner.scanTokens();
	// append the new line token list into our master token list
	_tokens.splice(_tokens.end(), tokens);
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
			processLine(line, ++linenumber); // this builds our _tokens list
		}
		testfile.close();

		printTokens();
		
		BasicParser parser;
		parser.init(&_tokens, &identifiers);
		parser.parse();
	}
	else cout << "File not found.";
}

void LanguageProcessor::printTokens()
{
	list<BasicToken>::iterator tokenIt;
	for (tokenIt = _tokens.begin(); tokenIt != _tokens.end(); ++tokenIt)
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
