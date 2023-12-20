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

class AstTranslator
{
public:
	// Constructor
	AstTranslator(ASTNode* cppRoot);

	// Desturctor
	~AstTranslator();

	ASTNode* translateProgram(ASTNode* node);
	void recursiveTranslate(ASTNode* cppNode, ASTNode* node);
	void translateDeclaration(ASTNode* sourceNode, ASTNode* &destNode);
	void translateStatement(ASTNode* sourceNode, ASTNode* &destNode);
	ASTNode* translateIfStatement(std::string ifConditionPart, std::string ifBlockPart);
	ASTNode* translateWhileStatement(std::string whileConditionPart, std::string whileBlockPart);
	ASTNode* translateForStatement(std::string forInitializationPart,std::string forConditionPart, std::string forPostPart, std::string forBlockPart);
	void translateExpression(ASTNode* sourceNode, ASTNode*& destNode);
private:
	ASTNode* _astRoot; //this is the go ast
	ASTNode* _cppRoot; //this is the cpp ast
};