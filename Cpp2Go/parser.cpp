#include "parser.h"


parser::parser(tokensVector tokenStream)
	: _tokensStream(tokenStream), _currentPosition(0), _identifiersTypes({})
{
	_astRoot = parseProgram();
	if (!isFunctionExists())
	{
		throw std::runtime_error("ERROR: function does not exist!");
	}
}

parser::~parser()
{
}

ASTNode* parser::parseProgram()
{
	if (_tokensStream.front().second != HASHTAG_OPERATOR)
	{
		throw std::runtime_error("ERROR: this code is not valid!");
	}
	ASTNode* programNode = new ASTNode("PROGRAM");
	while (_currentPosition < _tokensStream.size())
	{
		token currToken = getCurrentToken();
		if (currToken.second.find("DATATYPE") != std::string::npos)
		{
			consumeToken(2);
			currToken = getCurrentToken();
			unconsumeToken(2);

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
				consumeToken();
				currToken = getCurrentToken();
				unconsumeToken();
				if (currToken.second == POINTER_OPERATOR)
				{
					currToken = getCurrentToken();
					parsePointerDeclaration(programNode);
				}
				else
				{
					currToken = getCurrentToken();
					parseVariableDeclaration(programNode);
				}
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
			if (currToken.second == INCLUDE_KEYWORD)
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
		else if (currToken.second == STD_COUT_DECLARATION)
		{
			parseStdCout(programNode);
		}
		else if (currToken.second == STD_CIN_DECLARATION)
		{
			parseStdCin(programNode);
		}
		else if (currToken.second == STD_CERR_DECLARATION)
		{
			parseStdCerr(programNode);
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
	ASTNode* functionNameNode = new ASTNode(currToken);
	head->addChild(functionCallNode);
	functionCallNode->addChild(functionNameNode);
	_functionCalls.push_back(currToken.first);

	consumeToken(2);
	currToken = getCurrentToken();
	while (currToken.second != RIGHT_PARENTHESIS)
	{
		if (currToken.second.find("LITERAL") != std::string::npos || currToken.second == IDENTIFIER)
		{
			ASTNode* parameterNode = new ASTNode(PARAMETER);
			functionCallNode->addChild(parameterNode);
			ASTNode* identifierOrLiteralNode = new ASTNode(currToken);
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

void parser::parseStruct(ASTNode* head)
{
	if (head->name == "BLOCK")
		throw std::runtime_error("cannot create struct inside block");
	token currToken = getCurrentToken();
	ASTNode* structNode = new ASTNode("STRUCT");
	ASTNode* membersNode = new ASTNode("MEMBERS");
	ASTNode* structKeywordNode = new ASTNode(currToken);
	head->addChild(structNode);
	structNode->addChild(structKeywordNode);

	consumeToken();
	currToken = getCurrentToken();
	if (currToken.second == IDENTIFIER)
	{
		// create idetifier node and add it to the head node
		ASTNode* identifierNode = new ASTNode(currToken);
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
				ASTNode* identifierNode = new ASTNode(currToken);
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
				consumeToken();
				currToken = getCurrentToken();
				unconsumeToken();
				if (currToken.second == POINTER_OPERATOR)
				{
					currToken = getCurrentToken();
					parsePointerDeclaration(head);
				}
				else
				{
					currToken = getCurrentToken();
					parseVariableDeclaration(head);
				}
			}
			num_of_locals++;

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
		else if (currToken.second == STD_COUT_DECLARATION)
		{
			parseStdCout(head);
		}
		else if (currToken.second == STD_CIN_DECLARATION)
		{
			parseStdCin(head);
		}
		else if (currToken.second == STD_CERR_DECLARATION)
		{
			parseStdCerr(head);
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
	ASTNode* includeDirectiveNode = new ASTNode(INCLUDE_DIRECTIVE);

	ASTNode* includeKeyWordNode = new ASTNode(currToken);

	head->addChild(includeDirectiveNode);
	includeDirectiveNode->addChild(includeKeyWordNode);

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != LESS_THAN_OPERATOR)
		throw std::runtime_error("excepcted the char: '<'");

	consumeToken();
	currToken = getCurrentToken();

	ASTNode* libaryNameNode = new ASTNode(currToken);
	includeDirectiveNode->addChild(libaryNameNode);

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != MORE_THAN_OPERATOR)
		throw std::runtime_error("excepcted the char: '>'");
	consumeToken();

}

void parser::parseStdCout(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create std cout node and add it to the head node
	ASTNode* stdCoutnode = new ASTNode(STD_COUT_DECLARATION);
	head->addChild(stdCoutnode);

	if (getCurrentToken().second != STD_COUT_DECLARATION)
		throw std::runtime_error("excepcted: 'std::cout'");

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != INSERTION_OPERATOR)
		throw std::runtime_error("excepcted the operator: '<<'");

	ASTNode* stdCoutDeclarationNode = new ASTNode("PRINT");
	stdCoutnode->addChild(stdCoutDeclarationNode);

	consumeToken();
	currToken = getCurrentToken();
	ASTNode* stringLiteralNode = new ASTNode(currToken);
	stdCoutnode->addChild(stringLiteralNode);
}

void parser::parseStdCin(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create std cin node and add it to the head node
	ASTNode* stdCinNode = new ASTNode(STD_CIN_DECLARATION);
	head->addChild(stdCinNode);

	ASTNode* stdCinDeclarationNode = new ASTNode("SCAN");
	stdCinNode->addChild(stdCinDeclarationNode);

	if (getCurrentToken().second != STD_CIN_DECLARATION)
		throw std::runtime_error("excepcted: 'std::cin'");

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_SHIFT_OPERATOR)
		throw std::runtime_error("excepcted the operator: '>>'");

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != IDENTIFIER)
		throw std::runtime_error("excepcted identifier");

	ASTNode* identifierNode = new ASTNode(currToken.second, currToken.first);
	stdCinNode->addChild(identifierNode);
}

void parser::parseStdCerr(ASTNode* head)
{
	token currToken = getCurrentToken();

	//create std cerr node and add it to the head node
	ASTNode* stdCerrnode = new ASTNode(STD_CERR_DECLARATION);
	head->addChild(stdCerrnode);

	ASTNode* stdCerrDeclarationNode = new ASTNode("ERROR");
	stdCerrnode->addChild(stdCerrDeclarationNode);

	if (getCurrentToken().second != STD_CERR_DECLARATION)
		throw std::runtime_error("excepcted: 'std::cerr'");

	consumeToken();
	currToken = getCurrentToken();

	if (getCurrentToken().second != INSERTION_OPERATOR)
		throw std::runtime_error("excepcted the operator: '<<'");

	consumeToken();
	currToken = getCurrentToken();

	ASTNode* stringLiteralNode = new ASTNode(currToken);
	stdCerrnode->addChild(stringLiteralNode);
}

void parser::parseType(std::string& datatype, ASTNode* head)
{
	token currToken = getCurrentToken();
	if (currToken.second.find("DATATYPE") == std::string::npos)
	{
		throw std::runtime_error("ERROR: expected a datatype token");
	}
	consumeToken();
	datatype = currToken.second.substr(currToken.second.find("_") + 1);
	//create ast node
	ASTNode* dataTypeNode = new ASTNode(currToken);
	head->addChild(dataTypeNode);
}

void parser::parseSemicolon()
{
	token currToken = getCurrentToken();
	if (currToken.second != SEMICOLON)
	{
		throw std::runtime_error("ERROR: expecting an semicolon token...");
	}
	consumeToken();
}

token parser::getCurrentToken()
{
	if (_currentPosition < _tokensStream.size()) {
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

void parser::unconsumeToken(size_t n)
{
	if ((_currentPosition - n) > 0)
	{
		_currentPosition -= n;
	}
}

bool parser::isFunctionExists()
{
	for (std::string functionCall : _functionCalls)
	{
		if (_functionIdentifiers.find(functionCall) == _functionIdentifiers.end())
		{
			return false;
		}

	}
	return true;
}