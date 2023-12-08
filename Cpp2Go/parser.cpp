#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentPosition(0), _identifiersTypes({})
{
}

parser::~parser()
{
}

void parser::parseProgram()
{
	while (_currentPosition != _tokensStream.size())
	{
		token currToken = getCurrentToken();
		if (currToken.second.find("DATATYPE"))
		{
			parseDeclaration();
		}
		else if (currToken.second == IF_STATEMENT || currToken.second == WHILE_STATEMENT || currToken.second == FOR_STATEMENT)
		{
			parseStatement();
		}
		else
		{
			parseExpression();
		}

	}
}

void parser::parseDeclaration()
{
	std::string datatype = parseType();

	if (getCurrentToken().second == IDENTIFIER)
	{
		_identifiersTypes.insert(getCurrentToken().first, datatype );
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
	if (currToken.second == IF_STATEMENT)
	{
		consumeToken();
		parseIfStatment();
	}
	else if (currToken.second == WHILE_STATEMENT)
	{
		consumeToken();
		parseWhileStatement();
	}
	else if (currToken.second == FOR_STATEMENT)
	{
		consumeToken();
		parseForStatement();
	}
}

void parser::parseIfStatment()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseExpression(); //we expect an expression tp come

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") ////check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - if body
}

void parser::parseWhileStatement()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseExpression(); //we expect an expression tp come

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - while body
}

void parser::parseForStatement()
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseDeclaration(); // we expect decleration to come (for example: 'int i = 0;')

	parseExpression(); //we expect an expression tp come
	parseModifyOperator(); //we expect modify operator to come 

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - for body
}

void parser::parseExpression()
{
	if (isUnaryOperator(getCurrentToken()) || getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL))
	{
		consumeToken();
	}
	else if (getCurrentToken().second == "LEFT_PARENTHESIS")
	{
		consumeToken(); // Consume the '('
		parseExpression();
		if (getCurrentToken().second == "RIGHT_PARENTHESIS")
		{
			consumeToken(); // Consume the ')'
		}
		else
		{
			throw std::runtime_error("ERROR: expected a right parenthesis");
		}
	}
	else 
	{
		throw std::runtime_error("ERROR: invalid start of expression");
	}

	while (isBinaryOperator(getCurrentToken()))
	{
		std::string op = getCurrentToken().first;
		unconsumeToken();

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
		consumeToken();
		
		if (getCurrentToken().second != SEMICOLON && getCurrentToken().second != "RIGHT_PARENTHESIS")
		{
			throw std::runtime_error("ERROR: expected a semicolon or right parenthesis");
		}
	}
}

std::string parser::parseType()
{
	token currToken = getCurrentToken();
	if (currToken.second.find("DATATYPE"))
	{
		consumeToken();
		return currToken.second.substr(currToken.second.find("_") + 1);
	}
	else
	{
		throw std::runtime_error("ERROR: expected a datatype token");
	}
}

void parser::parseArithmeticOperator(const std::string& op)
{
	std::string datatype = getCurrentToken().second;
	if (getCurrentToken().second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(getCurrentToken().first)->second;
	}
	else if (!getCurrentToken().second.find(LITERAL))
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
	}
	consumeToken(2);
	if (!getCurrentToken().second.find(datatype))
	{
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}
}

void parser::parseRelationalOperator(const std::string& op)
{
	std::string datatype = getCurrentToken().second;
	if (getCurrentToken().second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(getCurrentToken().first)->second;
	}
	else if (!getCurrentToken().second.find(LITERAL))
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
	}
	consumeToken(2);
	if (!getCurrentToken().second.find(datatype))
	{
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}
}

void parser::parseLogicalOperator(const std::string& op)
{
	if (getCurrentToken().second != IDENTIFIER && !getCurrentToken().second.find(LITERAL))
	{
		throw std::runtime_error("ERROR: expected an identifier or literal before Logical Operator...");
	}
	consumeToken(2);
	if (getCurrentToken().second != IDENTIFIER && !getCurrentToken().second.find(LITERAL))
	{
		throw std::runtime_error("ERROR: expected an identifier or literal after Logical Operator...");
	}

}

void parser::parseBitwiseOperator(const std::string& op)
{
	if (getCurrentToken().second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand before bitwise operator.");
	}
	consumeToken(2);
	if (getCurrentToken().second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand after bitwise operator.");
	}
}

void parser::parseAssignmentOperator(const std::string& op)
{
	std::string datatype;
	if (getCurrentToken().second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(getCurrentToken().first)->second;
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier...");
	}
	consumeToken(2);
	if (!getCurrentToken().second.find(datatype))
	{
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}
}

void parser::parseAccessOperator(const std::string& op)
{
	if (getCurrentToken().second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier before Access Operator...");
	}
	consumeToken(2);
	if (getCurrentToken().second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier after Access Operator...");
	}
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
		throw std::runtime_error("excpected a modify operator");
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
	if (_currentPosition < _tokensStream.size())
	{
		++_currentPosition;
	}
}

void parser::consumeToken(size_t n)
{
	if ((_currentPosition + n) < _tokensStream.size())
	{
		_currentPosition += n;
	}
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

bool parser::isUnaryOperator(const token& t)
{
	const std::unordered_set<std::string> unaryOperators = { "+", "-", "!" /* Add other unary operators as needed */ };
	return (t.second.find("OPERATOR") && unaryOperators.find(t.first) != unaryOperators.end());
}