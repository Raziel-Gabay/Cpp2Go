#pragma once
#include "AstTranslator.h"

#define WHITESPACE ' '
#define COMMA ','

class codeGenerator
{
public:
	// Constructor
	codeGenerator(ASTNode* destNode);

	// Desturctor
	~codeGenerator();

	void iterativeGenerate(ASTNode* node);
	
	void generateCode(ASTNode* node);
	void generateVariableDeclaration(ASTNode* node);
	void generateFunctionDeclaration(ASTNode* node);
	void generatePointerDeclaration(ASTNode* node);
	void generateFunctionCall(ASTNode* node);
	void generateStruct(ASTNode* node);
	void generateStatement(ASTNode* node);
	void generateIfStatement(ASTNode* node);
	void generateElseIfStatement(ASTNode* node);
	void generateElseStatement(ASTNode* node);
	void generateWhileStatement(ASTNode* node);
	void generateForStatement(ASTNode* node);
	void generateBlock(ASTNode* node);
	void generateExpression(ASTNode* node);
	void generateExpression(std::vector<ASTNode*> nodes);
	void generateIncludeDirective(ASTNode* node);
	void generateArrayDeclaration(ASTNode* node);
	void generateStdCout(ASTNode* node);

	std::string getCode();
private:
	ASTNode* _destNode;
	std::string _code;
	int _countTab;

};