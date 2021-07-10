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

}
