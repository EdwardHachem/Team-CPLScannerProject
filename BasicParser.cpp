#include "BasicParser.h"
#include "Identifier.h"
#include <list>
#include <map>
#include <cassert>
using namespace std;

void BasicParser::init(list<BasicToken> *tokens, map<string, Identifier> *identifiers)
{
	_ptokens = tokens;
	_pidentifiers = identifiers;
	_tokenIt = _ptokens->begin();
}

void BasicParser::parse()
{
	// we parse one line as our top of the tree;

	parseLine();
}

/// <summary>
///		parses the following BNF
/// <Line> --> Number_Literal <Statements> NewLine <Line>
///				| Number_Literal <Statements> NewLine
/// </summary>
void BasicParser::parseLine()
{
	while (_tokenIt != _ptokens->end())
	{
		BasicToken token = *_tokenIt;
		// must be programline number
		if (token.type == TokenType::PROGRAMLINENUMBER) {
			int lineNumber = stoi(token.lexeme);
			// we'll need to put it as the P table so we can go to it for some conditions
			// save it in a lookup to the current Px index, the address of this line in the P array 
			ProgramLineNumberToPx[lineNumber] = Px;

			_tokenIt++; // advance to next token
			parseStatements();
		}
		else {
			_tokenIt++;
			error("error: expected basic line Number. line:", token.linenumber);
		}
	}
}

/// <summary>
///	parses BNF:
/// <Statements> --> <Statement> <Divider> <Statement>
///					| <Statement>
/// </summary>
void BasicParser::parseStatements()
{
	parseStatement();

	// now token iterator should be after statement.
	// if there's divider, continue parsing next statement
	BasicToken token = *_tokenIt;
	while ( (_tokenIt != _ptokens->end() && 
			(*_tokenIt).type != TokenType::EOL) &&
		token.lexeme.find_first_of(dividers) >= 0)
	{
		_tokenIt++;
		parseStatement();
	}
	if ((*_tokenIt).type == TokenType::EOL)
		_tokenIt++;
}

/// <summary>
/// <Statement> --> DATA <Constant List>
///				| DIM Identifier(<Number List>)
///				| END
///				| FOR Identifier = <Expression> TO <Expression>
///				| FOR Identifier = <Expression> TO <Expression> STEP <Constant>
///				| GOTO <Expression>
///				| GOSUB <Expression>
///				| IF <Expression> THEN Number_Literal
///				| INPUT <Identifier List>
///				| INPUT String <Divider> <Identifier List>
///				| LET Identifier = <Expression>
///				| NEXT <Identifier List>
///				| PRINT <Print list>
///				| READ <Identifier List>
///				| RETURN
///				| STOP
///				| <Function Expression>
///				| Blank_Statement
///				| Remark
/// </summary>
void BasicParser::parseStatement()
{
	BasicToken token = *_tokenIt;
	switch (token.type)
	{

	case TokenType::DIM:
		break;

	case TokenType::END:
		break;

	case TokenType::FOR:

		break;

	case TokenType::GOTO:
		break;

	case TokenType::GOSUB:
		break;

	case TokenType::IF:
	{	//IF <Expression> THEN Number_Literal
		// In P table, this is pushed in this order:
		//	Expr, 
		//	Number_Literal (the lineNumber to go to if true)
		//	BP (branches to the true address if result is true), 
		_tokenIt++;
		int exp1 = parseExpression();
		// pushed by the expression
		if ((*_tokenIt).type == TokenType::THEN)
		{
			_tokenIt++;
			int codeLineNumber = stoi((*_tokenIt).lexeme);
			pushP(CMD::LINENUMBER, codeLineNumber);
			pushP(CMD::BP, NULL);
		}
		else
		{
			error("Expected \"then\" ", ((*_tokenIt).linenumber));
		}
		break;
	}
	case TokenType::INPUT:
		break;

	case TokenType::LET:
	{
		///	 LET Identifier = <Expression>
		// pushed like this:
		// identifier, expression, assignOP
		// 
		string name; //identifer name
		int value;
		_tokenIt++;
		if ((*_tokenIt).type == TokenType::IDENTIFIER)
		{
			name = (*_tokenIt).lexeme;
			Identifier* pid = &(*_pidentifiers)[name];
			pushP(CMD::IDENTIFIER, pid);

			_tokenIt++;
			if ((*_tokenIt).type == TokenType::EQUAL)
			{
				_tokenIt++;
				value = parseExpression();
				// pushed by expression
				pushP(CMD::ASSIGN);
			}
		}
		else
		{
			error("Expected \"then\" ", ((*_tokenIt).linenumber));
		}
		break;
	}
	case TokenType::NEXT:
		break;

	case TokenType::PRINT:
		// push identifier pointer
		// push print cmd
		break;

	case TokenType::READ:
		break;

	case TokenType::RETURN:
		break;

	case TokenType::REM:
		// consume and ignore all tokens on the line
		while (_tokenIt != _ptokens->end() && (*_tokenIt).type!=TokenType::EOL)
			_tokenIt++;
		break;

	default:
		//Check function expression, otherwise error.
		break;
	}
}
/// <summary>
/// <Expression> --> <And Expression> OR <Expression>
///					| <And Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseExpression()
{
	BasicToken token = *_tokenIt;
	int left = parseAndExpression();
	if ((*_tokenIt).type == TokenType::OR)
	{
		_tokenIt++;
		int right = parseExpression();
		return left || right;
	}
	
	return left;
}

