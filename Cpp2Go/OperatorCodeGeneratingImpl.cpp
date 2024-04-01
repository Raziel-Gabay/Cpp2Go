#include "codeGenerator.h"


void codeGenerator::generateExpression(ASTNode* node)
{
	_code += node->children.front()->value + " " + node->value + " ";
	if (node->children.back()->name.find("OPERATOR") != std::string::npos && node->children.back()->name != ADDRESS_OF_OPERATOR)
		generateExpression(node->children.back());
	else
		_code += node->children.back()->value;
	if (node->children.back()->name == ADDRESS_OF_OPERATOR)
	{
		if (node->children.back()->children.size() > 0)
		{
			_code += node->children.back()->children.front()->value;
		}
	}
}

void codeGenerator::generateExpression(std::vector<ASTNode*> nodes)
{
	std::string last_value = nodes.front()->children.back()->value;
	for (ASTNode* node : nodes)
	{
		if (node->children.front()->value == last_value)
			_code += " " + node->value + " " + node->children.back()->value;
		else
			generateExpression(node);
		last_value = node->children.back()->value;
	}
}