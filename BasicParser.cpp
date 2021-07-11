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

			advanceToken(); // advance to next token
			parseStatements();
		}
		else {
			advanceToken();
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
	while ( (_tokenIt != _ptokens->end() && 
			(*_tokenIt).type != TokenType::EOL) &&
		(*_tokenIt).lexeme.find_first_of(dividers) >= 0)
	{
		advanceToken();
		parseStatement();
	}
	if ((_tokenIt != _ptokens->end() && (*_tokenIt).type == TokenType::EOL))
		advanceToken();
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
		while (_tokenIt != _ptokens->end())
			advanceToken();
		break;

	case TokenType::FOR:

		break;

	case TokenType::GOTO:
	{
		//push line number
		advanceToken();
		int codeLineNumber = stoi((*_tokenIt).lexeme);
		pushP(CMD::LINENUMBER, codeLineNumber);
		//push branch
		pushP(CMD::BR);
		break;
	}

	case TokenType::GOSUB:
		break;

	case TokenType::IF:
	{	//IF <Expression> THEN Number_Literal
		// In P table, this is pushed in this order:
		//	Expr, 
		//	Number_Literal (the lineNumber to go to if true)
		//	BP (branches to the true address if result is true), 
		advanceToken();
		int exp1 = parseExpression();
		// pushed by the expression
		if ((*_tokenIt).type == TokenType::THEN)
		{
			advanceToken();
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

	//Identifier List
	case TokenType::INPUT:
		advanceToken();
		parseValue();
		
		//push input
		pushP(CMD::INPUT);
		break;

	case TokenType::LET:
	{
		///	 LET Identifier = <Expression>
		// pushed like this:
		// identifier, expression, assignOP
		// 
		string name; //identifer name
		int value;
		advanceToken();
		if ((*_tokenIt).type == TokenType::IDENTIFIER)
		{
			name = (*_tokenIt).lexeme;
			Identifier* pid = &(*_pidentifiers)[name];
			pushP(CMD::IDENTIFIER, pid);

			advanceToken();
			if ((*_tokenIt).type == TokenType::EQUAL)
			{
				advanceToken();
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
		
		advanceToken();
		parsePrintList();
		// push print cmd
		pushP(CMD::PRINT);
		break;

	case TokenType::READ:
		break;

	case TokenType::RETURN:
		break;

	case TokenType::REM:
		// consume and ignore all tokens on the line
		while (_tokenIt != _ptokens->end() && (*_tokenIt).type!=TokenType::EOL)
			advanceToken();
		break;


		//DEF FunctionID '('')' '=' < Expression > !The ID must start with FN
	case TokenType::RND:
	case TokenType::SQR:
	case TokenType::EXP:
		parseFunctionExpression();

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
		advanceToken();
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
		advanceToken();
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
		advanceToken();
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
		advanceToken();
		parseCompareExpression();
		pushP(CMD::EQ);
		break;
	case TokenType::GREATERTHAN:
		advanceToken();
		parseCompareExpression();
		pushP(CMD::GT);
		break;
	case TokenType::GREATERTHANOREQUALTO:
		advanceToken();
		parseCompareExpression();
		pushP(CMD::GTE);
		break;
	case TokenType::NOTEQUAL:
		advanceToken();
		parseCompareExpression();
		pushP(CMD::NE);
		break;
	case TokenType::LESSTHAN:
		advanceToken();
		parseCompareExpression();
		pushP(CMD::LT);
		break;
	case TokenType::LESSTHANOREQUALTO:
		advanceToken();
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
		advanceToken();
		parseAddExpression();
		pushP(CMD::PLUS);
		break;
	case TokenType::SUBTRACT:
		//put the (*_tokenIt).type in parse tree
		advanceToken();
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
		advanceToken();
		parseMultExpression();
		// already pushed. push operator
		pushP(CMD::MULT);
		break;

	case TokenType::DIVIDE:
		//put the (*_tokenIt).type in parse tree
		advanceToken();
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
		advanceToken();
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
/// 		<Function Expression> -- > ABS(<Addition Expression>)
///			| ATN(<Addition Expression>)
///			| COS(<Addition Expression>)
///			| EXP(<Addition Expression>)
///			| INT(<Addition Expression>)
///			| LOG(<Addition Expression>)
///			| RND(Number_Literal)
///			| SIN(<Addition Expression>)
///			| SQR(<Addition Expression>)
///			| TAN(<Addition Expression>)
/// </summary>
/// <returns></returns>
int BasicParser::parseFunctionExpression()
{
	BasicToken token = *_tokenIt;
	CMD cmdType = CMD::NOP;
	switch (token.type)
	{
	case TokenType::INT:
		cmdType = CMD::INT;
		break;
	case TokenType::RND:
		cmdType = CMD::RND;

		break;
	case TokenType::SQR:
		cmdType = CMD::SQR;
		break;
	case TokenType::EXP:
		cmdType = CMD::EXP;		
		break;
	default:
		break;
	}

	advanceToken();
	if ((*_tokenIt).type == TokenType::LEFTPAREN)
	{
		advanceToken();
		if (cmdType == CMD::RND)
		{
			parseValue();
		}
		{
			parseAddExpression();
		}
		pushP(cmdType);
		if ((*_tokenIt).type != TokenType::RIGHTPAREN)
		{
			error("Expected right parenthesis", (*_tokenIt).linenumber);
		}
		advanceToken();
	}
	else
	{
		error("Expected left parenthesis", (*_tokenIt).linenumber);
	}
	
	return 0;
}

/// <summary>
/// <Print List> --> <Expression> <Divider> <Print List>
///					| <Expression>
///					| Blank_Statement
/// </summary>
/// <returns></returns>
int BasicParser::parsePrintList()
{
	BasicToken token = *_tokenIt;
	if (token.type != TokenType::EOL)
	{
		parseExpression();

		token = *_tokenIt;
		int i = token.lexeme.find_first_of(dividers);
		if (i >= 0)
		{
			parsePrintList();
		}
	}
	else
	{
		//Push Blank_Statement 
		map<string, Identifier>::iterator blankIter = _pidentifiers->find("");
		if (blankIter == _pidentifiers->end())
		{
			Identifier blankIdent;
			blankIdent.name = "";
			blankIdent.identifierType = TokenType::STRINGLITERAL;
			blankIdent.valueString = "";
			(*_pidentifiers)[""] = blankIdent;
		}
		pushP(CMD::STRING, &(*_pidentifiers)[""]);
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
		advanceToken();
		parseExpression();
		//assert that there was a leftparen token type in parse tree before
		assert((*_tokenIt).type == TokenType::RIGHTPAREN);
		advanceToken();
		break;
	case TokenType::IDENTIFIER:
	case TokenType::NUMBERINTLITERAL:
	case TokenType::NUMBERDBLLITERAL:
	case TokenType::STRINGLITERAL:
	{
		// these are all stored in the identifier table, including constants and strings
		// the interpreter will have the type info to figure it out
		advanceToken();
		Identifier *pIdentifier = &(*_pidentifiers)[token.lexeme];
		pushP(CMD::IDENTIFIER, pIdentifier);
		break;
	}
	case TokenType::RND:
	case TokenType::INT:
	case TokenType::SQR:
	case TokenType::EXP:
	// todo: add rest of functions here
		parseFunctionExpression();
		break;
	default:
		break;
	}
	
	return 0;
}

/// <summary>
///<Identifier List> --> Identifier <Divider> <Identifier List>
///						| Identifier
/// </summary>
/// <returns></returns>
int BasicParser::parseIdentifierList()
{
	parseValue();
	BasicToken token = *_tokenIt;
	if (token.lexeme.find_first_of(dividers) >= 0)
	{
		advanceToken();
		parseIdentifierList();
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

bool BasicParser::advanceToken() {
	if (_tokenIt != _ptokens->end())
	{
		_tokenIt++;
		return true;
	}
	return false;
}