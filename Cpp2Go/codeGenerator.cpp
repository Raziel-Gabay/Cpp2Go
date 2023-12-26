#include "codeGenerator.h"

codeGenerator::codeGenerator(ASTNode* destNode)
	:_destNode(destNode)
{
	this->_code = generateCode(destNode);
}

codeGenerator::~codeGenerator()
{
}

void codeGenerator::iterativeGenerate(ASTNode* node)
{
}

std::string codeGenerator::generateCode(ASTNode* node)
{
	return std::string();
}

void codeGenerator::generateDeclaration(ASTNode* node)
{
}

void codeGenerator::generateStatement(ASTNode* node)
{
}

void codeGenerator::generateIfStatement(ASTNode* node)
{
}

void codeGenerator::generateWhileStatement(ASTNode* node)
{
}

void codeGenerator::generateForStatement(ASTNode* node)
{
}

void codeGenerator::generateBlock(ASTNode* node)
{
}

void codeGenerator::generateExpression(ASTNode* node)
{
}
