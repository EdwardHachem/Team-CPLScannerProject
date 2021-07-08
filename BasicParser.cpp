#include "BasicParser.h"

BasicParser::BasicParser(list<BasicToken> tokens)
{
	_tokens = tokens;
}

void BasicParser::Parse()
{
	list<BasicToken>::iterator tokenIt;

	// parse each token
	for (tokenIt = _tokens.begin(); tokenIt != _tokens.end(); tokenIt++) 
	{
		switch (tokenIt->_type)
		{
		default:
			break;
		}
	}
}
