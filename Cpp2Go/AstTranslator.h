#pragma once

#include"ASTNode.h"
#include <iostream>

#define IDENTIFIER "IDENTIFIER"
#define ASSIGNMENT_OPERATOR "ASSIGNMENT_OPERATOR"
#define SEMICOLON "SEMICOLON"
#define LITERAL "LITERAL"
#define BOOL "BOOL"
#define INT_LITERAL "INT_LITERAL"
#define IF_STATEMENT "IF_STATEMENT"
#define WHILE_STATEMENT "WHILE_STATEMENT"
#define FOR_STATEMENT "FOR_STATEMENT"
#define LEFT_BRACE "LEFT_BRACE"
#define RIGHT_BRACE "RIGHT_BRACE"
#define LEFT_PARENTHESIS "LEFT_PARENTHESIS"
#define RIGHT_PARENTHESIS "RIGHT_PARENTHESIS"
#define PROGRAM "PROGRAM"
#define BLOCK "BLOCK"
#define CONDITION "CONDITION"
#define INITIALIZATION "INITIALIZATION"
#define ITERATION "ITERATION"
#define DATATYPE "DATATYPE"
#define DECLARATION "DECLARATION"
#define VAR_KEYWORD "VAR_KEYWORD"

class AstTranslator
{
public:
	// Constructor
	AstTranslator(ASTNode* cppRoot);

	// Desturctor
	~AstTranslator();

	ASTNode* translateProgram(ASTNode* node);
	void iterativeTranslate(ASTNode* cppNode, ASTNode* node);
	void translateDeclaration(ASTNode* sourceNode, ASTNode* &destNode);
	void translateStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateIfStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateWhileStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateForStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateBlock(ASTNode* sourceNode, ASTNode* &destNode);
	void translateExpression(ASTNode* sourceNode, ASTNode* &destNode);

	ASTNode* getAST();
private:
	ASTNode* _astRoot; //this is the go ast
	ASTNode* _cppRoot; //this is the cpp ast
};