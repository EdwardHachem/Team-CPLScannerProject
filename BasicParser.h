#pragma once
#include <list>
#include <stack>
#include "BasicToken.h"

class BasicParser
{
public:
	BasicParser(list<BasicToken> tokens);
	void Parse();
private:
	list<BasicToken> _tokens;
	stack<int> _stack;

};

