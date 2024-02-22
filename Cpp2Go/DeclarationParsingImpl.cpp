#include "parser.h"

void parser::parseVariableDeclaration(ASTNode* head)
{
	ASTNode* variableDeclarationNode = new ASTNode("VARIABLE_DECLARATION");
	head->addChild(variableDeclarationNode);

	std::string datatype;
	parseType(datatype, variableDeclarationNode);

	token currToken = getCurrentToken();

	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}
	// create idetifier node and add it to the head node
	ASTNode* identifierNode = new ASTNode(currToken);
	variableDeclarationNode->addChild(identifierNode);

	_identifiersTypes.emplace(currToken.first, datatype);
	if (head->name == "BLOCK")
		_localsVariables.emplace(currToken.first, datatype);
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second == ASSIGNMENT_OPERATOR)
	{
		unconsumeToken();
		variableDeclarationNode->children.pop_back();
		parseExpression(variableDeclarationNode);
		currToken = getCurrentToken();
	}
	else if (currToken.second == COLON_OPERATOR)
	{
		if (head->name != LOOP_VARIABLE)
		{
			throw std::runtime_error("ERROR: expecting to be in foreach loop...");
		}
		return;
	}

	parseSemicolon();
}

void parser::parseFunctionDeclaration(ASTNode* head)
{
	ASTNode* functionDeclarationNode = new ASTNode("FUNCTION_DECLARATION");
	ASTNode* returnValueNode = new ASTNode("RETURN_VALUE");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(functionDeclarationNode);
	functionDeclarationNode->addChild(returnValueNode);

	std::string datatype;
	parseType(datatype, returnValueNode);

	token currToken = getCurrentToken();

	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}

	// create idetifier node and add it to the head node
	ASTNode* identifierNode = new ASTNode(currToken);
	functionDeclarationNode->addChild(identifierNode);

	_functionIdentifiers.emplace(currToken.first, returnValueNode->children.front()->value);

	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second != LEFT_PARENTHESIS)
	{
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");
	}

	int num_of_parameters = 0;

	consumeToken();
	currToken = getCurrentToken();
	while (currToken.second != RIGHT_PARENTHESIS)
	{
		if (currToken.second.find("DATATYPE") == std::string::npos)
		{
			throw std::runtime_error("ERROR: expecting an parameter name or right parenthsis token......");
		}
		ASTNode* parameterNode = new ASTNode(PARAMETER);
		functionDeclarationNode->addChild(parameterNode);

		parseType(datatype, parameterNode);
		currToken = getCurrentToken();

		if (currToken.second != IDENTIFIER)
		{
			throw std::runtime_error("ERROR: expecting an parameter name token...");
		}
		ASTNode* identifierNode = new ASTNode(currToken);
		parameterNode->addChild(identifierNode);
		_identifiersTypes.emplace(currToken.first, datatype);
		_localsVariables.emplace(currToken.first, datatype);
		num_of_parameters++;

		consumeToken();
		currToken = getCurrentToken();
		if (currToken.second == COMMA)
		{
			consumeToken();
			currToken = getCurrentToken();
		}
		else if (currToken.second.find("DATATYPE") != std::string::npos)
		{
			throw std::runtime_error("ERROR: expecting an comma or right parenthsis token......");
		}
	}

	if (currToken.second != RIGHT_PARENTHESIS) //check that the token is ')'
	{
		throw std::runtime_error("excepcted RIGHT PARENTHESIS");
	}
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second == SEMICOLON)
	{
		throw std::runtime_error("ERROR: GO dosen't support function declartion without block...");
	}

	if (currToken.second != LEFT_BRACE)
	{
		throw std::runtime_error("ERROR: expecting an block...");
	}

	consumeToken();
	functionDeclarationNode->addChild(blockNode);
	parseBlock(blockNode, num_of_parameters);
	currToken = getCurrentToken();
	if (currToken.second == RIGHT_BRACE)
	{
		consumeToken();
	}

}

void parser::parseArrayDeclaration(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create an array node, and add it to the head node
	ASTNode* arrayNode = new ASTNode("ARRAY_DECLARATION");
	head->addChild(arrayNode);

	ASTNode* arrayLengthNode = new ASTNode("ARRAY_LENGTH"); // create an ast node of the length of the array
	arrayNode->addChild(arrayLengthNode);

	std::string datatype;
	parseType(datatype, arrayNode);
	arrayNode->children.back()->name += "_ARRAY"; //declare that the data type is of an array

	//consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != IDENTIFIER)
		throw std::runtime_error("excepcted identifier");
	ASTNode* identifierNode = new ASTNode(currToken); // create an ast representing the name of the array
	arrayNode->addChild(identifierNode);

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != LEFT_SQUARE_PARENTHESIS)
		throw std::runtime_error("excepcted the char '['");
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second.find(INT_LITERAL) == std::string::npos)
		throw std::runtime_error("excepcted int datatype");

	ASTNode* intLiteralNode = new ASTNode(currToken); // create an ast node representing the length of the array
	arrayLengthNode->addChild(intLiteralNode);
	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_SQUARE_PARENTHESIS)
		throw std::runtime_error("excepcted the char ']'");
	consumeToken();
	currToken = getCurrentToken();

	parseSemicolon();
}

void parser::parsePointerDeclaration(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create an array node, and add it to the head node
	ASTNode* pointerDeclarationNode = new ASTNode("POINTER_DECLARATION");
	head->addChild(pointerDeclarationNode);

	std::string datatype;
	parseType(datatype, pointerDeclarationNode);

	currToken = getCurrentToken();
	if (currToken.second == POINTER_OPERATOR)
	{
		ASTNode* pointerOperatorNode = new ASTNode(currToken);
		pointerDeclarationNode->addChild(pointerOperatorNode);
		consumeToken();
		currToken = getCurrentToken();
	}
	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: excepcting an identifier token...");
	}

	// create idetifier node and add it to the head node
	ASTNode* identifierNode = new ASTNode(currToken);
	pointerDeclarationNode->addChild(identifierNode);
	_identifiersTypes.emplace(currToken.first, datatype);
	if (head->name == "BLOCK")
		_localsVariables.emplace(currToken.first, datatype);
	consumeToken();
	currToken = getCurrentToken();

	if (currToken.second == SEMICOLON)
	{
		parseSemicolon();
	}
	else if (currToken.second == ASSIGNMENT_OPERATOR)
	{
		unconsumeToken();
		pointerDeclarationNode->children.pop_back();
		parseExpression(pointerDeclarationNode);
		currToken = getCurrentToken();
		parseSemicolon();
	}
	else
		throw std::runtime_error("ERROR: expecting an semicolon or assignment operator token...");
}
