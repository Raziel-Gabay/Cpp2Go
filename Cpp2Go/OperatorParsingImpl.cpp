#include "parser.h"

void parser::parseExpression(ASTNode* head)
{
	if (!isUnaryOperator(getCurrentToken()) && getCurrentToken().second != IDENTIFIER && getCurrentToken().second.find(LITERAL) == std::string::npos)
	{
		throw std::runtime_error("ERROR: invalid start of expression");
	}
	consumeToken();

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
		else if (!head->children.empty())
		{
			if (LogicalOperators.find(head->children.back()->value) != LogicalOperators.end())
			{
				head->children.back()->addChild(opNode);
			}
			else
			{
				head->addChild(opNode);
			}
		}
		else
		{
			head->addChild(opNode);
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

		if (getCurrentToken().second != SEMICOLON && getCurrentToken().second != "RIGHT_PARENTHESIS" && !isBinaryOperator(getCurrentToken()))
		{
			throw std::runtime_error("ERROR: expected a semicolon or right parenthesis");
		}
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
	ASTNode* intLiteralNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(intLiteralNode);
	
	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand after bitwise operator.");
	}	
	intLiteralNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(intLiteralNode);
	
}

void parser::parseAssignmentOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	std::string datatype = currToken.second;
	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier...");
	}
	datatype = _identifiersTypes.find(currToken.first)->second;
	// create idetifier node and add it to the head node
	ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(identifierNode);

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
		else if (currToken.second == ADDRESS_OF_OPERATOR)
		{
			ASTNode* addressNode = new ASTNode(currToken.second, currToken.first);
			head->addChild(addressNode);
			consumeToken();
			currToken = getCurrentToken();
			if (currToken.second == IDENTIFIER)
			{
				if (datatype == _identifiersTypes.find(currToken.first)->second)
				{
					// create idetifier node and add it to the head node
					ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
					addressNode->addChild(identifierNode);
					return;
				}
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
	ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(identifierNode);

	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier after Access Operator...");
	}
	identifierNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(identifierNode);

}

void parser::parseModifyOperator(ASTNode* head)
{
	if (getCurrentToken().second != "IDENTIFIER")
	{
		throw std::runtime_error("excpected a modify operator");
	}
	ASTNode* identifierNode = new ASTNode(getCurrentToken().second, getCurrentToken().first);
	head->addChild(identifierNode);
	consumeToken();
	if (getCurrentToken().first == "++" || getCurrentToken().first == "--")
	{
		ASTNode* modifyOperatorNode = new ASTNode(getCurrentToken().second, getCurrentToken().first); //create AST node
		head->addChild(modifyOperatorNode);
		consumeToken();
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