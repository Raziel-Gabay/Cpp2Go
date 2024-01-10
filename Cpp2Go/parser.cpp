#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentPosition(0), _identifiersTypes({})
{
	_astRoot = parseProgram();
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
			consumeToken(2);
			currToken = getCurrentToken();
			unconsumeToken();
			unconsumeToken();

			if (currToken.second == LEFT_PARENTHESIS)
			{
				currToken = getCurrentToken();
				parseFunctionDeclaration(programNode);
			}
			else if (currToken.second == LEFT_SQUARE_PARENTHESIS)
			{
				currToken = getCurrentToken();
				parseArrayDeclaration(programNode);
			}
			else
			{
				currToken = getCurrentToken();
				parseVariableDeclaration(programNode);
			}
		}
		else if (currToken.second == IF_STATEMENT || currToken.second == ELSE_IF_STATEMENT || currToken.second == ELSE_STATEMENT ||
				currToken.second == WHILE_STATEMENT || currToken.second == FOR_STATEMENT)
		{
			parseStatement(programNode);
		}
		else if (currToken.second == STRUCT_KEYWORD)
		{
			parseStruct(programNode);
		}
		else if (currToken.second == HASHTAG_OPERATOR)
		{
			consumeToken();
			currToken = getCurrentToken();
			if (currToken.second == INCLUDE)
				consumeToken();
				currToken = getCurrentToken();
				parseIncludeDirective(programNode);
		}
		else if (currToken.second == IDENTIFIER)
		{
			consumeToken();
			currToken = getCurrentToken();
			if (currToken.second == LEFT_PARENTHESIS)
			{
				unconsumeToken();
				parseFunctionCall(programNode);
			}
		}
		else
		{
			parseExpression(programNode);
			parseSemicolon();
		}

	}
	return programNode;
}


void parser::parseFunctionCall(ASTNode* head)
{
	token currToken = getCurrentToken();
	ASTNode* functionCallNode = new ASTNode("FUNCTION_CALL");
	ASTNode* functionNameNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(functionCallNode);
	functionCallNode->addChild(functionNameNode);

	consumeToken(2);
	currToken = getCurrentToken();
	while (currToken.second != RIGHT_PARENTHESIS)
	{
		if (currToken.second.find("LITERAL") != std::string::npos || currToken.second == IDENTIFIER)
		{
			ASTNode* parameterNode = new ASTNode(PARAMETER);
			functionCallNode->addChild(parameterNode);	
			ASTNode* identifierOrLiteralNode = new ASTNode(currToken.second, currToken.first);
			parameterNode->addChild(identifierOrLiteralNode);
			consumeToken();
			currToken = getCurrentToken();
			if (currToken.second == COMMA)
			{
				consumeToken();
				currToken = getCurrentToken();
			}
			else if (currToken.second.find("LITERAL") != std::string::npos || currToken.second == IDENTIFIER)
			{
				throw std::runtime_error("ERROR: expecting an comma or right parenthsis token......");
			}
		}
		else
		{
			throw std::runtime_error("ERROR: expecting an parameter name or right parenthsis token......");
		}
	}
	if (currToken.second == RIGHT_PARENTHESIS) //check that the token is ')'
	{
		consumeToken();
		currToken = getCurrentToken();
	}
	else
		throw std::runtime_error("excepcted RIGHT PARENTHESIS");

	parseSemicolon();
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
	else if (currToken.second == ELSE_IF_STATEMENT || currToken.second == ELSE_STATEMENT && !head->children.empty())
	{
		if (head->children.back()->name == IF_STATEMENT)
		{
			if (currToken.second == ELSE_IF_STATEMENT)
			{
				consumeToken();
				parseElseIfStatment(head);
			}
			else if (currToken.second == ELSE_STATEMENT)
			{
				consumeToken();
				parseElseStatment(head);
			}
		}
		else
		{
			throw std::runtime_error("excepcted If statement before...");
		}
	}
}

