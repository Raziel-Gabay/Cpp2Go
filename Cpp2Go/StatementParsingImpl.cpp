#include "parser.h"

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
		consumeToken(4);
		currToken = getCurrentToken();
		unconsumeToken(3);
		if (currToken.second == COLON_OPERATOR)
		{
			parseForeachStatement(head);
		}
		else
		{
			parseForStatement(head);
		}
	}
	else if (currToken.second == ELSE_IF_STATEMENT || currToken.second == ELSE_STATEMENT && !head->children.empty())
	{
		if (head->children.back()->name != IF_STATEMENT)
		{
			throw std::runtime_error("excepcted If statement before...");
		}

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
}

void parser::parseIfStatment(ASTNode* head)
{
	token currToken = getCurrentToken();
	//create an if statement node
	ASTNode* ifStatementNode = new ASTNode("IF_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(ifStatementNode);
	//create children for the if statement
	ifStatementNode->addChild(conditionNode);
	ifStatementNode->addChild(blockNode);

	if (currToken.second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");
	consumeToken();

	parseExpression(conditionNode); //we expect an expression to come
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_PARENTHESIS)
		throw std::runtime_error("ERROR: expected a right parenthesis");
	consumeToken(); // Consume the ')'
	currToken = getCurrentToken();

	//check - if body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
		parseBlock(blockNode); //the block part of the tree
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseElseIfStatment(ASTNode* head)
{
	token currToken = getCurrentToken();
	//create an else if statement node
	ASTNode* elseIfStatementNode = new ASTNode("ELSE_IF_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(elseIfStatementNode);
	//create children for the else if statement
	elseIfStatementNode->addChild(conditionNode);
	elseIfStatementNode->addChild(blockNode);

	if (currToken.second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");
	consumeToken();

	parseExpression(conditionNode); //we expect an expression to come
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_PARENTHESIS)
		throw std::runtime_error("ERROR: expected a right parenthesis");
	consumeToken(); // Consume the ')'
	currToken = getCurrentToken();

	//check - else if body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}


void parser::parseElseStatment(ASTNode* head)
{
	token currToken = getCurrentToken();
	//create an if statement node
	ASTNode* elseStatementNode = new ASTNode("ELSE_STATEMENT");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(elseStatementNode);
	elseStatementNode->addChild(blockNode);

	//check - else body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseWhileStatement(ASTNode* head)
{
	token currToken = getCurrentToken();
	//create a while statement node
	ASTNode* whileStatementNode = new ASTNode("WHILE_STATEMENT");
	ASTNode* conditionNode = new ASTNode("CONDITION");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(whileStatementNode);
	//create children for the while statement
	whileStatementNode->addChild(conditionNode);
	whileStatementNode->addChild(blockNode);

	if (currToken.second != LEFT_PARENTHESIS) //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");
	consumeToken();

	parseExpression(conditionNode); //we expect an expression to come
	currToken = getCurrentToken();

	if (getCurrentToken().second != RIGHT_PARENTHESIS)
		throw std::runtime_error("ERROR: expected a right parenthesis");
	consumeToken(); // Consume the ')'
	currToken = getCurrentToken();

	//check - while body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseForStatement(ASTNode* head)
{
	token currToken = getCurrentToken();
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

	if (currToken.second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseVariableDeclaration(initializationNode); // we expect decleration to come (for example: 'int i = 0;')
	parseExpression(conditionNode); //we expect an expression tp come
	parseSemicolon();
	parseModifyOperator(iterationNode); //we expect modify operator to come 

	currToken = getCurrentToken();
	if (currToken.second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	currToken = getCurrentToken();

	//check - for body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree 
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

void parser::parseForeachStatement(ASTNode* head)
{
	token currToken = getCurrentToken();
	//create a for statement node
	ASTNode* foreachStatementNode = new ASTNode("FOREACH_STATEMENT");
	ASTNode* loopVariableNode = new ASTNode("LOOP_VARIABLE");
	ASTNode* containerNode = new ASTNode("CONTAINER");
	ASTNode* blockNode = new ASTNode("BLOCK");

	head->addChild(foreachStatementNode);
	//create children for the for statement
	foreachStatementNode->addChild(loopVariableNode);
	if (currToken.second != "LEFT_PARENTHESIS") //check that the token is '('
		throw std::runtime_error("excepcted LEFT_PARENTHESIS");

	consumeToken();
	parseVariableDeclaration(loopVariableNode); // we expect decleration to come (for example: 'int i')

	currToken = getCurrentToken();
	foreachStatementNode->addChild(new ASTNode(currToken));
	foreachStatementNode->addChild(containerNode);
	foreachStatementNode->addChild(blockNode);
	consumeToken();

	currToken = getCurrentToken();
	if (currToken.second != IDENTIFIER)
	{
		throw std::runtime_error("ERROR: expecting an identifier token...");
	}
	// create idetifier node and add it to the head node
	ASTNode* identifierNode = new ASTNode(currToken);
	containerNode->addChild(identifierNode);

	consumeToken();
	currToken = getCurrentToken();
	if (currToken.second != "RIGHT_PARENTHESIS") //check that the token is ')'
		throw std::runtime_error("excepcted RIGHT_PARENTHESIS");
	consumeToken();
	currToken = getCurrentToken();

	//check - for body
	if (currToken.second != LEFT_BRACE) //check that the token is '{'
		throw std::runtime_error("excepcted LEFT BRACE");
	consumeToken();
	parseBlock(blockNode); //the block part of the tree 
	currToken = getCurrentToken();

	if (currToken.second != RIGHT_BRACE) //check that the token is '}'
		throw std::runtime_error("excepcted RIGHT BRACE");
	consumeToken();
}