/// <summary>
/// <And Expression> --> <Not Expression> AND <And Expression>
///						| <Not Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseAndExpression()
{
	BasicToken token = *_tokenIt;
	int left = parseNotExpression();
	if ((*_tokenIt).type == TokenType::AND)
	{
		_tokenIt++;
		int right = parseAndExpression();
		return left && right;
	}
	return left;
}

/// <summary>
/// <Not Expression> --> NOT <Compare Expression>
///						| <Compare Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseNotExpression()
{
	BasicToken token = *_tokenIt;
	if ((*_tokenIt).type == TokenType::NOT)
	{
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::NOT);
	}
	else
	{
		parseCompareExpression();
	}
	return 0;
}

/// <summary>
/// <Compare Expression> --> <Addition Expression> = <Compare Expression>
///							| <Addition Expression> > <Compare Expression>
///							| <Addition Expression> >= <Compare Expression>
///							| <Addition Expression> <> <Compare Expression>
///							| <Addition Expression> < <Compare Expression>
///								| <Addition Expression> <= <Compare Expression>
///								| <Addition Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseCompareExpression()
{
	BasicToken token = *_tokenIt;
	parseAddExpression();
	switch ((*_tokenIt).type)
	{
	case TokenType::EQUAL:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::EQ);
		break;
	case TokenType::GREATERTHAN:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::GT);
		break;
	case TokenType::GREATERTHANOREQUALTO:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::GTE);
		break;
	case TokenType::NOTEQUAL:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::NE);
		break;
	case TokenType::LESSTHAN:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::LT);
		break;
	case TokenType::LESSTHANOREQUALTO:
		_tokenIt++;
		parseCompareExpression();
		pushP(CMD::LTE);
		break;
	default:
		break;
	}
	return 0;
}

/// <summary>
/// <Addition Expression> --> <Multiplication Expression> + <Addition Expression>
///							| <Multiplication Expression> -<Addition Expression>
///							| <Multiplication Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseAddExpression()
{
	BasicToken token = *_tokenIt;
	parseMultExpression();
	switch ((*_tokenIt).type)
	{
	case TokenType::ADD:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseAddExpression();
		pushP(CMD::PLUS);
		break;
	case TokenType::SUBTRACT:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseAddExpression();
		pushP(CMD::MINUS);
		break;
	default:
		break;
	}
	return 0;
}

/// <summary>
/// <Multiplication Expression> --> <Negate Expression> * <Multiplication Expression>
///								| <Negate Expression> / <Multiplication Expression>
///								| <Negate Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseMultExpression()
{
	BasicToken token = *_tokenIt;
	parseNegateExpression();
	// already pushed
	switch ((*_tokenIt).type)
	{
	case TokenType::MULTIPLY:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseMultExpression();
		// already pushed. push operator
		pushP(CMD::MULT);
		break;

	case TokenType::DIVIDE:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseMultExpression();
		// already pushed. push operator
		pushP(CMD::DIV);
		break;
	default:
		break;
	}
	return 0;
}

/// <summary>
/// <Negate Expression> --> - <Value>
///						| <Value>
/// </summary>
/// <returns></returns>
int BasicParser::parseNegateExpression()
{
	BasicToken token = *_tokenIt;
	if ((*_tokenIt).lexeme == "-")
	{
		//put negative lexeme into parse tree
		//Ptable sequence: Value, Not
		_tokenIt++;
		parseValue();
		pushP(CMD::CONST, -1);
		pushP(CMD::MULT);
	}
	else
	{
		parseValue();
	}

	return 0;
}


/// <summary>
/// <Value> --> ( <Expression> )
///				| Identifier
///				| <Constant>
///				| <Function Expression>
/// </summary>
/// <returns></returns>

int BasicParser::parseValue()
{
	BasicToken token = *_tokenIt;
	switch (token.type)
	{
	case TokenType::LEFTPAREN:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseExpression();
		//assert that there was a leftparen token type in parse tree before
		assert((*_tokenIt).type == TokenType::RIGHTPAREN);
		_tokenIt++;
		break;
	case TokenType::IDENTIFIER:
	case TokenType::NUMBERINTLITERAL:
	case TokenType::NUMBERDBLLITERAL:
	case TokenType::STRINGLITERAL:
	{
		// these are all stored in teh identifier table, including constants and strings
		// the interpreter will have the type info to figure it out
		_tokenIt++;
		Identifier *pIdentifier = &(*_pidentifiers)[token.lexeme];
		pushP(CMD::IDENTIFIER, pIdentifier);
		break;
	}
	default:
		break;
	}
	
	return 0;
}

void BasicParser::pushP(CMD cmd, Identifier* ptr)
{
	P[Px].cmd = cmd;
	P[Px].ptr = ptr;
	Px++;
}

void BasicParser::pushP(CMD cmd , int value)
{
	P[Px].cmd = cmd;
	P[Px].value = value;
	Px++;
}

