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
		ASTNode* identifierNode = new ASTNode(currToken);
		head->addChild(identifierNode);
	}
	else if (getCurrentToken().second.find(LITERAL) != std::string::npos)
	{
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken);
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
				ASTNode* identifierNode = new ASTNode(currToken);
				head->addChild(identifierNode);
				return;
			}
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}

	ASTNode* literalNode = new ASTNode(currToken);
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
		ASTNode* identifierNode = new ASTNode(currToken);
		head->addChild(identifierNode);
	}
	else if (getCurrentToken().second.find(LITERAL) != std::string::npos)
	{
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken);
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
				ASTNode* identifierNode = new ASTNode(currToken);
				head->addChild(identifierNode);
				return;
			}
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}

	ASTNode* literalNode = new ASTNode(currToken);
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
	ASTNode* intLiteralNode = new ASTNode(currToken);
	head->addChild(intLiteralNode);
	
	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != INT_LITERAL)
	{
		throw std::runtime_error("ERROR: Expected an integer operand after bitwise operator.");
	}	
	intLiteralNode = new ASTNode(currToken);
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
	ASTNode* identifierNode = new ASTNode(currToken);
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
				ASTNode* identifierNode = new ASTNode(currToken);
				head->addChild(identifierNode);
				return;
			}
		}
		else if (currToken.second == ADDRESS_OF_OPERATOR)
		{
			ASTNode* addressNode = new ASTNode(currToken);
			head->addChild(addressNode);
			consumeToken();
			currToken = getCurrentToken();
			if (currToken.second == IDENTIFIER)
			{
				if (datatype == _identifiersTypes.find(currToken.first)->second)
				{
					// create idetifier node and add it to the head node
					ASTNode* identifierNode = new ASTNode(currToken);
					addressNode->addChild(identifierNode);
					return;
				}
			}
		}
		else if (currToken.second == LEFT_PARENTHESIS)
		{
			parseTernaryOperator(datatype, head);
			return;
		}
		throw std::runtime_error("ERROR: cannot use two diffrent types...");
	}
	else if (getCurrentToken().second.find(LITERAL) != std::string::npos)
	{
		ASTNode* literalNode = new ASTNode(currToken);
		head->addChild(literalNode);
	}
	
	
}

void parser::parseAccessOperator(const std::string& op, ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier before Access Operator...");
	}
	ASTNode* identifierNode = new ASTNode(currToken);
	head->addChild(identifierNode);

	consumeToken(2);
	currToken = getCurrentToken();
	if (getCurrentToken().second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expected an identifier after Access Operator...");
	}
	identifierNode = new ASTNode(currToken);
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

void parser::parseTernaryOperator(std::string datatype, ASTNode* head)
{
	ASTNode* conditionNode = new ASTNode("CONDITION");
	token currToken = getCurrentToken();
	if (currToken.second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");
	consumeToken();

	parseExpression(conditionNode); //we expect an expression to come
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_PARENTHESIS)
		throw std::runtime_error("ERROR: expected a right parenthesis");
	consumeToken(); // Consume the ')'
	currToken = getCurrentToken();

	if (currToken.second != TERNARY_OPERATOR)
	{
		if (datatype == "BOOL")
		{
			head->addChild(conditionNode);
			return;
		}
		else
			throw std::runtime_error("ERROR: expected ternary operator");

	}
	consumeToken();

	ASTNode* expressionTrueNode = new ASTNode("EXPRESSION_TRUE");
	ASTNode* expressionFalseNode = new ASTNode("EXPRESSION_FALSE");
	ASTNode* ternaryOperatorNode = new ASTNode(currToken);
	head->addChild(ternaryOperatorNode);
	ternaryOperatorNode->addChild(conditionNode);
	ternaryOperatorNode->addChild(expressionTrueNode);
	ternaryOperatorNode->addChild(expressionFalseNode);
	currToken = getCurrentToken();

	// note: we gonna support 2 cases, case 1: literal value, case 2: identifier.
	if (currToken.second == IDENTIFIER)
	{
		if (datatype != _identifiersTypes.find(currToken.first)->second)
			throw std::runtime_error("ERROR: expected same type!");

		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken);
		expressionTrueNode->addChild(identifierNode);
	}
	else if (currToken.second.find(LITERAL) != std::string::npos)
	{
		if (datatype != currToken.second.substr(0,currToken.second.find("_")))
			throw std::runtime_error("ERROR: expected same type!");
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken);
		expressionTrueNode->addChild(literalNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
	}
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second != COLON_OPERATOR)
		throw std::runtime_error("ERROR: expected colon operator");
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second == IDENTIFIER)
	{
		if (datatype != _identifiersTypes.find(currToken.first)->second)
			throw std::runtime_error("ERROR: expected same type!");

		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken);
		expressionFalseNode->addChild(identifierNode);
	}
	else if (currToken.second.find(LITERAL) != std::string::npos)
	{
		if (datatype != currToken.second.substr(0, currToken.second.find("_")))
			throw std::runtime_error("ERROR: expected same type!");
		// create literal node and add it to the head node
		ASTNode* literalNode = new ASTNode(currToken);
		expressionFalseNode->addChild(literalNode);
	}
	else
	{
		throw std::runtime_error("ERROR: expected an identifier or literal...");
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