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
#define PARAMETER "PARAMETER"
#define RETURN_VALUE "RETURN_VALUE"
#define STRUCT_KEYWORD "STRUCT_KEYWORD"
#define HASHTAG_OPERATOR "HASHTAG_OPERATOR"
#define HASHTAG "#"
#define INCLUDE "INCLUDE"
#define INCLUDE_DIRECTIVE "INCLUDE_DIRECTIVE"
#define INCLUDE_KEYWORD "INCLUDE_KEYWORD"
#define LESS_THAN_OPERATOR "LESS_THAN_OPERATOR"
#define MORE_THAN_OPERATOR "MORE_THAN_OPERATOR"
#define COMMA "COMMA"
#define DATATYPE_STRING "DATATYPE_STRING"
#define LEFT_SQUARE_PARENTHESIS "LEFT_SQUARE_PARENTHESIS"
#define RIGHT_SQUARE_PARENTHESIS "RIGHT_SQUARE_PARENTHESIS"
#define DATATYPE "DATATYPE"
#define POINTER_OPERATOR "POINTER_OPERATOR"
#define ADDRESS_OF_OPERATOR  "ADDRESS_OF_OPERATOR"
#define INSERTION_OPERATOR "INSERTION_OPERATOR"
#define STD_COUT_DECLARATION "STD_COUT_DECLARATION"

typedef std::pair<std::string, std::string> token;
typedef std::multimap<std::string, std::string> identifiersVector;
typedef std::map<std::string, std::string>;
typedef std::vector<std::pair<std::string, std::string>> tokensVector;

const std::unordered_set<std::string> ArithmeticOperators = { "+", "-", "*", "/", "%"};
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
	// declaration parsing
	void parseVariableDeclaration(ASTNode* head);
	void parseFunctionDeclaration(ASTNode* head);
	void parseArrayDeclaration(ASTNode* head);
	void parsePointerDeclaration(ASTNode* head);

	// statement parsing
	void parseStatement(ASTNode* head);
	void parseIfStatment(ASTNode* head);
	void parseElseIfStatment(ASTNode* head);
	void parseElseStatment(ASTNode* head);
	void parseWhileStatement(ASTNode* head);
	void parseForStatement(ASTNode* head);

	// operator parsing
	void parseExpression(ASTNode* head);
	void parseArithmeticOperator(const std::string& op, ASTNode* head);
	void parseRelationalOperator(const std::string& op, ASTNode* head);
	void parseLogicalOperator(const std::string& op, ASTNode* head);
	void parseBitwiseOperator(const std::string& op, ASTNode* head);
	void parseAssignmentOperator(const std::string& op, ASTNode* head);
	void parseAccessOperator(const std::string& op, ASTNode* head);
	void parseModifyOperator(ASTNode* head);
	bool isUnaryOperator(const token& t);

	// other parsing
	void parseFunctionCall(ASTNode* head);
	void parseStruct(ASTNode* head);
	void parseBlock(ASTNode* head, int num_of_locals = 0);
	void parseIncludeDirective(ASTNode* head);
	void parseStdCout(ASTNode* head);
	void parseType(std::string& datatype, ASTNode* head);
	void parseSemicolon();

	bool isFunctionExists();
	//get functions
	token getCurrentToken();
	ASTNode* getAST();
private:
	tokensVector _tokensStream;
	identifiersVector _identifiersTypes;
	identifiersVector _localsVariables;
	std::map<std::string, std::string> _functionIdentifiers;
	std::vector<std::string> _functionCalls;
	size_t _currentPosition;
	ASTNode* _astRoot;

	// Help Methods
	void consumeToken();
	void consumeToken(size_t n);
	void unconsumeToken();
	void unconsumeToken(size_t n);
	bool isBinaryOperator(token t);
};

