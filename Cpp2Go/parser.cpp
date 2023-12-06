#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentPosition(0)
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
	parseType();

	if (getCurrentToken().second == IDENTIFIER)
	{
		consumeToken();
	}
	else
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}
	if (getCurrentToken().second == ASSIGNMENT_OPERATOR)
	{
		unconsumeToken();
		parseExpression();
	}
	else
	{
		if (getCurrentToken().second == SEMICOLON)
		{
			consumeToken();
		}
		else
		{
			throw std::runtime_error("ERROR: expecting an semicolon token...");
		}
	}
}

void parser::parseStatement()
{
	token currToken = getCurrentToken();
	if (currToken.second == "IF_STATEMENT")
	{
		consumeToken();
		parseIfStatment();
	}
	else if (currToken.second == "WHILE_STATEMENT")
	{
		consumeToken();
		parseWhileStatement();
	}
	else if (currToken.second == "FOR_STATEMENT")
	{
		consumeToken();
		parseForStatement();
	}
}

void parser::parseIfStatment()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw "excepcted LEFT_PARENTHESIS";

	consumeToken();
	parseExpression(); //we expect an expression tp come

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") ////check that the token is ')'
		throw "excepcted RIGHT_PARENTHESIS";
	consumeToken();
	//check - if body
}

void parser::parseWhileStatement()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw "excepcted LEFT_PARENTHESIS";

	consumeToken();
	parseExpression(); //we expect an expression tp come

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw "excepcted RIGHT_PARENTHESIS";
	consumeToken();
	//check - while body
}

void parser::parseForStatement()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw "excepcted LEFT_PARENTHESIS";

	consumeToken();
	parseDeclaration(); // we expect decleration to come (for example: 'int i = 0;')

	parseExpression(); //we expect an expression tp come
	parseModifyOperator(); //we expect modify operator to come 

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw "excepcted RIGHT_PARENTHESIS";
	consumeToken();
	//check - for body
}

void parser::parseExpression()
{
	if (getCurrentToken().second == IDENTIFIER)
	{
		consumeToken();
	}
	else
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}
	while (isBinaryOperator(getCurrentToken()))
	{
		std::string op = getCurrentToken().first;
		consumeToken();

		if (ArithmeticOperators.find(op) != ArithmeticOperators.end())
		{
			parseArithmeticOperator(op);
		}
		else if (RelationalOperators.find(op) != RelationalOperators.end())
		{
			parseRelationalOperator(op);
		}
		else if (LogicalOperators.find(op) != LogicalOperators.end())
		{
			parseLogicalOperator(op);
		}
		else if (BitwiseOperators.find(op) != BitwiseOperators.end())
		{
			parseBitwiseOperator(op);
		}
		else if (AssignmentOperators.find(op) != AssignmentOperators.end())
		{
			parseAssignmentOperator(op);
		}
		else if (AccessOperators.find(op) != AccessOperators.end())
		{
			parseAccessOperator(op);
		}
	}
}

void parser::parseType()
{
	token currToken = getCurrentToken();
	if (currToken.second == "DATATYPE")
		consumeToken();
	else
	{
		throw "error, expected a datatype token";
	}
}

token parser::getCurrentToken()
{
	if (_currentPosition <= _tokensStream.size()) {
		return _tokensStream[_currentPosition];
	}
	return token("", "");
}

void parser::consumeToken()
{
	++_currentPosition;
}

void parser::unconsumeToken()
{
	if (_currentPosition > 0)
	{
		--_currentPosition;
	}
}

bool parser::isBinaryOperator(token t)
{
	//here we define a list of all binary operators in cpp
	std::list<std::string> listOfBinaryOperators =
	{
		"+", "-", "*", "/", "%",    // Arithmetic Operators
		"==", "!=", "<", ">", "<=", ">=",    // Relational Operators
		"&&", "||",    // Logical Operators
		"&", "|", "^", "<<", ">>",    // Bitwise Operators
		"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=",    // Assignment Operators
		",", "->", ".", "[]",    // Access Operators
		"++", "--", // Modify Operators 
	};
	//define an iterator that going over the list and check if the operator is binary or not
	std::list<std::string>::iterator iter = std::find(listOfBinaryOperators.begin(), listOfBinaryOperators.end(), t.first);
	//if the iterator isn't getting to the last node int he list, it means the operator is binary, and we will return true
	if (iter != listOfBinaryOperators.end())
		return true;
	//else return false
	return false;
}

void parser::parseArithmeticOperator(const std::string& op)
{
	if (getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL))
	{

	}
}

void parser::parseRelationalOperator(const std::string& op)
{
	if (getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL))
	{

	}
}

void parser::parseLogicalOperator(const std::string& op)
{
	// Parsing logic for logical operators
}

void parser::parseBitwiseOperator(const std::string& op)
{
	// Parsing logic for bitwise operators
}

void parser::parseAssignmentOperator(const std::string& op)
{
	if (getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL))
	{

	}
}

void parser::parseAccessOperator(const std::string& op) {
	// Parsing logic for access operators
}

void parser::parseModifyOperator()
{
	if (getCurrentToken().second == "IDENTIFIER")
	{
		consumeToken();
		if (getCurrentToken().first == "++" || getCurrentToken().first == "--")
		{
			consumeToken();
		}
	}
	else
	{
		throw "excpected a modify operator"; 
	}
}
