#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentToken(0)
{
}

parser::~parser()
{
}

void parser::parseProgram()
{
}

void parser::parseDeclaration()
{
}

void parser::parseStatement()
{
}

void parser::parseExpression()
{
}

void parser::parseType()
{
}

token parser::getCurrentToken()
{
	if (_currentToken <= _tokensStream.size()) {
		return _tokensStream[_currentToken];
	}
	return token("", "");
}

void parser::consumeToken()
{
}

bool parser::isBinaryOperator(token t)
{
	return false;
}
