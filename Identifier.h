#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "BasicToken.h"
using namespace std;


class Identifier
{
public:
	string name;
	TokenType identifierType;
	double valueDouble;
	string valueString;
	friend ostream& operator<<(ostream& os, const Identifier& identifier);
private:
};

