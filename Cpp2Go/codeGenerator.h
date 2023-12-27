#pragma once
#include "AstTranslator.h"

class codeGenerator
{
public:
	// Constructor
	codeGenerator(ASTNode* destNode);

	// Desturctor
	~codeGenerator();

	void iterativeGenerate(ASTNode* node);
	
	void generateCode(ASTNode* node);
	void generateDeclaration(ASTNode* node);
	void generateStatement(ASTNode* node);
	void generateIfStatement(ASTNode* node);
	void generateWhileStatement(ASTNode* node);
	void generateForStatement(ASTNode* node);
	void generateBlock(ASTNode* node);
	void generateExpression(ASTNode* node);

	std::string getCode();
private:
	ASTNode* _destNode;
	std::string _code;
	int _countTab;
};