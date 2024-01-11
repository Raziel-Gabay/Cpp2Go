#pragma once

#include"ASTNode.h"
#include <iostream>
#include <unordered_set>

#define IDENTIFIER "IDENTIFIER"
#define ASSIGNMENT_OPERATOR "ASSIGNMENT_OPERATOR"
#define SHORT_ASSIGNMENT_OPERATOR "SHORT_ASSIGNMENT_OPERATOR"
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
#define PROGRAM "PROGRAM"
#define BLOCK "BLOCK"
#define CONDITION "CONDITION"
#define INITIALIZATION "INITIALIZATION"
#define ITERATION "ITERATION"
#define DATATYPE "DATATYPE"
#define VARIABLE_DECLARATION "VARIABLE_DECLARATION"
#define VAR_KEYWORD "VAR_KEYWORD"
#define DATATYPE_STRING "DATATYPE_STRING"
#define PARAMETER "PARAMETER"
#define RETURN_VALUE "RETURN_VALUE"
#define FUNCTION_DECLARATION "FUNCTION_DECLARATION"
#define FUNCTION_CALL "FUNCTION_CALL"
#define MEMBERS "MEMBERS"
#define STRUCT_KEYWORD "STRUCT_KEYWORD"
#define STRUCT "STRUCT"
#define INCLUDE_DIRECTIVE "INCLUDE_DIRECTIVE"
#define INCLUDE_KEYWORD "INCLUDE_KEYWORD"
#define IMPORT "IMPORT"
#define IMPORT_DIRECTIVE "IMPORT_DIRECTIVE"
#define IOSTREAM "iostream"
#define ARRAY_DECLARATION "ARRAY_DECLARATION"
#define ARRAY_LENGTH "ARRAY_LENGTH"
#define ADDRESS_OF_OPERATOR  "ADDRESS_OF_OPERATOR"
#define POINTER_DECLARATION "POINTER_DECLARATION"
#define POINTER_OPERATOR "POINTER_OPERATOR"

class AstTranslator
{
public:
	// Constructor
	AstTranslator(ASTNode* cppRoot);

	// Desturctor
	~AstTranslator();

	ASTNode* translateProgram(ASTNode* node);
	void iterativeTranslate(ASTNode* cppNode, ASTNode* node);
	void translateVariableDeclaration(ASTNode* sourceNode, ASTNode* &destNode);
	void translateFunctionDeclaration(ASTNode* sourceNode, ASTNode*& destNode);
	void translateArrayDeclaration(ASTNode* sourceNode, ASTNode*& destNode);
	void translatePointerDeclaration(ASTNode* sourceNode, ASTNode*& destNode);
	void translateStruct(ASTNode* sourceNode, ASTNode*& destNode);
	void translateStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateIfStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateElseIfStatement(ASTNode* sourceNode, ASTNode*& destNode);
	void translateElseStatement(ASTNode* sourceNode, ASTNode*& destNode);
	void translateWhileStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateForStatement(ASTNode* sourceNode, ASTNode* &destNode);
	void translateBlock(ASTNode* sourceNode, ASTNode* &destNode);
	void translateExpression(ASTNode* sourceNode, ASTNode* &destNode);
	void translateIncludeDirective(ASTNode* sourceNode, ASTNode*& destNode);
	void translateType(ASTNode* sourceNode, ASTNode*& destNode);


	ASTNode* getAST();
private:
	ASTNode* _astRoot; //this is the go ast
	ASTNode* _cppRoot; //this is the cpp ast
	std::unordered_set<std::string> _identifiers;
};