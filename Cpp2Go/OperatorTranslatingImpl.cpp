#include "AstTranslator.h"


void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* opNode = new ASTNode(sourceNode);
	ASTNode* frontNode = new ASTNode(sourceNode->children.front());
	ASTNode* backNode = new ASTNode(sourceNode->children.back());
	opNode->addChild(frontNode);
	opNode->addChild(backNode);

	if (sourceNode->value == "~")
	{
		opNode->value = "^";
		destNode->addChild(opNode);
	}
	else
	{
		destNode->addChild(opNode);
	}
}