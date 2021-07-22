#include "Interpreter.h"
using namespace std;

void Interpreter::init(P_struct* pTable, int count, map<string, Identifier>* pidentifiers, map<int, int>* plinenumberToPx)
{
	_pTable = pTable;
	_count = count;
	_pidentifiers = pidentifiers;
	_plinenumberToPx = plinenumberToPx;
}

void Interpreter::interpret()
{
	P_struct item1;
	for (_currentPx = 0; _currentPx <= _count; _currentPx++)
	{
		int val1 = 0, val2 = 0;
		double dval1, dval2;
		bool dblValue1 = false;
		bool dblValue2 = false;
		P_struct entry = _pTable[_currentPx];
		switch (entry.cmd)
		{
		case CMD::IDENTIFIER:
		case CMD::CONST:
		case CMD::STRING:
		{
			S.push(entry);
			break;
		}

		case CMD::PLUS:
		{
			getItem(dblValue1, dval1, val1);
			getItem(dblValue2, dval2, val2);
			//if (dblValue1 && dblValue2) //Checks for doubles. I didn't include this in the later tests as the examples don't use doubles.
			//{
			//	item1.dblValue = dval1 + dval2;
			//	item1.cmd = CMD::STACKDOUBLE;
			//	S.push(item1);
			//}
			//else if (dblValue1 && !dblValue2) {
			//	item1.dblValue = dval1 + val2;
			//	item1.cmd = CMD::STACKDOUBLE;
			//	S.push(item1);
			//}
			//else if (!dblValue1 && dblValue2) {
			//	item1.dblValue = val1 + dval2;
			//	item1.cmd = CMD::STACKDOUBLE;
			//	S.push(item1);
			//}
			//else if (!dblValue1 && !dblValue2) {
			string name = getUniqueName();
			item1.ptr = createIdentifier(name, val1 + val2);
			item1.cmd = CMD::IDENTIFIER;
			S.push(item1);
		//}
			break;
		}

		case CMD::MULT:
		{
			getItem(dblValue1, dval1, val1);
			getItem(dblValue2, dval2, val2);
			string name = getUniqueName();
			item1.ptr = createIdentifier(name, val1 * val2);
			item1.cmd = CMD::IDENTIFIER;
			S.push(item1);
			break;
		}

		case CMD::DIV:
		{
			getItem(dblValue1, dval1, val1);
			getItem(dblValue2, dval2, val2);
			string name = getUniqueName();
			item1.ptr = createIdentifier(name, val2 / val1);
			item1.cmd = CMD::IDENTIFIER;
			S.push(item1);
			break;
		}

		case CMD::INT:
		{
			getItem(dblValue1, dval1, val1);
			// convert to int and then push.
			string name = getUniqueName();
			item1.ptr = createIdentifier(name, val1);
			item1.cmd = CMD::IDENTIFIER;
			S.push(item1);
			break;
		}

		case CMD::MINUS:
		{
			getItem(dblValue1, dval1, val1);
			getItem(dblValue2, dval2, val2);
			string name = getUniqueName();
			item1.ptr = createIdentifier(name, val2 - val1);
			item1.cmd = CMD::IDENTIFIER;
			S.push(item1);
			break;
		}

		case CMD::PRINT:
			getItem(dblValue1, dval1, val1);
			cout << val1 << endl;
			break;

		case CMD::ASSIGN:
		{
			Identifier *pid1, *pid2;

			pid1 = getItem(dblValue1, dval1, val1);
			pid2 = getItem(dblValue2, dval2, val2);
			switch (pid1->identifierType)
			{
			case TokenType::NUMBERDBLLITERAL:
				pid2->valueDouble = pid1->valueDouble;
				break;

			case TokenType::NUMBERINTLITERAL:
				pid2->valueInt = pid1->valueInt;
				break;

			case TokenType::STRINGLITERAL:
			case TokenType::STRINGIDENTIFIER:
				pid2->identifierType = TokenType::STRINGIDENTIFIER;
				pid2->valueString = pid1->valueString;
				break;
			default:
				break;
			}
		}
		}
	}
}

Identifier* Interpreter::createIdentifier(const string &name, int value)
{
	Identifier identifier;
	identifier.identifierType = TokenType::NUMBERINTLITERAL;
	identifier.name = name;
	identifier.valueInt = value;
	(*_pidentifiers)[name] = identifier;
	return &(*_pidentifiers)[name];
}


Identifier *Interpreter::getItem(bool& dblValue1, double& dval1, int& val1)
{
	P_struct item1 = S.top();
	S.pop();
	switch (item1.cmd)
	{
	case CMD::IDENTIFIER:
	{
		if (item1.ptr->identifierType == TokenType::DOUBLE)
		{
			dblValue1 = true;
			dval1 = item1.ptr->valueDouble;
		}
		else
		{
			val1 = item1.ptr->valueInt;
		}
		break;
	}
	default:
		return nullptr;
		break;
	}
	return item1.ptr;

}

string Interpreter::getUniqueName()
{

	return string(_unique + to_string(_uniqueint++));
}

