#include "ASTNode.h"

ASTNode::ASTNode(const std::string& nodeName, const std::string& nodeValue)
	: name(nodeName), value(nodeValue)
{
}

ASTNode::ASTNode(ASTNode* otherAst)
	: name(otherAst->name), value(otherAst->value)
{
}

ASTNode::~ASTNode()
{
	for (auto child : this->children) 
	{
		delete child;
	}
}

void ASTNode::addChild(ASTNode* childNode)
{
	children.push_back(childNode);
}
