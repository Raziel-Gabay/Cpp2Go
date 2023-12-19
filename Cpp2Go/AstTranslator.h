#pragma once
#include"ASTNode.h"

class AstTranslator
{
public:
	ASTNode* translateProgram(ASTNode* node);
	void translateDeclaration(ASTNode* sourceNode, ASTNode* &destNode);
	void translateStatement(ASTNode* sourceNode, ASTNode* &destNode);
	ASTNode* translateIfStatement(std::string ifConditionPart, std::string ifBlockPart);
	ASTNode* translateWhileStatement(std::string whileConditionPart, std::string whileBlockPart);
	ASTNode* translateForStatement(std::string forInitializationPart,std::string forConditionPart, std::string forPostPart, std::string forBlockPart);
	void translateExpression(ASTNode* sourceNode, ASTNode*& destNode);
};