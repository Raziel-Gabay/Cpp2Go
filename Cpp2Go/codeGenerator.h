#pragma once
#include "AstTranslator.h"

#define WHITESPACE ' '
#define COMMA_CHAR ','


class codeGenerator
{
public:
	// Constructor
	codeGenerator(ASTNode* destNode);

	// Desturctor
	~codeGenerator();

	void iterativeGenerate(ASTNode* node);
	
	void generateCode(ASTNode* node);

	// declaration generate
	void generateVariableDeclaration(ASTNode* node);
	void generateFunctionDeclaration(ASTNode* node);
	void generatePointerDeclaration(ASTNode* node);
	void generateArrayDeclaration(ASTNode* node);

	// statement generate
	void generateStatement(ASTNode* node);
	void generateIfStatement(ASTNode* node);
	void generateElseIfStatement(ASTNode* node);
	void generateElseStatement(ASTNode* node);
	void generateWhileStatement(ASTNode* node);
	void generateForStatement(ASTNode* node);
	void generateForeachStatement(ASTNode* node);

	// expression generate
	void generateExpression(ASTNode* node);
	void generateExpression(std::vector<ASTNode*> nodes);

	// other generate
	void generateFunctionCall(ASTNode* node);
	void generateStruct(ASTNode* node);
	void generateBlock(ASTNode* node);
	void generateIncludeDirective(ASTNode* node);
	void generateStdCout(ASTNode* node);
	void generateStdCin(ASTNode* node);
	void generateStdCerr(ASTNode* node);
	void generateOpenFile(ASTNode* node);

	std::string getCode();
private:
	ASTNode* _destNode;
	std::string _code;
	int _countTab;

};