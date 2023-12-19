#include "AstTranslator.h"

ASTNode* AstTranslator::translateProgram(ASTNode* node)
{
    return nullptr;
}

void AstTranslator::translateDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
}

void AstTranslator::translateStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
}

ASTNode* AstTranslator::translateIfStatement(std::string ifConditionPart, std::string ifBlockPart)
{
    return nullptr;
}

ASTNode* AstTranslator::translateWhileStatement(std::string whileConditionPart, std::string whileBlockPart)
{
    return nullptr;
}

ASTNode* AstTranslator::translateForStatement(std::string forInitializationPart, std::string forConditionPart, std::string forPostPart, std::string forBlockPart)
{
    return nullptr;
}

void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
}
