#include "BasicParser.h"
#include "Identifier.h"
#include <list>
#include <map>
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
/// 	<Line>        :: = Integer <Statements> 
/// </summary>
void BasicParser::parseLine()
{
	BasicToken token = *_tokenIt;
	// must be programline number
	if (token.type == TokenType::PROGRAMLINENUMBER) {
		int lineNumber = stoi(token.lexeme);
		// todo:
		// do something with the line number
		// we'll need to put it as the P table so we can go to it for some conditions
		_tokenIt++; // advance to next token
		parseStatements();
	}
	else {
		error("error: expected basic line Number. line:", token.linenumber);
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
	while (token.lexeme.find_first_of(dividers) >= 0)
	{
		_tokenIt++;
		parseStatement();
	}
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
		//IF <Expression> THEN Number_Literal
		_tokenIt++;
		int exp1 = parseExpression();
		if ((*_tokenIt).type == TokenType::THEN)
		{
			_tokenIt++;
			int gotoLinenumber = stoi((*_tokenIt).lexeme);
		}
		else
		{
			error("Expected \"then\" ", ((*_tokenIt).linenumber));
		}
		break;

	case TokenType::INPUT:
		break;

	case TokenType::LET:
		break;

	case TokenType::NEXT:
		break;

	case TokenType::PRINT:
		break;

	case TokenType::READ:
		break;

	case TokenType::RETURN:
		break;

	case TokenType::REM:
		break;

	default:
		//Check function expression, otherwise error.
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
	parseAndExpression();
	if ((*_tokenIt).type == TokenType::OR)
	{
		_tokenIt++;
		parseExpression();
	}
	return 0;
}

/// <summary>
/// <And Expression> --> <Not Expression> AND <And Expression>
///						| <Not Expression>
/// </summary>
/// <returns></returns>
int BasicParser::parseAndExpression()
{
	BasicToken token = *_tokenIt;
	parseNotExpression();
	if ((*_tokenIt).type == TokenType::AND)
	{
		_tokenIt++;
		parseAndExpression();
	}
	return 0;
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
	case TokenType::GREATERTHAN:
	case TokenType::GREATERTHANOREQUALTO:
	case TokenType::NOTEQUAL:
	case TokenType::LESSTHAN:
	case TokenType::LESSTHANOREQUALTO:
		_tokenIt++;
		parseCompareExpression();
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
	case TokenType::SUBTRACT:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseAddExpression();
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
	switch ((*_tokenIt).type)
	{
	case TokenType::MULTIPLY:
	case TokenType::DIVIDE:
		//put the (*_tokenIt).type in parse tree
		_tokenIt++;
		parseMultExpression();
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
		_tokenIt++;
		parseValue();
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
		break;
	case TokenType::RIGHTPAREN:
		//put the (*_tokenIt).type in parse tree
		//assert that there was a leftparen token type in parse tree before
		//otherwise throw an error
		//unsure what to do after a right paren, do we parse the next token at the expression level?	
		break;
	case TokenType::IDENTIFIER:
		_tokenIt++;
		//save name of identifier here
		string identifier = token.lexeme;
		parseValue();
		break;
	case TokenType::NUMBERLITERAL:

		break;
	case TokenType::STRINGLITERAL:
		break;
	default:
		break;
	}

	
	return 0;
}

