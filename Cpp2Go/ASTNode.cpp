#include "ASTNode.h"

ASTNode::ASTNode(const std::string& nodeName, const std::string& nodeValue)
	: name(nodeName), value(nodeValue)
{
}

ASTNode::~ASTNode()
{
}

void ASTNode::addChild(ASTNode* childNode)
{
	children.push_back(childNode);
}
