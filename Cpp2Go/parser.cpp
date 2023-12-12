#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentPosition(0), _identifiersTypes({})
{
	parseProgram();
}

parser::~parser()
{
}

ASTNode* parser::parseProgram()
{
	ASTNode* programNode = new ASTNode("PROGRAM");
	while (_currentPosition < _tokensStream.size())
	{
		token currToken = getCurrentToken();
		if (currToken.second.find("DATATYPE") != std::string::npos)
		{
			parseDeclaration(programNode);
		}
		else if (currToken.second == IF_STATEMENT || currToken.second == WHILE_STATEMENT || currToken.second == FOR_STATEMENT)
		{
			parseStatement(programNode);
		}
		else
		{
			parseExpression(programNode);
		}

	}
	return programNode;
}

void parser::parseDeclaration(ASTNode* head)
{
	std::string datatype;
	parseType(datatype, head);

	if (getCurrentToken().second == IDENTIFIER)
	{
		_identifiersTypes.emplace(getCurrentToken().first, datatype);
		consumeToken();
	}
	else
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}
	if (getCurrentToken().second == ASSIGNMENT_OPERATOR)
	{
		unconsumeToken();
		parseExpression(head);
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

void parser::parseStatement(ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second == IF_STATEMENT)
	{
		consumeToken();
		parseIfStatment(head);
	}
	else if (currToken.second == WHILE_STATEMENT)
	{
		consumeToken();
		parseWhileStatement(head);
	}
	else if (currToken.second == FOR_STATEMENT)
	{
		consumeToken();
		parseForStatement(head);
	}
}

void parser::parseIfStatment(ASTNode* head)
{
	//create an if statement node
	ASTNode* ifStatementNode = new ASTNode("ifStatement");
	head->addChild(ifStatementNode);
	if (getCurrentToken().second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseExpression(ifStatementNode); //we expect an expression to come

	if (getCurrentToken().second != RIGHT_PARENTHESIS) //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - if body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
}

void parser::parseWhileStatement(ASTNode* head)
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseExpression(head); //we expect an expression tp come

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - while body
}

void parser::parseForStatement(ASTNode* head)
{
	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseDeclaration(head); // we expect decleration to come (for example: 'int i = 0;')

	parseExpression(head); //we expect an expression tp come
	parseModifyOperator(); //we expect modify operator to come 

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	//check - for body
}

void parser::parseExpression(ASTNode* head)
{
	if (isUnaryOperator(getCurrentToken()) || getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL))
	{
		consumeToken();
	}
	else if (getCurrentToken().second == "LEFT_PARENTHESIS")
	{
		consumeToken(); // Consume the '('
		parseExpression(head);
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
		// create op node and assign it to the head node
		ASTNode* opNode = new ASTNode(getCurrentToken().second, getCurrentToken().first);

		if (LogicalOperators.find(op) != LogicalOperators.end()) //if the current node is logical operator add up the last one as a child of the current node
		{
			parseLogicalOperator(op, opNode);
			opNode->addChild(head->children.back());
			head->children.pop_back();
			head->addChild(opNode);
		}
		else if (LogicalOperators.find(head->children.back()->value) != LogicalOperators.end()) // if the last node is logical operator add up the current one as a child of the last node
		{
			head->children.back()->addChild(opNode);
		}

		unconsumeToken();

		if (ArithmeticOperators.find(op) != ArithmeticOperators.end())
		{
			parseArithmeticOperator(op, opNode);
		}
		else if (RelationalOperators.find(op) != RelationalOperators.end())
		{
			parseRelationalOperator(op, opNode);
		}
		else if (BitwiseOperators.find(op) != BitwiseOperators.end())
		{
			parseBitwiseOperator(op, opNode);
		}
		else if (AssignmentOperators.find(op) != AssignmentOperators.end())
		{
			parseAssignmentOperator(op, opNode);
		}
		else if (AccessOperators.find(op) != AccessOperators.end())
		{
			parseAccessOperator(op, opNode);
		}
		consumeToken();
		
		if (getCurrentToken().second != SEMICOLON && getCurrentToken().second != "RIGHT_PARENTHESIS")
		{
			throw std::runtime_error("ERROR: expected a semicolon or right parenthesis");
		}
	}
}

void parser::parseType(std::string& datatype, ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second.find("DATATYPE") != std::string::npos)
	{
		consumeToken();
		datatype = currToken.second.substr(currToken.second.find("_") + 1);
		//create ast node
		ASTNode* dataTypeNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(dataTypeNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected a datatype token");
	}
}

void parser::parseArithmeticOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	std::string datatype = currToken.second;
	if (currToken.second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(currToken.first)->second;
		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(identifierNode);
	}
	else if (getCurrentToken().second.find(LITERAL) != std::string::npos)
	{
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(literalNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
	}
	consumeToken(2);
	currToken = getCurrentToken();

	if (currToken.second.find(datatype) == std::string::npos)
	{
		if (currToken.second == IDENTIFIER)
		{
			if (datatype == _identifiersTypes.find(currToken.first)->second)
			{
				// create idetifier node and add it to the head node
				ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
				head->addChild(identifierNode);
				return;
			}
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}

	ASTNode* literalNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(literalNode);
}

void parser::parseRelationalOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	std::string datatype = currToken.second;
	if (currToken.second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(currToken.first)->second;
		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(identifierNode);
	}
	else if (getCurrentToken().second.find(LITERAL) != std::string::npos)
	{
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(literalNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
	}
	consumeToken(2);
	currToken = getCurrentToken();

	if (currToken.second.find(datatype) == std::string::npos)
	{
		if (currToken.second == IDENTIFIER)
		{
			if (datatype == _identifiersTypes.find(currToken.first)->second)
			{
				// create idetifier node and add it to the head node
				ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
				head->addChild(identifierNode);
				return;
			}
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}

	ASTNode* literalNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(literalNode);
}

void parser::parseLogicalOperator(const std::string& op, ASTNode* head)
{
	if (getCurrentToken().second != IDENTIFIER && getCurrentToken().second.find(LITERAL) == std::string::npos)
	{
		throw std::runtime_error("ERROR: expected an identifier or literal before Logical Operator...");
	}
	consumeToken(2);
	if (getCurrentToken().second != IDENTIFIER && getCurrentToken().second.find(LITERAL) == std::string::npos)
	{
		throw std::runtime_error("ERROR: expected an identifier or literal after Logical Operator...");
	}

}

void parser::parseBitwiseOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand before bitwise operator.");
	}
	else
	{
		ASTNode* intLiteralNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(intLiteralNode);
	}
	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand after bitwise operator.");
	}
	else
	{
		ASTNode* intLiteralNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(intLiteralNode);
	}
}

void parser::parseAssignmentOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	std::string datatype = currToken.second;
	if (currToken.second == IDENTIFIER)
	{
		datatype = _identifiersTypes.find(currToken.first)->second;
		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(identifierNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier...");
	}
	consumeToken(2);
	currToken = getCurrentToken();

	if (currToken.second.find(datatype) == std::string::npos)
	{
		if (currToken.second == IDENTIFIER)
		{
			if (datatype == _identifiersTypes.find(currToken.first)->second)
			{
				// create idetifier node and add it to the head node
				ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
				head->addChild(identifierNode);
				return;
			}
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}

	ASTNode* literalNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(literalNode);
}

void parser::parseAccessOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier before Access Operator...");
	}
	else
	{
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(identifierNode);
	}
	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier after Access Operator...");
	}
	else
	{
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		head->addChild(identifierNode);
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
	if (_currentPosition < _tokensStream.size()) {
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
		"&&", "||", "!"  // Logical Operators
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