void parser::parseIfStatment(ASTNode* head)
{
	//create an if statement node
	ASTNode* ifStatementNode = new ASTNode("IF_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(ifStatementNode);
	//create children for the if statement
	ifStatementNode->addChild(conditionNode);
	ifStatementNode->addChild(blockNode);

	if (getCurrentToken().second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	parseExpression(conditionNode); //we expect an expression to come

	//check - if body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree

	if (getCurrentToken().second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseElseIfStatment(ASTNode* head)
{
	//create an else if statement node
	ASTNode* elseIfStatementNode = new ASTNode("ELSE_IF_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(elseIfStatementNode);
	//create children for the else if statement
	elseIfStatementNode->addChild(conditionNode);
	elseIfStatementNode->addChild(blockNode);

	if (getCurrentToken().second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	parseExpression(conditionNode); //we expect an expression to come

	//check - else if body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree

	if (getCurrentToken().second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}


void parser::parseElseStatment(ASTNode* head)
{
	//create an if statement node
	ASTNode* elseStatementNode = new ASTNode("ELSE_STATEMENT");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(elseStatementNode);
	elseStatementNode->addChild(blockNode);

	//check - else body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree

	if (getCurrentToken().second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseStruct(ASTNode* head)
{
	if (head->name == "BLOCK")
		throw std::runtime_error("cannot create struct inside block");
	token currToken = getCurrentToken();
	ASTNode* structNode = new ASTNode("STRUCT");
	ASTNode* membersNode = new ASTNode("MEMBERS");
	ASTNode* structKeywordNode = new ASTNode(currToken.second, currToken.first);
	head->addChild(structNode);
	structNode->addChild(structKeywordNode);

	consumeToken();
	currToken = getCurrentToken();
	if (currToken.second == IDENTIFIER)
	{
		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
		structNode->addChild(identifierNode);
		structNode->addChild(membersNode);
		consumeToken();
		currToken = getCurrentToken();
	}
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	currToken = getCurrentToken();
	while (currToken.second != RIGHT_BRACE)
	{
		if (currToken.second.find("DATATYPE") != std::string::npos)
		{
			ASTNode* declarationNode = new ASTNode("VARIABLE_DECLARATION");
			membersNode->addChild(declarationNode);
			//add the type to the declartion
			std::string datatype;
			parseType(datatype, declarationNode);
			currToken = getCurrentToken();
			//add the identfier to the declaration
			if (currToken.second == IDENTIFIER)
			{
				// create idetifier node and add it to the head node
				ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
				declarationNode->addChild(identifierNode);
				consumeToken();
				currToken = getCurrentToken();
			}
			if (currToken.second == SEMICOLON)
			{
				consumeToken();
				currToken = getCurrentToken();
			}
			else
				throw std::runtime_error("ERROR: expecting an semicolon token...");
		}
		else
			throw std::runtime_error("ERROR: excepcted declaration...");
	}
	consumeToken();
	currToken = getCurrentToken();
	if (currToken.second == SEMICOLON)
		consumeToken();
	else
		throw std::runtime_error("ERROR: excepcted declaration...");
}

void parser::parseWhileStatement(ASTNode* head)
{
	//create a while statement node
	ASTNode* whileStatementNode = new ASTNode("WHILE_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(whileStatementNode);
	//create children for the while statement
	whileStatementNode->addChild(conditionNode);
	whileStatementNode->addChild(blockNode);

	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	parseExpression(conditionNode); //we expect an expression to come

	//check - while body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree

	if (getCurrentToken().second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseForStatement(ASTNode* head)
{
	//create a for statement node
	ASTNode* forStatementNode = new ASTNode("FOR_STATEMENT");
	ASTNode* initializationNode = new ASTNode("INITIALIZATION");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* iterationNode = new ASTNode("ITERATION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(forStatementNode);
	//create children for the for statement
	forStatementNode->addChild(initializationNode);
	forStatementNode->addChild(conditionNode);
	forStatementNode->addChild(iterationNode);
	forStatementNode->addChild(blockNode);

	if (getCurrentToken().second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseVariableDeclaration(initializationNode); // we expect decleration to come (for example: 'int i = 0;')
	parseExpression(conditionNode); //we expect an expression tp come
	parseSemicolon();
	parseModifyOperator(iterationNode); //we expect modify operator to come 

	if (getCurrentToken().second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();

	//check - for body
	if (getCurrentToken().second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree 

	if (getCurrentToken().second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseBlock(ASTNode* head, int num_of_locals)
{
	while (true)
	{
		token currToken = getCurrentToken();
		if (currToken.second == IF_STATEMENT || currToken.second == ELSE_IF_STATEMENT || currToken.second == ELSE_STATEMENT ||
			currToken.second == WHILE_STATEMENT || currToken.second == FOR_STATEMENT)
		{
			parseStatement(head);
		}
		else if (currToken.second.find("DATATYPE") != std::string::npos)
		{
			consumeToken(2);
			currToken = getCurrentToken();
			unconsumeToken();
			unconsumeToken();

			if (currToken.second == LEFT_PARENTHESIS)
			{
				throw std::runtime_error("You Cannot declare a function in a block!");
			}
			else if (currToken.second == LEFT_SQUARE_PARENTHESIS)
			{
				currToken = getCurrentToken();
				parseArrayDeclaration(head);
			}
			else
			{
				currToken = getCurrentToken();
				parseVariableDeclaration(head);
				num_of_locals++;
			}
			
		}
		else if (currToken.second == RIGHT_BRACE)
		{
			for (auto it = _localsVariables.begin(); it != _localsVariables.end();)
			{
				if (num_of_locals)
				{
					it = _localsVariables.erase(it);
					num_of_locals--;
				}
				if (!num_of_locals)
					return;
			}

			return;
		}
		else if (currToken.second == IDENTIFIER)
		{
			consumeToken();
			currToken = getCurrentToken();
			unconsumeToken();
			if (currToken.second == LEFT_PARENTHESIS)
			{
				parseFunctionCall(head);
			}
			else
			{
				parseExpression(head);
				parseSemicolon();
			}

		}
		else
		{
			parseExpression(head);
			parseSemicolon();
		}
	}

}

void parser::parseIncludeDirective(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create an include directive node
	ASTNode* includeDirectiveNode = new ASTNode("INCLUDE_DIRECTIVE");
	
	ASTNode* includeKeyWordNode = new ASTNode(currToken.second, currToken.first);
	
	head->addChild(includeDirectiveNode);
	includeDirectiveNode->addChild(includeKeyWordNode);

	consumeToken();
	currToken = getCurrentToken();

	if(getCurrentToken().second != LESS_THAN_OPERATOR)
		throw std::runtime_error("excepcted the char: '<'");

	consumeToken();
	currToken = getCurrentToken();

	ASTNode* libaryNameNode = new ASTNode(currToken.second, currToken.first);
	includeDirectiveNode->addChild(libaryNameNode);

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != MORE_THAN_OPERATOR)
		throw std::runtime_error("excepcted the char: '>'");
	consumeToken();

}


void parser::parseExpression(ASTNode* head)
{
	if (isUnaryOperator(getCurrentToken()) || getCurrentToken().second == IDENTIFIER || getCurrentToken().second.find(LITERAL) != std::string::npos)
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
	else if (getCurrentToken().second == RIGHT_BRACE)
	{
		return;
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

void parser::parseSemicolon()
{
	token currToken = getCurrentToken();
	if (currToken.second == SEMICOLON)
	{
		consumeToken();
	}
	else
	{
		throw std::runtime_error("ERROR: expecting an semicolon token...");
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

void parser::parseModifyOperator(ASTNode* head)
{
	if (getCurrentToken().second == "IDENTIFIER")
	{
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
	else
	{
		throw std::runtime_error("excpected a modify operator");
	}
}

token parser::getCurrentToken()
{
	;	if (_currentPosition < _tokensStream.size()) {
		return _tokensStream[_currentPosition];
	}
	return token("", "");
}

ASTNode* parser::getAST()
{
	return _astRoot;
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