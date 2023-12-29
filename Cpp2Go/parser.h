#pragma once
#include "ASTNode.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <string>

#define IDENTIFIER "IDENTIFIER"
#define ASSIGNMENT_OPERATOR "ASSIGNMENT_OPERATOR"
#define SEMICOLON "SEMICOLON"
#define LITERAL "LITERAL"
#define BOOL "BOOL"
#define INT_LITERAL "INT_LITERAL"
#define IF_STATEMENT "IF_STATEMENT"
#define ELSE_IF_STATEMENT "ELSE_IF_STATEMENT"
#define ELSE_STATEMENT "ELSE_STATEMENT"
#define WHILE_STATEMENT "WHILE_STATEMENT"
#define FOR_STATEMENT "FOR_STATEMENT"
#define LEFT_BRACE "LEFT_BRACE"
#define RIGHT_BRACE "RIGHT_BRACE"
#define LEFT_PARENTHESIS "LEFT_PARENTHESIS"
#define RIGHT_PARENTHESIS "RIGHT_PARENTHESIS"

typedef std::pair<std::string, std::string> token;
typedef std::multimap<std::string, std::string> identifiersVector;
typedef std::vector<std::pair<std::string, std::string>> tokensVector;
typedef std::pair<std::string, std::string> token;

const std::unordered_set<std::string> ArithmeticOperators = { "+", "-", "*", "/"};
const std::unordered_set<std::string> RelationalOperators = { "==", "!=", "<", ">", "<=", ">="};
const std::unordered_set<std::string> LogicalOperators = { "&&", "||", "!"};
const std::unordered_set<std::string> BitwiseOperators = { "&", "|", "^", "<<", ">>" };
const std::unordered_set<std::string> AssignmentOperators = { "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=" };
const std::unordered_set<std::string> AccessOperators = { ",", "->", ".", "[]" };





class parser
{
public:
	// Constructor
	parser(tokensVector tokenStream);

	// Desturctor
	~parser();

	// Parse Functions
	ASTNode* parseProgram();
	void parseDeclaration(ASTNode* head);
	void parseStatement(ASTNode* head);
	void parseIfStatment(ASTNode* head);
	void parseElseIfStatment(ASTNode* head);
	void parseElseStatment(ASTNode* head);
	void parseWhileStatement(ASTNode* head);
	void parseForStatement(ASTNode* head);
	void parseBlock(ASTNode* head);

	void parseExpression(ASTNode* head);
	void parseType(std::string& datatype, ASTNode* head);

	void parseArithmeticOperator(const std::string& op, ASTNode* head);
	void parseRelationalOperator(const std::string& op, ASTNode* head);
	void parseLogicalOperator(const std::string& op, ASTNode* head);
	void parseBitwiseOperator(const std::string& op, ASTNode* head);
	void parseAssignmentOperator(const std::string& op, ASTNode* head);
	void parseAccessOperator(const std::string& op, ASTNode* head);
	void parseModifyOperator(ASTNode* head);
	bool isUnaryOperator(const token& t);

	//get functions
	token getCurrentToken();
	ASTNode* getAST();
private:
	tokensVector _tokensStream;
	identifiersVector _identifiersTypes;
	size_t _currentPosition;
	ASTNode* _astRoot;

	// Help Methods
	void consumeToken();
	void consumeToken(size_t n);
	void unconsumeToken();
	bool isBinaryOperator(token t);
};

